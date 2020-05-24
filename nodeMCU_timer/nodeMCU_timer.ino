#include <ESP8266WiFi.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

//SSID and Password of your WiFi router
const char* ssid = "000000000";
const char* password = "11112222";

int wait_time = 2;
int run_time = 1;

const long utcOffsetInSeconds = 3600;

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
//  getTime();
//  int getTime1 = getTime();
//  Serial.println(getTime1);
  if ((getTime() - last_time_1 >= wait_time) && state == 0){
    Serial.println(getTime());
    Serial.println("Turn on");
    state = 1;
  }
  if ((getTime() - last_time_1 >= (run_time + wait_time)) && state == 1){
    Serial.println(getTime());
    Serial.println("Turn off");
    state = 0;
    last_time_1 = getTime();
    Serial.println("Last time :");
    Serial.println(last_time_1);
    
  }

  if ((getTime() - last_time_1) < 0){
    int check = 60 - last_time_1 + getTime();
    if ((check > wait_time) && state == 0){
      Serial.println(getTime());
      Serial.println("Turn on");
      state = 1;
    }
    if(check > (run_time + wait_time) && state == 1){
      Serial.println(getTime());
      Serial.println("Turn off");
      state = 0;
      last_time_1 = getTime(); 
    }
  }
}
