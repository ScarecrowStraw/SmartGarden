#include <ESP8266WiFi.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

//SSID and Password of your WiFi router
const char* ssid = "000000000";
const char* password = "11112222";

int wait_time = 2;
int run_time = 10;

const long utcOffsetInSeconds = 7*3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// int last_time_1 = 0;// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int getTime() {
//  Serial.print(daysOfTheWeek[timeClient.getDay()]);
//  Serial.print(", ");
//  Serial.print(timeClient.getHours());
//  Serial.print(":");
//  Serial.print(timeClient.getMinutes());
//  Serial.print(":");
//  Serial.println(timeClient.getSeconds());
//  server.send(200, "text/plain", String(timeClient.getFormattedTime()) + "\t" + String(daysOfTheWeek[timeClient.getDay()]));
  int something = timeClient.getMinutes();
  return something;
}


void setup() {
  Serial.begin(9600);
  ESP.eraseConfig();
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  
  timeClient.begin();
  Serial.println(getTime());
}

int last_time_1 = getTime();
//int last_time_2 = getTime();
int state = 0;
void loop() {
  timeClient.update();
  if ((timeClient.getHours() == 6) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() == 0)){
    Serial.println("Turn on");
  }
  if ((timeClient.getHours() == 6) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() >= run_time){
    Serial.println("Turn off");
  }
  if ((timeClient.getHours() == 12) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() == 0)){
    Serial.println("Turn on");
  }
  if ((timeClient.getHours() == 12) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() >= run_time){
    Serial.println("Turn off");
  }
  if ((timeClient.getHours() == 18) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() == 0)){
    Serial.println("Turn on");
  }
  if ((timeClient.getHours() == 18) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() >= run_time){
    Serial.println("Turn off");
  }
  if ((timeClient.getHours() == 0) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() == 0)){
    Serial.println("Turn on");
  }
  if ((timeClient.getHours() == 0) && (timeClient.getMinutes() == 0) && (timeClient.getSeconds() >= run_time){
    Serial.println("Turn off");
  }
}
