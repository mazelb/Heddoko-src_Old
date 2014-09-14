#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <String.h>


#define noOfSensor 2  //number of sensors used

int i;
String data;
//Flex sensors 
int flexSensorPin[noOfSensor] = {A0, A1};  //analog pin 0, 1 // array element 0,1 //no. of sensor = no. of array element
int flexSensorReading[noOfSensor];
int flex0to100_sensor[noOfSensor];

 
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
  
data=NULL;    

flexSensorReading[0] = analogRead(flexSensorPin[0]);
flexSensorReading[1] = analogRead(flexSensorPin[1]);

flex0to100_sensor[0] = map(flexSensorReading[0], 0, 968, 0, 100);        // Mapping Raw data from 0 to 100
flex0to100_sensor[1] = map(flexSensorReading[1], 0, 968, 0, 100);        // Mapping Raw data from 0 to 100

for (i = 0 ; i < noOfSensnor ; i = i + 1) {  // array index from 0 to "noOfSensor-1"  => "2-1"
   
  data=data+flex0to100_sensor[i]+',';       // prints out value from 0 to 100

}
  
  
lsm.read();
delay(50);

data=data+(int)lsm.accelData.x+','+(int)lsm.accelData.y+','+(int)lsm.accelData.z+'.';

Serial.print(data);
Serial.print("\n");

 
  delay(500); //just here to slow down the output for easier reading 
} 


 

