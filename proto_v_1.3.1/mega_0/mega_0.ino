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
  
  if(Serial1.available()>0)
  {  
    for(int i=0; i < 10; i++) 
    {
      mega1[i] = Serial1.parseInt();
      Serial.println(mega1[i]);
      
      if (i==2) {Serial.println("~~LEFT ARM ACCEL 1~~");}
      if (i==5) {Serial.println("~~LEFT ARM ACCEL 2~~");}
      if (i==8) {Serial.println("~~LEFT ARM ACCEL 3~~");}
      if (i==9) {Serial.println("~~LEFT ARM BEND~~");}
    }
    
    flushIncomingSerial1();
  }

  delay(7); 
  
  if(Serial2.available())
  {
    for(int i=0; i < 10; i++) 
    {
      mega2[i] = Serial2.parseInt();
      Serial.println(mega2[i]);      
      
      if (i==2) {Serial.println("~~RIGHT ARM ACCEL 1~~");}
      if (i==5) {Serial.println("~~RIGHT ARM ACCEL 2~~");}
      if (i==8) {Serial.println("~~RIGHT ARM ACCEL 3~~");}
      if (i==9) {Serial.println("~~RIGHT ARM BEND~~");}      
    }
    
    flushIncomingSerial2();
  }
  
  delay(7);   
  
  if(Serial3.available())
  {
    for(int i=0; i < 9; i++) 
    {
      mega3[i] = Serial3.parseInt();
      Serial.println(mega3[i]);
           
      if (i==2) {Serial.println("~~TORSO IMU 1~~");}
      if (i==5) {Serial.println("~~TORSO IMU 2~~");}
      if (i==8) {Serial.println("~~TORSO IMU 3~~");}        
    }  

    flushIncomingSerial3();
  }
  
  delay(500);
}
