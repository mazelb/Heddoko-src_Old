#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <String.h>


char   data[30];
String tempStr;
int    mega;
int    curMega = 0;
int    prevMega = 0;
int    flexSensorReading;
int    flex0to100_sensor;
int    ledPin = 13;
 
//Accelerometer 
Adafruit_LSM303 lsm; 
 
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
} 
 
 
void loop()
{ 
  digitalWrite(ledPin, LOW);
  curMega = 0;   
  curMega = Serial.read();
  
  if (curMega != prevMega ) 
  {
    digitalWrite(ledPin, HIGH);
    prevMega = curMega;

    flexSensorReading = analogRead(A0);  
    flex0to100_sensor = map(flexSensorReading, 0, 968, 0, 100);    
    
    lsm.read();
    
    tempStr = String((int)lsm.accelData.x) + String(',') + String((int)lsm.accelData.y) + String(',') + String((int)lsm.accelData.z) + String(',') + String(int(flex0to100_sensor)) + String('.');
    //tempStr = String((int)71) + String(',') + String((int)72) + String(',') + String((int)73) + String(',') + String((int)74) + String('.');
    tempStr.toCharArray(data, 29);

    Serial.println(data);
  }

  Serial.flush();
} 
  
