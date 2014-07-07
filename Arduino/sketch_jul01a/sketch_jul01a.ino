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
  //int flexSensorReading = analogRead(flexSensorPin);  
  //int flexSensor1Reading = analogRead(flexSensorPin1);  
  //int flexSensor2Reading = analogRead(flexSensorPin2); 
  int flexSensor2Reading = analogRead(flexSensorPin1);  
  //lsm.read(); 
 
 
  //In my tests I was getting a reading on the arduino between 512, and 614.  
  //Using map(), you can convert that to a larger range like 0-100. 
  //int flex0to100_sensor0 = map(flexSensorReading, 807, 837, 0, 100); 
  //int flex0to100_sensor1 = map(flexSensor1Reading, 570, 625, 0, 100); 
  int flex0to100_sensor2 = map(flexSensor2Reading, 787, 920, 0, 100); 
 
 
 //A0
 //no constrains 
  //Serial.println(flex0to100_sensor0); 
 
 //with constrains
 //Serial.println(constrain(flex0to100_sensor0, 0, 100)); 
 
 //pin reading
 //Serial.println(flexSensorReading); 
 
 
 //A1
 //no constrains 
 //Serial.println(flex0to100_sensor1); 
 
 //with constrains
 //Serial.println(constrain(flex0to100_sensor1, 0, 100)); 
 
 //pin reading
 
 // Graph results
  numOfBars = flex0to100_sensor2;// / 10;
  //numOfBars = flex0to100;
  while (numOfBars >0)
  {
    Serial.print("o");
    numOfBars--;
  }
  Serial.print(": ");
  //Serial.println(flexSensorReading);
  
  Serial.println(flex0to100_sensor2); 
 
 
 //A2
 //no constrains 
 //Serial.println(flex0to100_sensor2); 
 
 //with constrains
 //Serial.println(constrain(flex0to100_sensor2, 0, 100)); 
 
 //pin reading
 //Serial.println(flexSensor2Reading); 
   
  //Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" "); 
  //Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" "); 
  //Serial.print("Z: "); Serial.println((int)lsm.accelData.z);     Serial.print(" "); 
  //Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" "); 
  //Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" "); 
  //Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" "); 
 
 
  delay(30); //just here to slow down the output for easier reading 
} 
 


