#include <ESP8266WiFi.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

//SSID and Password of your WiFi router
const char* ssid = "TP-Link_C78F";
const char* password = "0123456@";

int check = 4;
int run_time = 10;
int hours[check] = {15, 15, 15, 15};
int minutes[check] = {10, 15, 20, 25};

int LED = 16; // GPIO16 (D0)

const long utcOffsetInSeconds = 7*3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// int last_time_1 = 0;// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

WiFiServer server(80);

void getTime() {
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
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
  getTime();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

int state = 0;
void loop() {
  timeClient.update();
  getTime();

  int i = 0;
  for (i = 0; i< check; i++){
    if ((timeClient.getHours() == hours[i]) && (timeClient.getMinutes() == minutes[i]) && (timeClient.getSeconds() == 0)){
      Serial.println("Turn on");
      digitalWrite(LED, HIGH);
    }
    if ((timeClient.getHours() == hours[i]) && (timeClient.getMinutes() == minutes[i]) && (timeClient.getSeconds() >= run_time)){
      Serial.println("Turn off");
      digitalWrite(LED, LOW);
    }
  }
  delay(1000);
}
