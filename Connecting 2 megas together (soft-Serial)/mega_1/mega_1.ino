#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>



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

  Serial2.begin(9600); 
  
  Serial3.begin(19200); 
  
  //bluetooth.begin(4800);
   
 //bluetooth.begin(9600);
  
 //bluetooth.begin(115200);
} 


 
void loop() {
//char str1[128] = {'1'} ;
//char str2[128];
//char str3[128];
 curMega = Serial.read();
 
 if (curMega != prevMega) {
  
  Serial1.print(goFlag); 
  Serial2.print(goFlag);
  Serial3.print(goFlag);
  //Serial.println(goFlag);

  int accel1[3];
  int accel2[3];
  int accel3[3];
  
  delay(33);
  
  if(Serial1.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel1[i] = Serial1.parseInt();
      //Serial.println(accel1[i]);
    }
    
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
  tempStr = String((int)accel1[1]) + String(',') + 
            String((int)accel1[2]) + String(',') + 
            String((int)accel1[3]) + String(',') +
           String((int)accel2[1]) + String(',') + 
            String((int)accel2[2]) + String(',') + 
            String((int)accel2[3]) + String(',') +
           String((int)accel3[1]) + String(',') + 
            String((int)accel3[2]) + String(',') + 
            String((int)accel3[3]) + String(',') ;  
  //}          
  tempStr.toCharArray(data, 100);
    
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
