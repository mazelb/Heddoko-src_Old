//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Simple code for the ADXL335, prints calculated orientation via serial
//////////////////////////////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_LSM303.h>

Adafruit_LSM303 lsm;

//The minimum and maximum values that came from
//the accelerometer while standing still
//You very well may need to change these
int minVal = -1100;
int maxVal = 1000;


//to hold the caculated values
double x;
double y;
double z;


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
  
  //read the analog values from the accelerometer
  int xRead = (int)lsm.accelData.x;
  int yRead = (int)lsm.accelData.y;
  int zRead = (int)lsm.accelData.z;

  //convert read values to degrees -90 to 90 - Needed for atan2
  int xAng = map(xRead, minVal, maxVal, -90, 90);
  int yAng = map(yRead, minVal, maxVal, -90, 90);
  int zAng = map(zRead, minVal, maxVal, -90, 90);

  //Caculate 360deg values like so: atan2(-yAng, -zAng)
  //atan2 outputs the value of -π to π (radians)
  //We are then converting the radians to degrees
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  //Output the caculations
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.print(y);
  Serial.print(" | z: ");
  Serial.println(z);

  delay(100);//just here to slow down the serial output - Easier to read
}
