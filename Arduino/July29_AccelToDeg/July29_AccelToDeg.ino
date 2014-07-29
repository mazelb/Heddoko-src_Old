#include <Wire.h>
#include <Adafruit_LSM303.h>

Adafruit_LSM303 lsm;

void setup() 
{
  Serial.begin(9600);
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }
}

void loop() 
{
  lsm.read();
  
  // Accel data
  int ax = lsm.accelData.x;
  int ay = lsm.accelData.y;
  int az = lsm.accelData.z;
  
  // Convert acceleration to angles
  int x = atan( ax / sqrt( ay*ay + az*az ) ) * 180 / 3.14;
  int y = atan( ay / sqrt( ax*ax + az*az ) ) * 180 / 3.14;
  int z = atan( sqrt( ax*ax + ay*ay ) / az ) * 180 / 3.14;
  
  // Print data
  Serial.print("Pitch: "); Serial.print(x); Serial.print(" ");
  Serial.print("Roll: "); Serial.print(y); Serial.print(" ");
  Serial.print("Yaw: "); Serial.print(z); Serial.print(" ");
  Serial.println("");
  
  delay(300);
}
