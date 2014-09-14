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
  int flexSensor2Reading = analogRead(flexSensorPin2); 
  //int flexSensor2Reading = analogRead(flexSensorPin1);  
  lsm.read(); 
 
 
  //int flex0to100_sensor0 = map(flexSensorReading, 807, 837, 0, 100); 
  //int flex0to100_sensor1 = map(flexSensor1Reading, 570, 625, 0, 100); 
  int flex0to100_sensor2 = map(flexSensor2Reading, 787, 920, 0, 100); 
 
 
  //Analog readings
  //no constrains 
  //Serial.println(flex0to100_sensor0); 
 
  //with constrains
  //Serial.println(constrain(flex0to100_sensor0, 0, 100)); 
 
  //pin reading
  //Serial.println(flexSensorReading); 
 
  // Graph results
  numOfBars = flex0to100_sensor2;// / 10;
  //numOfBars = flex0to100;
  while (numOfBars >0)
  {
    Serial.print("o");
    numOfBars--;
  }
  Serial.print(": Serial: ");
  Serial.print(flexSensor2Reading);
  Serial.print(" Perc: ");
  Serial.println(flex0to100_sensor2); 
 
    
  //Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" "); 
  //Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" "); 
  //Serial.print("Z: "); Serial.println((int)lsm.accelData.z);     Serial.print(" "); 
  //Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" "); 
  //Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" "); 
  //Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" "); 
 
 
  delay(30); //just here to slow down the output for easier reading 
} 
 

