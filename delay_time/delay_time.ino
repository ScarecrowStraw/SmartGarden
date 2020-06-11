#include <ESP8266WiFi.h>

int relay = D0;

void setup(){
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.println("ON");
  delay(1000*5);
  digitalWrite(relay, HIGH);
  Serial.println("OFF");
  int i = 0;
  for( i = 0; i<4; i++){
    delay(1000*3600);
  }
  digitalWrite(relay, HIGH);
  delay(1000*runtime);
  delay(1000*3600*4);
}

void loop(){
  digitalWrite(relay, LOW);
  delay(5*1000);
  digitalWrite(relay, HIGH);
  for(int i = 0; i < 6; i++){
    delay(1000*3600);
  }
  
  digitalWrite(relay, LOW);
  delay(5*1000);
  digitalWrite(relay, HIGH);
  for(int i = 0; i < 18; i++){
    delay(1000*3600);
  }
}



