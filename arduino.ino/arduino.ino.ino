
// Relay pin is controlled with D8. The active wire is connected to Normally Closed and common
int relay = 8;
int state = 0;

void setup() {
  // Pin for relay module set as output
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(if (Serial.available() > 0)){
    state = Serial.read(); // read the incoming byte:

    Serial.print(" I received:");
    Serial.println(state);
    if(state == 1){
      digitalWrite(relay, HIGH);
      Serial.println("ON");    
    }
    else {
      digitalWrite(relay, LOW);
      Serial.println("OFF");
    }
  }
  delay(50);
}
