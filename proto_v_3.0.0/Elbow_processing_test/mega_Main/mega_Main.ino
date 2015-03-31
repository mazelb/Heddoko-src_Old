/*
THIS IS THE MASTER MEGA
*/

#include <Wire.h> 

#include <SoftwareSerial.h> 
#include <String.h>
#include <AltSoftSerial.h>


char  inChar;
int   index = 0;
int   goFlag = 0;
int   i;
int   ledPin = 13;

//SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

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
  
  Serial1.begin(9600); 

  Serial2.begin(9600); 
  
  Serial3.begin(9600); 
   
 //bluetooth.begin(9600);
  
 //bluetooth.begin(115200);
} 


 
void loop() 
{
  digitalWrite(ledPin, LOW);
  Serial.println("~~~~~~~LOOP START~~");
 
  goFlag = !goFlag;
  Serial1.print(goFlag);
  Serial1.flush(); 
  
  Serial2.print(goFlag);
  Serial2.flush();
  
  Serial3.print(goFlag);
  Serial3.flush();
  
  int mega1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int mega2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int mega3[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  //delay(30);
  digitalWrite(ledPin, HIGH);
    
  //RIGHT ARM
  if(Serial1.available()>0)
  {  
    for(int i=0; i < 10; i++) 
    {
      mega1[i] = Serial1.parseInt();

      if (i==0) {Serial.println("~~RIGHT ARM IMU 1~~");}
      if (i==3) {Serial.println("~~RIGHT ARM IMU 2~~");}
      if (i==6) {Serial.println("~~RIGHT ARM IMU 3~~");}
      if (i==9) {Serial.println("~~RIGHT ARM BEND~~");}
      Serial.flush(); 

      Serial.println(mega1[i]);
    }
    
    flushIncomingSerial1();
  }
  
  delay(10); 
  
  //LEFT ARM
  if(Serial3.available())
  {
    for(int i=0; i < 10; i++) 
    {
      mega2[i] = Serial3.parseInt();
           
      if (i==0) {Serial.println("~~LEFT ARM IMU 1~~");}
      if (i==3) {Serial.println("~~LEFT ARM IMU 2~~");}
      if (i==6) {Serial.println("~~LEFT ARM IMU 3~~");}
      if (i==9) {Serial.println("~~LEFT ARM BEND~~");}            
      Serial.flush(); 

      Serial.println(mega2[i]);
    }  

    flushIncomingSerial3();
  }
  
   delay(10);   
  
  //TORSO 
  //Serial.println("~~TORSO~~");
  if(Serial2.available())
  {
    //Serial.println("~~TORSO AVAILABLE~~");
    for(int i=0; i < 9; i++) 
    {
      mega3[i] = Serial2.parseInt();

      if (i==0) {Serial.println("~~TORSO IMU 1~~");}
      if (i==3) {Serial.println("~~TORSO IMU 2~~");}
      if (i==6) {Serial.println("~~TORSO IMU 3~~");}
      Serial.flush(); 

      Serial.println(mega3[i]);      
    }
    
    Serial.println("!");
    flushIncomingSerial2();
  }
  
  delay(60);
}
