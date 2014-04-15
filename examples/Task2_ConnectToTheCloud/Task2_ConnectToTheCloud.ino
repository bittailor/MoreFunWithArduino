#include <SPI.h>
#include <BtMqttSn.h>

//-----

#define TEAM 1

//-----

#define xstr(s) str(s)
#define str(s) #s


#define CLIENT_NODE_ID (TEAM)                           // the RF network node ID for your arduino e.g 1
#define CLIENT_ID "GeekTeam" xstr(TEAM)                 // the client ID of the team e.g GeekTeam1
#define PUBLISH_TOPIC "Geek/Team/" xstr(TEAM) "/Button" // the teams publish topic e.g. 'Geek/Team/1/Button'
#define SUBSCRIBE_TOPIC "Geek/Team/" xstr(TEAM) "/Led"  // the teams subscribe topic e.g. 'Geek/Team/1/Led'

#define GATEWAY_NODE_ID 0                               // the RF network node ID of the MQTT-SN gateway
#define CHIP_ENABLE 9                                 
#define CHIP_SELECT 10

//-----

MqttSnClient client;

void setup() {
  Serial.begin(9600);
  Serial << "setup ..." << endl;
  
  client.begin(CHIP_ENABLE, CHIP_SELECT, CLIENT_NODE_ID, GATEWAY_NODE_ID, CLIENT_ID, &subscribeCallback);
  
  while (!client.connect()) {
    Serial << "connect failed => retry in 10s ..." << endl;
    delay(10000); 
  Serial << "... retry ..." << endl;  
  }
  
  client.subscribe(SUBSCRIBE_TOPIC); 
  
  client.publish(PUBLISH_TOPIC,"off");

  Serial << "... all setup" << endl;
}

void loop() {
  client.loop(); 
}

void subscribeCallback(const char* iTopic, const char*  iMessage) {
  Serial << "sub> " << iTopic << ":" << iMessage << endl;  
}

