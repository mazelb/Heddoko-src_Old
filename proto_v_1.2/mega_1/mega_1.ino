/*
THIS IS THE SLAVE MEGA
*/

#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

int softTx = 11;  
int softRx = 10; 

SoftwareSerial softSerial(softRx, softTx);

char    data[100];
String  tempStr;
int     mega;
int     curMega = 0;
int     prevMega = 0;
char    inChar;
int     index = 0;
int     goFlag = 0;
int     ledPin = 13;

void flushIncomingSerial1() 
{
  while(Serial1.available()) { Serial1.read(); }
}

void flushIncomingSerial2() 
{
  while(Serial2.available()) { Serial2.read(); }
}

void flushIncomingSerial3() 
{
  while(Serial3.available()) { Serial3.read(); }
}

void setup()
{ 
  Serial.begin(9600); 
  
  Serial1.begin(19200); 

  Serial2.begin(19200); 
  
  Serial3.begin(19200); 
  
  softSerial.begin(9600);
} 

 
void loop() 
{
  digitalWrite(ledPin, LOW);
  curMega = 0;
  curMega = softSerial.read();
   
  if (curMega != prevMega) 
  {
    digitalWrite(ledPin, HIGH);
    Serial1.print(curMega); 
    Serial1.flush();

    Serial2.print(curMega);
    Serial2.flush();

    Serial3.print(curMega);
    Serial3.flush();
    
    prevMega = curMega;

    int accel1[3] = {0, 0, 0};
    int accel2[3] = {0, 0, 0};
    int accel3[3] = {0, 0, 0};
    int bend = 0;
        
    //Wrist
    if(Serial3.available() > 0)
    {
      for(int i=0; i < 3; i++) 
      {
        accel1[i] = Serial3.parseInt();
      }
      bend = Serial3.parseInt();//bend sensor
      flushIncomingSerial3();
    }
    
    delay(7); 
    
    //Elbow 
    if(Serial2.available() > 0)
    {
      for(int i=0; i < 3; i++) 
      {
        accel2[i] = Serial2.parseInt();
      }
      flushIncomingSerial2();
    }

    delay(7); 
    
    //Shoulder
    if(Serial1.available() > 0)
    {
      for(int i=0; i < 3; i++) 
      {
        accel3[i] = Serial1.parseInt();
      }
      flushIncomingSerial1();
    }

    delay(7); 

    tempStr = String((int)accel1[0]) + String(',') + 
              String((int)accel1[1]) + String(',') + 
              String((int)accel1[2]) + String(',') +
              String((int)accel2[0]) + String(',') + 
              String((int)accel2[1]) + String(',') + 
              String((int)accel2[2]) + String(',') +
              String((int)accel3[0]) + String(',') + 
              String((int)accel3[1]) + String(',') + 
              String((int)accel3[2]) + String(',') +
              String((int)bend) + String('.') ;

    tempStr.toCharArray(data, 100);
    
    softSerial.println(data);
    softSerial.flush();
    
    Serial.println(data);
  }
  
  Serial.flush(); 
}


