#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

SoftwareSerial mySerial(13, 15); // RX, TX

String cmd;
int wait_time = 2;
int run_time = 1;

//SSID and Password of your WiFi router
const char* ssid = "000000000";
const char* password = "11112222";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// int last_time_1 = 0;// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

WiFiServer server(80);

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

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(19200);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
//  mySerial.println("Hello, world?");
  Serial.println("Hello");
  ESP.eraseConfig();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();
  timeClient.begin();
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
int last_time_1 = getTime();
int state = 0;

void loop() // run over and over
{
  WiFiClient client = server.available();
  if (!client){
    return;}
    
  Serial.println("Waiting for new client");
  while(!client.available())
  {
    delay(1);
  }
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
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
  
  int value = HIGH;
  if(state == 0){
    
    mySerial.println("1");
    value = LOW;
  }

  if(state == 1){

    mySerial.println("0");
    value = HIGH;
  } 
  
//*------------------HTML Page Code---------------------*//

  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print(" CONTROL Pump: ");
  if(value == HIGH){
    client.print("OFF");
  }
  else
  {
    client.print("ON");
  }
  client.println("<br><br>");
  client.println(String(timeClient.getFormattedTime()) + "\t" + String(daysOfTheWeek[timeClient.getDay()]));
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
