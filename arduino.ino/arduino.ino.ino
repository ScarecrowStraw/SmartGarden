#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

// Relay pin is controlled with D8. The active wire is connected to Normally Closed and common
int relay = 8;
String state = "0";

char check(String state){
  for (int i = 0; i < state.length(); i++){
    if (state[i] == '0'){
      return '0';
    }
    if (state[i] == '1'){
      return '1';
    }
  }
  return 'n';
}

void setup() {
  // Pin for relay module set as output
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  digitalWrite(relay, LOW);
}

void loop() {
  if(mySerial.available() > 0){
    state = mySerial.readStringUntil('\n');

    Serial.print(" I received:");
    Serial.println(state);
    if(check(state) == '0'){
      digitalWrite(relay, HIGH);
      Serial.println("ON");    
    }
    else if (check(state) == '1'){
      digitalWrite(relay, LOW);
      Serial.println("OFF");
    }
  }
}
