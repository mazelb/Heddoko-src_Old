#include <Wire.h> 
#include <Adafruit_LSM303.h> 
 
 
 
 
//Flex sensors 
int flexSensorPin = A0; //analog pin 0 
int flexSensorPin1 = A1; //analog pin 1 
int flexSensorPin2 = A2; //analog pin 2 
int numOfBars = 0; 
 
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
  int flexSensor2Reading = analogRead(flexSensorPin2);  
  
  //read from the accelerometer
  //lsm.read(); 
 
  
  //map flex sensor readings to percentage of bend 
  int flex0to100_sensor2 = map(flexSensor2Reading, 811, 950, 0, 100); 
 
 
  // Graph results
  //numOfBars = flex0to100_sensor2;// / 10;
  //numOfBars = flex0to100;
  
  //draw wave form with zeros
  //while (numOfBars >0)
  //{
    //Serial.print("o");
    //numOfBars--;
  //}

  //Send the flex sensor data to the serial port  
  //Serial.println(flexSensor2Reading);  
  //Serial.println(flex0to100_sensor2); 
  Serial.println(constrain(flex0to100_sensor2, 0, 100)); 
 
  //Get the data from the accelerometer 
  //Serial.println((int)lsm.accelData.x);
  //Serial.println((int)lsm.accelData.y);
  //Serial.println((int)lsm.accelData.z);
  //Serial.println((int)lsm.magData.x);
  //Serial.println((int)lsm.magData.y);
  //Serial.println((int)lsm.magData.z); 
 
  delay(30); //just here to slow down the output for easier reading 
} 
 


