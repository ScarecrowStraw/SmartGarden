#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define FIREBASE_HOST "demo1-c11e3.firebaseio.com"
#define FIREBASE_AUTH ""  
#define WIFI_SSID "loudou"   
#define WIFI_PASSWORD "duongpr0"
#define DHTPIN D3
#define DHTTYPE DHT21

long TIME_SLEEP_LOOP = 10000*6;
long COUNT = 6;
long TIME_OUT_WATER = 0;
long TIME_OUT_DEFAULT = 100 * 1000;
long CURRENT_TIME_DEFAULT_SLEEP = 0;
long TIME_WATER_ = 0;
long TIME_SENSOR_ = 0;
long TIME_CHECK_SENSOR = 300 * 1000;

const long utcOffsetInSeconds = 7*3600;

float lux=0;
int value = 0;
float h = 0;
float t = 0;

int run_time = 5;

int red = D6;
int led = D7;
int relay1 = D8;
int relay2 = D4;

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
}

      // defaut 

void defaut(){
  if(TIME_WATER_ == TIME_OUT_DEFAULT){
      tuoi_cay();
  }
}
      // kiem tra tuoi

void check_tuoi_cay(){
  if(TIME_WATER_ == TIME_CHECK_SENSOR){
    TIME_WATER_ = 0;
    tuoi_cay();
  }
}

int hour = timeClient.getHours();
int minutes = timeClient.getMinutes();

      // get Time

void getTime() {
  hour = timeClient.getHours();
  minutes = timeClient.getMinutes();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
}

      // Tuoi cay

void tuoi_cay(){
    digitalWrite(red, 0);
    
    delay(1000*run_time);
  
    digitalWrite(red, HIGH);
    
}

      // che do online

void mode_1(){
    
    timeClient.update();
    getTime();
    //set_time_out_water(8);
    FirebaseObject object = Firebase.get("/");
    
    Firebase.setFloat("LightMeter: ",lux);
    Firebase.setInt("DoAmDat_STATUS", value);
    Firebase.setFloat("Humidity_STATUS", h);
    Firebase.setFloat("Temperature_STATUS", t);

      // chon gio tuoi tieu
    int timegio = object.getInt("gio");
    int timephut = object.getInt("phut");
    Serial.print (timegio);
    Serial.print (timephut);
    
    if (timegio == hour  && timephut == minutes){
        Serial.print("ab");
        digitalWrite(red, HIGH);
        delay(1000*5);
    }
    else{
        digitalWrite(red, LOW);
    }
    int timegio2 = object.getInt("gio2");
    int timephut2 = object.getInt("phut2");
    
    if (timegio2 == hour  && timephut2 == minutes){
        digitalWrite(red, HIGH);
        delay(1000*5);
       }
    else{
        digitalWrite(red, LOW);
    }
   //
    
   if (Firebase.failed()){
     Serial.println(Firebase.error());
     return;
    }   
}
 
    // check cam bien
    
void check_sensor(){
     if(TIME_SENSOR_ == TIME_CHECK_SENSOR){
     TIME_SENSOR_ = 0;
     doc_cam_bien();
    }
}
      // kiem tra tuoi cua cam bien

void doc_cam_bien(){
    
    if ( value < 500 && t < 30){
        digitalWrite (red, HIGH);
        delay (1000*5);
        digitalWrite (red, 0);
    }
    if ( value > 500 && t > 30){
      digitalWrite (red,HIGH);
      delay(1000*10);
      digitalWrite (red, 0);
      
    }
    if ( value > 500 && t < 30){
      digitalWrite (red, HIGH);
      delay(1000*5);
      digitalWrite (red, 0);
    }
    if ( hour > 6 && hour < 18){
      if (lux < 500) {
        digitalWrite (led, HIGH);
      }
      if (lux > 500){
        digitalWrite (led, 0);
      }
    }
}
   
      // Main

void loop() {
   lux = lightMeter.readLightLevel();
   value = analogRead(A0);
   h = dht.readHumidity();
   t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
      Serial.print("Failed to read from DHT sensor!");
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Set to defaut");
    TIME_OUT_WATER = TIME_OUT_DEFAULT;
    TIME_WATER_ += TIME_SLEEP_LOOP;
   }
   else{
      
      mode_1();
   }
   
   check_sensor();
   check_tuoi_cay();
   
   delay(TIME_SLEEP_LOOP);
   
   TIME_SENSOR_ += TIME_SLEEP_LOOP;

} 
