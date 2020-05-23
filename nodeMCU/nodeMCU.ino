#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial mySerial(13, 15); // RX, TX

String cmd;

//SSID and Password of your WiFi router
const char* ssid = "TP-Link_C78F";
const char* password = "0123456@";

WiFiServer server(80);

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
//  mySerial.println("Hello, world?");
  Serial.println("Hello");
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
}

void loop() // run over and over
{
  if (Serial.available() > 0){
    cmd = Serial.readStringUntil('\n');
    Serial.println(cmd);
    mySerial.println(cmd);
  }

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
  
  int value = HIGH;
  if(request.indexOf("/Pump=ON") != -1){
    
    mySerial.println("1");
    value = LOW;
  }

  if(request.indexOf("/Pump=OFF") != -1){

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
  client.println("<a href=\"/Pump=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/Pump=OFF\"\"><button>OFF</button></a><br />");
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
