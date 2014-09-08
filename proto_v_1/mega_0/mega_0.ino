/*
THIS IS THE MASTER MEGA
*/

#include <Wire.h> 

#include <SoftwareSerial.h> 
#include <String.h>
#include <AltSoftSerial.h>



//int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
//int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3





char inChar;
int index = 0;
int goFlag = 0;
int i;

//SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);







 
void setup(){ 
  Serial.begin(9600); 
  
  Serial1.begin(9600); 

  Serial2.begin(9600); 
  
  Serial3.begin(9600); 
  
 
  
  


   
 //bluetooth.begin(9600);
  
 //bluetooth.begin(115200);
} 


 
void loop() {
//char str1[128] = {'1'} ;
//char str2[128];
//char str3[128];

 
  goFlag = !goFlag;
  Serial1.print(goFlag); 
  Serial2.print(goFlag);
  Serial3.print(goFlag);
  
  
  
  int mega1[10];
  int mega2[10];
  int mega3[9];
  
  delay(33);
  
  if(Serial1.available()>0)
  {
    for(int i=0; i < 10; i++) {
      mega1[i] = Serial1.parseInt();
      Serial.println(mega1[i]);
    
      
    if (i==2) {Serial.println("~~LEFT ARM ACCEL 1~~");}
    if (i==5) {Serial.println("~~LEFT ARM ACCEL 2~~");}
    if (i==8) {Serial.println("~~LEFT ARM ACCEL 3~~");}
    if (i==9) {Serial.println("~~LEFT ARM BEND~~");}
    
    }
  
  }
  


  if(Serial2.available())
  {
    for(int i=0; i < 10; i++) {
      mega2[i] = Serial2.parseInt();
      Serial.println(mega2[i]);
    
      
    if (i==2) {Serial.println("~~RIGHT ARM ACCEL 1~~");}
    if (i==5) {Serial.println("~~RIGHT ARM ACCEL 2~~");}
    if (i==8) {Serial.println("~~RIGHT ARM ACCEL 3~~");}
    if (i==9) {Serial.println("~~RIGHT ARM BEND~~");}
    
    }
  
  }
  
  
  
  if(Serial3.available())
  {
    for(int i=0; i < 9; i++) {
      mega3[i] = Serial3.parseInt();
      Serial.println(mega3[i]);
    
      
    if (i==2) {Serial.println("~~TORSO IMU 1~~");}
    if (i==5) {Serial.println("~~TORSO IMU 2~~");}
    if (i==8) {Serial.println("~~TORSO IMU 3~~");}
    
    
    }
  
  }
  
  
  
  
  //Serial.println("1");
  //Serial.println(str1);
  //Serial.println("2");
  //Serial.println(str2);
  //Serial.println("3");
  //Serial.println(str3);
  delay(460);

}
