#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

char str1[128];
char str2[128];
String data;

int goFlag = 0;

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
 
  goFlag = !goFlag;
  Serial1.print(goFlag); 
  Serial2.print(goFlag);
  
  delay(40);
    
  int j=0;
  int k=0;
  
  if (Serial1.available()) {

    while(Serial1.available()) {        
      str1[j] = Serial1.read();
      j++;  
    }
  }
  
  
  if (Serial2.available()) {
    
    while(Serial2.available()) {
        
      str2[k] = Serial2.read(); 
      k++;  
    }
  }
  
  
  Serial.println(str1);
  Serial.println(str2);
  delay(460);

}
 
