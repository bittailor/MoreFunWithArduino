const int inputPin = 2;
const int outputPin = 3;

boolean oldInputState; 

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
  oldInputState = digitalRead(inputPin);
}

void loop() {
  int state = digitalRead(inputPin);
  if (state != oldInputState) {
    oldInputState = state;
    digitalWrite(outputPin, !oldInputState);
  }  
}

