#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

char str1[128];
char str2[128];
char str3[128];

char inChar;
String data;
byte index = 0;

int goFlag = 0;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
 
//Accelerometer 
Adafruit_LSM303 lsm; 
 
void setup(){ 
  Serial.begin(9600); 
  
  Serial1.begin(19200); 

  Serial2.begin(9600); //LP6 works on 9600 
  
  Serial3.begin(19200); 
  
//  Serial3.begin(19200); 
  
  
  
  bluetooth.begin(9600);
  
 //bluetooth.begin(115200);
} 


 
void loop() {
 
  goFlag = !goFlag;
  
  
  Serial1.print(goFlag); 
  Serial2.print(goFlag);
  Serial3.print(goFlag);
  
  delay(40);
    
  
  
  while (Serial1.available() > 0) // Don't read unless there you know there is data
  {
      if(index < 30) // One less than the size of the array
      {
          inChar = Serial1.read(); // Read a character
          str1[index] = inChar; // Store it
          index++; // Increment where to write next
          str1[index] = '\0'; // Null terminate the string
      }
  }
  Serial.println("1");
  Serial.println(index);

  index = 0;
  
  while (Serial2.available() > 0) // Don't read unless there you know there is data
  {
      if(index < 30) // One less than the size of the array
      {
          inChar = Serial2.read(); // Read a character
          str2[index] = inChar; // Store it
          index++; // Increment where to write next
          str2[index] = '\0'; // Null terminate the string
      }
  }
  
  Serial.println("2");
  Serial.println(index);
  index = 0;
  
  while (Serial3.available() > 0) // Don't read unless there you know there is data
  {
      if(index < 30) // One less than the size of the array
      {
          inChar = Serial3.read(); // Read a character
          str3[index] = inChar; // Store it
          index++; // Increment where to write next
          str3[index] = '\0'; // Null terminate the string
      }
  }
  Serial.println("3");
  Serial.println(index);
  index = 0;
  
  Serial.println("1");
  Serial.println(str1);
  Serial.println("2");
  Serial.println(str2);
  Serial.println("3");
  Serial.println(str3);
  delay(460);

}
 
