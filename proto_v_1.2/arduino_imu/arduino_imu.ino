#include <Wire.h> 
#include <SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Sensor.h>  
#include <String.h>


char   data[30];
String tempStr;
int    mega;
int    curMega = 0;
int    prevMega = 0;
int    ledPin = 13;

//Accelerometer 
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();
 
void setup()
{ 
  Serial.begin(9600); 
  
  // Try to initialise accel.  
  // and warn if we couldn't detect the chip 
  if (!lsm.begin()) 
  { 
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!"); 
    while (1); 
  }   
  
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
} 
 
 
void loop()
{
  digitalWrite(ledPin, LOW);
  
  curMega = 0;   
  curMega = Serial.read();  
    
  if (curMega != prevMega) 
  {
    digitalWrite(ledPin, HIGH);

    prevMega = curMega;
    
    lsm.read();
    
    tempStr = String((int)lsm.accelData.x) + String(',') + String((int)lsm.accelData.y) + String(',') + String((int)lsm.accelData.z) + String('.');
    //tempStr = String((int)141) + String(',') + String((int)142) + String(',') + String((int)143) + String('.');
    
    tempStr.toCharArray(data, 29);
    
    Serial.println(data);
  }

  Serial.flush();
} 
  
