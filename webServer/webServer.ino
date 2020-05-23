#include <ESP8266WiFi.h>
#include <time.h>

#include <WiFiUdp.h>
 
#include <NTPClient.h>               // Include NTPClient library

//SSID and Password of your WiFi router
const char* ssid = "TP-Link_C78F";
const char* password = "0123456@";

int LED = 16; // GPIO16 (D0)

int timezone = 7 * 3600;
int dst = 0;

WiFiServer server(80);

void setup(){
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.print("Connecting to the Newtork");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
//  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
//  Serial.println("\nWaiting for Internet time");
//
//  while(!time(nullptr)){
//     Serial.print("*");
//     delay(1000);
//  }
//  Serial.println("\nTime response....OK");
}

void loop(){
  WiFiClient client = server.available();
//  time_t now = time(nullptr);
//  struct tm* p_tm = localtime(&now);
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
  
  int value = HIGH;
  if(request.indexOf("/LED=ON") != -1){
    
    digitalWrite(LED, LOW); // Turn LED ON
    value = LOW;
  }

  if(request.indexOf("/LED=OFF") != -1){

    digitalWrite(LED, HIGH); // Turn LED OFF
    value = HIGH;
  } 
  
//*------------------HTML Page Code---------------------*//

  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print(" CONTROL LED: ");
  if(value == HIGH){
    client.print("OFF");
  }
  else
  {
    client.print("ON");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");
//  client.print(String(p_tm->tm_mday));
//  client.print("/");
//  client.print(String(p_tm->tm_mon + 1));
//  client.print("/");
//  client.print(String(p_tm->tm_year + 1900));
//  
//  client.print(" ");
//  
//  client.print(String(p_tm->tm_hour));
//  client.print(":");
//  client.print(String(p_tm->tm_min));
//  client.print(":");
//  client.println(String(p_tm->tm_sec));
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
