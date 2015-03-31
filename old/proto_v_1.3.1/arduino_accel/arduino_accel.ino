#include <Wire.h> 
#include <String.h>

#include <Adafruit_Sensor.h>  
#include <Adafruit_LSM303_U.h> 
#include <Adafruit_Simple_AHRS.h>

char   data[30];
String tempStr;
int    mega;
int    curMega = 0;
int    prevMega = 0;
int    ledPin = 13;

//Accelerometer 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified();
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified();
Adafruit_Simple_AHRS ahrs(&accel, &mag);

void setup()
{ 
  Serial.begin(9600); 
  
  // Try to initialise accel.  
  // and warn if we couldn't detect the chip 
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
   /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
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
    //tempStr = String((int)31) + String(',') + String((int)32) + String(',') + String((int)33) + String('.');
    if (ahrs.getOrientation(&orientation)) 
    {
      tempStr = String(int(orientation.roll)) + String(',') + String(int(orientation.pitch)) + String(',') + String(int(orientation.heading)) + String('.');
      tempStr.toCharArray(data, 29);
    }
    
    Serial.println(data);
  }

  Serial.flush();
} 
  
