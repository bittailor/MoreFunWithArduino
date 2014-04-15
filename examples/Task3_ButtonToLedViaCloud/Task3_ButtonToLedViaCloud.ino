#include <SPI.h>
#include <BtMqttSn.h>

#define TEAM 1

//-----

#define xstr(s) str(s)
#define str(s) #s

#define CLIENT_NODE_ID (TEAM)
#define CLIENT_ID "GeekTeam" xstr(TEAM)
#define PUBLISH_TOPIC "Geek/Team/" xstr(TEAM) "/Button"
#define SUBSCRIBE_TOPIC "Geek/Team/" xstr(TEAM) "/Led"

#define GATEWAY_NODE_ID 0
#define CHIP_ENABLE 9
#define CHIP_SELECT 10

//-----


const int inputPin = 2;
const int outputPin = 3;
boolean oldInputState; 

MqttSnClient client;

void setup() {
  Serial.begin(9600);
  Serial << "setup ..." << endl;
  
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
  oldInputState = digitalRead(inputPin);
  
  client.begin(CHIP_ENABLE, CHIP_SELECT, CLIENT_NODE_ID, GATEWAY_NODE_ID, CLIENT_ID, &subscribeCallback);
  while (!client.connect()) {
    Serial << "connect failed => retry in 10s" << endl;
    delay(10000);   
  }
  
  client.subscribe(SUBSCRIBE_TOPIC);   
  publishButton(oldInputState);

  Serial << "... all setup" << endl;
}

void loop() {
  client.loop();
  int state = digitalRead(inputPin);
  if (state != oldInputState) {
    oldInputState = state;
    publishButton(oldInputState);
  }  
}

void subscribeCallback(const char* iTopic, const char*  iMessage) {
  Serial << "sub> " << iTopic << ":" << iMessage << endl;  
  if(stringsAreEqual(iMessage, "on")) {
    digitalWrite(outputPin, HIGH);  
  }
  if(stringsAreEqual(iMessage, "off")) {
    digitalWrite(outputPin, LOW);  
  }
}

void publishButton(boolean state) {
  const char* message = "on";
  if(state == HIGH) {
    message = "off";  
  }
  Serial << "pub> " << PUBLISH_TOPIC << ":" << message << endl;
  client.publish(PUBLISH_TOPIC,message);
}

bool stringsAreEqual(const char* iStringOne, const char*  iStringTwo) {
  return strcmp(iStringOne, iStringTwo) == 0; 
}
