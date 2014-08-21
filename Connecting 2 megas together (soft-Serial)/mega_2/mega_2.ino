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

char data[100];
String tempStr;
int mega;
int curMega = 0;
int prevMega = 0;

char inChar;
int index = 0;
int goFlag = 0;

 
//Accelerometer 

 
void setup(){ 
  Serial.begin(9600); 
  
  Serial1.begin(19200); 

  Serial2.begin(19200); 
  
  Serial3.begin(19200); 
  
  
   softSerial.begin(9600);
  //bluetooth.begin(4800);
   
 //bluetooth.begin(9600);
  
 //bluetooth.begin(115200);
} 


 
void loop() {
//char str1[128] = {'1'} ;
//char str2[128];
//char str3[128];
 curMega = softSerial.read();
 
 if (curMega != prevMega) {
  
  Serial1.print(goFlag); 
  Serial2.print(goFlag);
  Serial3.print(goFlag);
  

  int accel1[3];
  int accel2[3];
  int accel3[3];
  int bend;
  
  delay(33);
  
  if(Serial1.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel1[i] = Serial1.parseInt();
      //Serial.println(accel1[i]);
    }
    
    bend = Serial1.parseInt();//bend sensor
    
    
    //Serial.println("right arm 1-------");
  }

  if(Serial2.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel2[i] = Serial2.parseInt();
      //Serial.println(accel2[i]);
    }
    
    //Serial.println("right arm 2-------");
  }

  if(Serial3.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel3[i] = Serial2.parseInt();
      //Serial.println(accel3[i]);
    }
    
    //Serial.println("right arm 3-------");
  }
  //for (int i=0; i<3; i++){
  tempStr =String((int)accel1[1]) + String(',') + 
            String((int)accel1[2]) + String(',') + 
            String((int)accel1[3]) + String(',') +
           String((int)accel2[1]) + String(',') + 
            String((int)accel2[2]) + String(',') + 
            String((int)accel2[3]) + String(',') +
           String((int)accel3[1]) + String(',') + 
            String((int)accel3[2]) + String(',') + 
            String((int)accel3[3]) + String(',') +
            String((int)bend) + String(',') ;  
  //}          
  tempStr.toCharArray(data, 100);
    
  softSerial.println(data);
  softSerial.flush();
  
  
  Serial.println(data);
  Serial.flush(); 
    
  prevMega = curMega;
  
  
  
 }
  //Serial.println("1");
  //Serial.println(str1);
  //Serial.println("2");
  //Serial.println(str2);
  //Serial.println("3");
  //Serial.println(str3);
 

}
