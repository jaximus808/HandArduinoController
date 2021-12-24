//Uno script
#include <SoftwareSerial.h>

SoftwareSerial Serial1(10,11); 

String packetStr;
int y = 0;
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.print("program starting");
  delay(2000);
  
}

void loop() {
  String x = "";
  for(int i = 0; i < y; i++ )
  {
    x+= ".";
  }
  if(y < 20)
  {
    y++;
  }
  Serial.println("sending");
  packetStr = String("hello") + String(x);
  Serial1.println(packetStr);
  delay(1000);
}
