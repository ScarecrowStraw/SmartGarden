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
#define WIFI_SSID "TP-Link_C78F"   
#define WIFI_PASSWORD "0123456@"
#define DHTPIN D5
#define DHTTYPE DHT21

long TIME_SLEEP_LOOP = 1000*3600;
long COUNT = 6;
long TIME_OUT_WATER = 0;
long TIME_OUT_DEFAULT = TIME_SLEEP_LOOP * COUNT;
long CURRENT_TIME_DEFAULT_SLEEP = 0;
long TIME_WATER_ = 0;

const long utcOffsetInSeconds = 7*3600;
float lux=0;
int value = 0;
float h = 0;
float t = 0;
   
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

int red = D6;
int wait_time = 6;
int run_time = 5;
int led = D7;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void getTime() {
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
}
int hour = timeClient.getHours();
int minutes = timeClient.getMinutes();

void defaut(){
  if(TIME_WATER_ == TIME_OUT_DEFAULT){
    tuoi_cay();
  }
  
  //delay(1000*3600*wait_time);

}

void setup() {
  ESP.eraseConfig();
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println(WiFi.localIP());
  Serial.print("connected: ");
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
void tuoi_cay(){
    digitalWrite(red, HIGH);
    Serial.println("Turn on");
    delay(1000*run_time);
    digitalWrite(red, LOW);
    Serial.println("Turn off");
}

void set_time_out_water(int cycle){
  TIME_OUT_WATER = cycle * TIME_SLEEP_LOOP;
}

void mode_1(){
  timeClient.update();
    getTime();
    set_time_out_water(8);
    
    
    Firebase.setFloat("LightMeter: ",lux);
    Firebase.setInt("DoAmDat_STATUS", value);
      Firebase.setFloat("Humidity_STATUS", h);
      Firebase.setFloat("Temperature_STATUS", t);  
      FirebaseObject object = Firebase.get("/");
    String path = object.getString("demo");
   // int time1 = object.getInt("time".child("gio"));
    Firebase.setString("LEDSTATUS", path);
   
   if (Firebase.failed()){
     Serial.println(Firebase.error());
     return;
    }   
}

void check_tuoi_cay(){
  if(TIME_WATER_ == TIME_OUT_WATER){
    TIME_WATER_ = 0;
    tuoi_cay();
  }
}

void doc_cam_bien(){
    lux = lightMeter.readLightLevel();
    value = analogRead(A0);
    h = dht.readHumidity();
    t = dht.readTemperature();
   
    if (isnan(h) || isnan(t)) {
      Serial.print("Failed to read from DHT sensor!");
      return;
    }
     if ( value < 500 && t < 30){
        digitalWrite (red, 0);
        delay (1000*5);
    }
    if ( value > 500 && t > 30){
      digitalWrite (red, 0);
      delay(1000*10);
    }
    if ( value > 500 && t < 30){
      digitalWrite (red, 0);
      delay(1000*5);
    }
    if ( hour > 6 && hour < 18){
      if (lux < 500) {
        digitalWrite (led, HIGH);
      }TIME_WATER_
      if (lux > 500){
        digitalWrite (led, 0);
      }
    }
}

void loop() {
   doc_cam_bien();
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Set to defaut");
        TIME_OUT_WATER = TIME_OUT_DEFAULT;
   }else{
      mode_1();
   }
   check_tuoi_cay();
   
    delay(TIME_SLEEP_LOOP);
    
   TIME_WATER_ += TIME_SLEEP_LOOP;
} 