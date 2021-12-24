//NodeMCU script
void setup() {
  Serial.begin(115200);
  while(!Serial)
  {
    ;
  }
}

void loop() {
  if(Serial.available())
  {
    //Serial.print("sup");
    Serial.write(Serial.read());
  }
}
