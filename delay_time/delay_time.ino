#include <ESP8266WiFi.h>

int wait_time = 6;
int run_time = 5;

int relay = 16; // GPIO 16 (D0)

void setup(){
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop(){
  digitalWrite(relay, HIGH);
  Serial.println("Turn on");
  delay(1000*run_time);
  digitalWrite(relay, LOW);
  Serial.println("Turn off");
  delay(1000*3600*wait_time - 1000*run_time);
}
