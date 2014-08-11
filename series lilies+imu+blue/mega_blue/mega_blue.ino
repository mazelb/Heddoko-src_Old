#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

char str1[128];
char str2[20];
String data;

int i;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
 
//Accelerometer 
Adafruit_LSM303 lsm; 
 
void setup(){ 
  Serial.begin(9600); 
  
  Serial1.begin(19200); 

  //Serial2.begin(19200); 
  
//  Serial3.begin(19200); 
  
  
  
  bluetooth.begin(9600);
  
 //bluetooth.begin(115200);
} 


 
void loop() {
  
  Serial1.print(1); 

//data=NULL;    

int j=0;
int k=0;

if (Serial1.available()) {
  //delay(100); //allows all serial sent to be received together
  
  while(Serial1.available()) {
  //while(j < 64) {
      
  str1[j] = Serial1.read();

  j++;  
  }
    
}
//Serial1.print(0);
/*
if (Serial2.available()) {
  delay(100); //allows all serial sent to be received together
  
  while(Serial2.available()) {
      
  str2[k] = Serial2.read();

  k++;  
  }
    
}
*/


//data=data+str1+str2;
//Serial.println(j);
//Serial.print(data);
Serial.println(str1);
//Serial.print('\n');
//Serial.print(str2);

delay(500);

}
 
