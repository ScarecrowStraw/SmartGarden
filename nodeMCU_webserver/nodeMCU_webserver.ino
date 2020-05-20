#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LED 2  

//SSID and Password of your WiFi router
const char* ssid = "TP-Link_C78F";
const char* password = "0123456@";
int count = 0;

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);
  ESP.eraseConfig();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Lacie","123456789");
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  //Power on LED state off
  digitalWrite(LED,HIGH);
  
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
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/",[]{
    Serial.println(String("Co ng truy cap") + count++);
    server.send(200, "text/plain", "Ngao roi");
  });
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
  delay(1);
}
