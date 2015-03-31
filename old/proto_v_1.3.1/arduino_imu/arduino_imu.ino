#include <SPI.h>
#include <Wire.h> 
#include <String.h>
#include <Adafruit_Sensor.h>  
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Simple_AHRS.h>


char   data[30] = {0};
String tempStr;
int    curMega = 0;
int    prevMega = 0;
int    ledPin = 13;

//Accelerometer 
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();
Adafruit_Simple_AHRS ahrs(&lsm.getAccel(), &lsm.getMag());


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
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);
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
    
    sensors_vec_t   orientation;
    
    //tempStr = String((int)lsm.accelData.x) + String(',') + String((int)lsm.accelData.y) + String(',') + String((int)lsm.accelData.z) + String('.');
    //tempStr = tempStr + String((int)lsm.magData.x) + String(',') + String((int)lsm.magData.y) + String(',') + String((int)lsm.magData.z) + String('.');
    //tempStr = String((int)141) + String(',') + String((int)142) + String(',') + String((int)143) + String('.');
    if (ahrs.getOrientation(&orientation)) 
    {
      tempStr = String(int(orientation.roll)) + String(',') + String(int(orientation.pitch)) + String(',') + String(int(orientation.heading)) + String('.');
      tempStr.toCharArray(data, 29);
    }

    Serial.println(data);
  }

  Serial.flush();
  //delay(100);
} 
  
