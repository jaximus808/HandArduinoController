//Uno script

#include<Servo.h>

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

int dataNumber = 0;    
bool reading = false; 

int pos = 0; 

int motorPos = 0; 

Servo servos[3]; 


void setup() {
  Serial.begin(115200);
  while(!Serial)
  {
    ;
  }
  Serial.println("connected");
//  servos[0].attach(9);
//  servos[1].attach(10);
//  servos[2].attach(11);
}



void loop() {
  while(Serial.available())
  {
    char c = Serial.read();
    Serial.println(c);
    if(c == '<')
    {
      reading = true; 
    }
    else if(c=='>' && reading)
    {
      reading = false; 
      String stringInt = "";
      for(int i = 0; i < pos; i++)
      {
        //Serial.write(receivedChars[i]);
        if(receivedChars[i] == ',')
        {
          int rotation = stringInt.toInt(); 
          if(rotation < 0) rotation = 0; 
          Serial.println(rotation);
          stringInt = "";
          //servos[motorPos].write(rotation);
          motorPos++; 
        }
        else
        {
          stringInt += receivedChars[i];
        }
        
      }
      pos = 0; 
      motorPos = 0; 
      Serial.println();
    }
    else if(reading)
    {
      receivedChars[pos] = c; 
      pos++;
      if(pos == 32)
      {
        pos == 31;
      }
    }
  }
}
