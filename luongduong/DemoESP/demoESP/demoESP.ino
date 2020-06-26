#include <FirebaseArduino.h>
#include <string.h>
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "demo1-c11e3.firebaseio.com"
#define FIREBASE_AUTH ""  
#define WIFI_SSID "TP-Link_C78F"   
#define WIFI_PASSWORD "0123456@!"
#define DHTPIN D3
#define DHTTYPE DHT21

long TIME_SLEEP_LOOP = 1000*60;
long TIME_OUT_DEFAULT = 60 * 1000;
long TIME_CHECK_SENSOR = 3600 * 1000;

long TIME_OUT_WATER = 0;
long TIME_WATER_ = 0;
long TIME_SENSOR_ = 0;

const long utcOffsetInSeconds = 7*3600;

float lux=0;
int value = 0;
float h = 0;
float t = 0;
float percent = 0;

int red = D6;
int led = D7;
int relay1 = D4;
int relay2 = D5;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
   
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter(0x23);
  
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

byte thermometro[8] = //icon for termometer
{
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};


byte igrasia[8] = //icon for water droplet
{
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
};

      //set up
  
  
void setup() {
  ESP.eraseConfig();
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if(WiFi.status() == WL_CONNECTED){
    Serial.print("Connected");
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
  Wire.begin();
  
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }
  
  timeClient.begin();
  getTime();
  
  dht.begin();
  pinMode(red, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(led, OUTPUT);
}

int hour = timeClient.getHours();
int minutes = timeClient.getMinutes();
String ngay = daysOfTheWeek[timeClient.getDay()];
        
      // get Time

void getTime() {
  
  hour = timeClient.getHours();
  minutes = timeClient.getMinutes();
  ngay = daysOfTheWeek[timeClient.getDay()];
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
}

      // Tuoi cay

void tuoi_cay(int run_time){
    digitalWrite(relay2, 0);
    digitalWrite(relay1, 0);
    
    TIME_WATER_ = 0;
    digitalWrite(red, HIGH);
    digitalWrite(relay1, HIGH);
    
    delay(1000*run_time);
  
    digitalWrite(red, 0);
    digitalWrite(relay1, 0);
}      
  
  // Defaut 

void defaut(){
  
  if(TIME_WATER_ >= TIME_OUT_DEFAULT){
       tuoi_cay(15);
  }
}
      // Che do online

void mode_1(){
    
    timeClient.update();
    getTime();
    FirebaseObject object = Firebase.get("/");
    
    Firebase.setFloat("LightMeter",lux);
    Firebase.setInt("DoAmDat_STATUS", percent);
    Firebase.setFloat("Humidity_STATUS", h);
    Firebase.setFloat("Temperature_STATUS", t);

      // chon gio tuoi tieu
    int timegio = object.getInt("gio");
    int timephut = object.getInt("phut");
    
    String lichbon = object.getString("lich");
   
    if (timegio == hour  && timephut == minutes){
        tuoi_cay(15);
    }

    int timegio2 = object.getInt("gio2");
    int timephut2 = object.getInt("phut2");
    
    if (timegio2 == hour  && timephut2 == minutes && lichbon != ngay){
        tuoi_cay(15);
    }

   // Chon ngay bon phan
   if (lichbon == ngay && timegio2 == hour && timephut2 == minutes){
        digitalWrite(relay2, HIGH);
        digitalWrite(red, HIGH);
        delay (1000*15);
        
        digitalWrite(red, 0);
        digitalWrite(relay2, 0);
      }
      
   if (Firebase.failed()){
     Serial.println(Firebase.error());
     return;
    }   
}
 
    // Check cam bien
    
void check_sensor(){
     if(TIME_SENSOR_ == TIME_CHECK_SENSOR){
     TIME_SENSOR_ = 0;
     doc_cam_bien();
    }
}
      
      // Kiem tra dieu kien tuoi cua cam bien

void doc_cam_bien(){
    if ( percent < 50 && t > 30){
        tuoi_cay(15);
    }
    else if ( percent > 50 && t < 30){
        tuoi_cay(10);
    }
    else if ( h < 50 && t < 30){
        tuoi_cay(5);
    }
    else if (h < 50 && t > 30){
      tuoi_cay(5);
    }
    
    if ( hour > 6 && hour < 18){
      if (lux < 500) {
        digitalWrite (led, HIGH);
      }
      if (lux > 500){
        digitalWrite (led, 0);
      }
    }
    
    if (hour < 6 || hour > 18){
        digitalWrite (led, 0);
    }
}   
      // Main

void loop() {
   lux = lightMeter.readLightLevel();
   value = analogRead(A0);
   percent = map(value, 0, 1023, 0, 100);
   h = dht.readHumidity();
   t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
      Serial.print("Failed to read from DHT sensor!");
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Set to defaut");
    TIME_OUT_WATER = TIME_OUT_DEFAULT;
    defaut();
   }
   else{
      mode_1();
   }
   
   check_sensor();
   
   delay(TIME_SLEEP_LOOP);
   
   TIME_WATER_ += TIME_SLEEP_LOOP;
   TIME_SENSOR_ += TIME_SLEEP_LOOP;

} 
