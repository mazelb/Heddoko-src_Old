#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <String.h>


String data;
int mega;
int curMega = 0;
int prevMega = 0;
 
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
  
  curMega = Serial.read();
  
  if (curMega != prevMega) {
  
    lsm.read();
    
    data=data+(int)lsm.accelData.x+','+(int)lsm.accelData.y+','+(int)lsm.accelData.z+'.';
    
    Serial.println(data);
    //Serial.print("\n");
    
    prevMega = curMega;
  }
} 


 

