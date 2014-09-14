#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <SPI.h>
//#include <Adafruit_LSM9DS0.h>
//#include <Adafruit_Sensor.h>
#include <String.h>

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;



char data[30];
String tempStr;
int mega;
int curMega = 0;
int prevMega = 0;
 
//Accelerometer 
//Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();
 
void setup(){
  Wire.begin();
  Serial.begin(9600); 
  
  accelgyro.initialize();
  
  // Try to initialise accel.  
  // and warn if we couldn't detect the chip 
 /* if (!lsm.begin()) 
  { 
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!"); 
    while (1); 
  } */
} 
 
 
void loop(){     
  
  curMega = Serial.read();
  
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
  
  
  if (curMega != prevMega) {
  
    //lsm.read();
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    
    tempStr = String((int)ax) + String(',') + String((int)ay) + String(',') + String((int)az) + String(',');
    tempStr.toCharArray(data, 29);
    
    //data = 'Accel' + String((int)lsm.accelData.x) + ',' + String((int)lsm.accelData.y) + ',' + String((int)lsm.accelData.z);
    
    //int len = strlen(data); 
    Serial.println(data);
    Serial.flush();
    
    prevMega = curMega;
  }
} 
  
