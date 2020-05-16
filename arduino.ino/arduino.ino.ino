#include <SoftwareSerial.h>

// Relay pin is controlled with D8. The active wire is connected to Normally Closed and common
int relay = 8;
String state = "0";

void setup() {
  // Pin for relay module set as output
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    state = Serial.readStringUntil('\n'); // read the incoming byte:

    Serial.print(" I received:");
    Serial.println(state);
    if(state == "1"){
      digitalWrite(relay, HIGH);
      Serial.println("ON");    
    }
    else if (state == "0"){
      digitalWrite(relay, LOW);
      Serial.println("OFF");
    }
  }
  delay(50);
}
