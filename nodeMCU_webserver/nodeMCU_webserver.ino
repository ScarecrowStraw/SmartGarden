#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

int LED = 16; // GPIO16 (D0)

ESP8266WebServer server(80);

uint16_t x, y;
boolean flag = false;

//SSID and Password of your WiFi router
const char* ssid = "TP-Link_C78F";
const char* password = "0123456@";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Khi trình duyệt web vào địa chỉ của server, server sẽ gủi về cho client giao diện điều khiển.
void handleRoot() {
    // khai báo biến s lưu trữ code giao diện html
    String s = "<html>";
    s += "<head>";
    s += "<meta charset='UTF-8'>";
    s += "</head>";
    s += "<div class=\"container\">";
    s += "<center><h3>ĐLED CONTROL</h3>";
    s += "<div class=\"row\">";
    s += "<table>";
    s += "<tr>";
    s += "<td>LED </td>";
    s += "<td><form menthod='get'><input class='button' type='submit' name='LED' value='ON' ></form></td>";
    s += "<td><form menthod='get'><input class='button' type='submit' name='LED' value='OFF' ></form></td>";
    s += "</tr>";
    s += "</table>";
    s += "</center>";
    s += "</html>";
    
    server.send(200, "text/html", s); // gửi giao diện về cho trình duyệt web hiển thị.
    
    if(server.hasArg("LED")){ // kiểm tra trong chuỗi request từ client gủi về server có chứa dữ liệu có tên LED1 không
    String led1 = server.arg("LED"); // nếu có thì sẽ lấy dữ liệu của LED1 chứa.
        Serial.println("LED=");
        Serial.println(led1);
        if(led1 == "OFF") // nếu dữ liệu chứa bằng "ON"
            digitalWrite(LED, HIGH); // bật LED1
        else
            digitalWrite(LED, LOW); // tắt LED1
     }
}

void handleNotFound(){
      String message = "File Not Found\n\n";
      message += "URI: ";
      message += server.uri();
      message += "\nMethod: ";
      message += (server.method() == HTTP_GET)?"GET":"POST";
      message += "\nArguments: ";
      message += server.args();
      message += "\n";
      for (uint8_t i=0; i<server.args(); i++){
            message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      }
      server.send(404, "text/plain", message);
}

void getTime() {
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  server.send(200, "text/plain", String(timeClient.getFormattedTime()) + "\t" + String(daysOfTheWeek[timeClient.getDay()]));
//  return timeClient.getMinutes();
}

void setup() {
  Serial.begin(9600);
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
  
  timeClient.begin();
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.onNotFound(handleNotFound);
  server.on("/time", getTime);
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  timeClient.update();
}
