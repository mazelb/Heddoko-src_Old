#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3


char inChar;
String data;
int index = 0;
int counter=0;
int availablebytes=0;
byte discard; 
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
char str2[128];
char str3[128];

 
  goFlag = !goFlag;
  Serial1.print(goFlag); 
  Serial2.print(goFlag);
  Serial3.print(goFlag);
  Serial.println("GOs");
  
  delay(33);
  
  index = 0;
  //while (Serial1.available() > 0)// && index < 30 ) // Don't read unless there you know there is data
  {
      //Serial.println("step1");
      //Serial.println(index);
      //if(index < 10) // One less than the size of the array
      //{
          //Serial.println("step2");
          //inChar = Serial1.read(); // Read a character
          //Serial.println("step3");        
          //str1[index] = Serial1.read();//inChar; // Store it
          for(int i=0; i < 3; i++) {
            char str1[40];
            int len = Serial1.readBytesUntil(',', str1, 30);
            Serial.println(str1);
          }
//          if(str1[index] == ',') {
//            str1[index] = '\0'; // Null terminate the string
//            
//            index = 0;
//            //Serial1.flush();
//            while(Serial1.available() > 0  && index < 30) {discard = Serial.read(); index++;}
//            
//            Serial.println(str1);
//            Serial.flush();
//            break;
//          }
//          
//          index++; // Increment where to write next
//          //Serial.println("----");        
//      }
//      else
//      {
//        Serial.println("step3");
//        Serial1.flush();
//      }
  }
  
//  str1[index] = '\0'; // Null terminate the string
//  Serial.print("1: ");
//  Serial.println(str1);
  //Serial.println("");
  //Serial.();
  index = 0;
  
//  while (Serial2.available() > 0) // Don't read unless there you know there is data
//  {
//      if(index < 10) // One less than the size of the array
//      {
//          inChar = Serial2.read(); // Read a character
//          str2[index] = inChar; // Store it
//          index++; // Increment where to write next
//          str2[index] = '\0'; // Null terminate the string
//      }
//      else
//      {
//        Serial2.flush();
//      }
//  }
//  
//  Serial.print("2: ");
//  Serial.print(str2);
//
//  Serial.println("");
// 
//  
//  index = 0;
//  
//  while (Serial3.available() > 0) // Don't read unless there you know there is data
//  {
//      if(index < 10) // One less than the size of the array
//      {
//          inChar = Serial3.read(); // Read a character
//          str3[index] = inChar; // Store it
//          index++; // Increment where to write next
//          str3[index] = '\0'; // Null terminate the string
//      }
//      else
//      {
//        Serial3.flush();
//      }
//  }
//  Serial.print("3: ");
//  Serial.print(str3);
//  Serial.println("");
//
//  index = 0;
  
  //Serial.println("1");
  //Serial.println(str1);
  //Serial.println("2");
  //Serial.println(str2);
  //Serial.println("3");
  //Serial.println(str3);
  delay(460);

}
 
