/*
THIS IS THE MASTER MEGA
*/

#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

//int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
//int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

int softTx = 11;  
int softRx = 10;  



char inChar;
int index = 0;
int goFlag = 0;
int i;

//SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

SoftwareSerial softSerial(softRx, softTx);
 
//Accelerometer 
Adafruit_LSM303 lsm; 
 
void setup(){ 
  Serial.begin(9600); 
  
  Serial1.begin(19200); 

  Serial2.begin(19200); 
  
  Serial3.begin(19200); 
  
 softSerial.begin(9600);
   
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
  softSerial.print(goFlag);
  
  int accel1[3];
  int accel2[3];
  int accel3[3];  
  int bend;
  int mega[10];
  
  delay(33);
  
  if(Serial1.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel1[i] = Serial1.parseInt();
      Serial.println(accel1[i]);
    }
    
    Serial.println("~~LEFT ARM ACCEL 1~~");
  }

  if(Serial2.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel2[i] = Serial2.parseInt();
      Serial.println(accel2[i]);
    }
    
    Serial.println("~~LEFT ARM ACCEL 2~~");
  }

  if(Serial3.available() > 0)
  {
    for(int i=0; i < 3; i++) {   
      accel3[i] = Serial3.parseInt();
      Serial.println(accel3[i]);
  
}
    
    Serial.println("~~LEFT ARM ACCEL 3~~");

bend = Serial3.parseInt();//bend sensor
Serial.println(bend); //bend sensor
Serial.println("~~LEFT ARM BEND~~");


} 
    


  if(softSerial.available())
  {
    for(int i=0; i < 10; i++) {
      mega[i] = softSerial.parseInt();
      Serial.println(mega[i]);
    
      
    if (i==2) {Serial.println("~~RIGHT ARM ACCEL 1~~");}
    if (i==5) {Serial.println("~~RIGHT ARM ACCEL 2~~");}
    if (i==8) {Serial.println("~~RIGHT ARM ACCEL 3~~");}
    if (i==9) {Serial.println("~~RIGHT ARM BEND~~");}
    
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
