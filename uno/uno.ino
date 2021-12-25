//Uno script
void setup() {
  Serial.begin(115200);
  while(!Serial)
  {
    ;
  }
  Serial.println("connected");
  
}

void loop() {
  if(Serial.available())
  {
    Serial.write(Serial.read());
    
  }
  
}
