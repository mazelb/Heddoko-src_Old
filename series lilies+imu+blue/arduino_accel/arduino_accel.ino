#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <String.h>


String data;


 
//Accelerometer 
Adafruit_LSM303 lsm; 
 
void setup(){ 
  Serial.begin(9600); 
  
  // Try to initialise accel.  
  // and warn if we couldn't detect the chip 
  if (!lsm.begin()) 
  { 
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!"); 
    while (1); 
  } 
} 
 
 
void loop(){ 
  
data=NULL;    

if (Serial.read()){
lsm.read();
//delay(50);

data=data+(int)lsm.accelData.x+','+(int)lsm.accelData.y+','+(int)lsm.accelData.z+'.';

Serial.print(data);
Serial.print("\n");

}  


 
//  delay(400); //just here to slow down the output for easier reading 
} 


 

