//NodeMCU script
#include<SoftwareSerial.h>
//
//SoftwareSerial s(3,1);
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(2000);
}

void loop() {
  Serial1.println("H: ");
  delay(1000);
}
