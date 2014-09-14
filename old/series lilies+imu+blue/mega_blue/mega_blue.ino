#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3


char inChar;
int index = 0;
int goFlag = 0;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
 
//Accelerometer 
Adafruit_LSM303 lsm; 
 
void setup(){ 
  Serial.begin(9600); 
  
  Serial1.begin(19200); 

  Serial2.begin(9600); 
  
  Serial3.begin(19200); 
   
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
  Serial.println("GOs");

  int accel1[3];
  int accel2[3];
  int accel3[3];
  
  delay(33);
  
  if(Serial1.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel1[i] = Serial1.parseInt();
      Serial.println(accel1[i]);
    }
    
    Serial.println("1-------");
  }

  if(Serial2.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel2[i] = Serial2.parseInt();
      Serial.println(accel2[i]);
    }
    
    Serial.println("2-------");
  }

  if(Serial3.available() > 0)
  {
    for(int i=0; i < 3; i++) {
      accel3[i] = Serial2.parseInt();
      Serial.println(accel3[i]);
    }
    
    Serial.println("3-------");
  }
  
  //Serial.println("1");
  //Serial.println(str1);
  //Serial.println("2");
  //Serial.println(str2);
  //Serial.println("3");
  //Serial.println(str3);
  //delay(460);

}
