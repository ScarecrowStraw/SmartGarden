#include <SoftwareSerial.h>

SoftwareSerial mySerial(13, 15); // RX, TX

String cmd;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
//  mySerial.println("Hello, world?");
  Serial.println("Hello");
}

void loop() // run over and over
{
  if (Serial.available() > 0){
    cmd = Serial.readStringUntil('\n');
    Serial.println(cmd);
    mySerial.println(cmd);
  }
}
