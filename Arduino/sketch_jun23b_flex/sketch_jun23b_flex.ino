#include <Wire.h> 
#include <Adafruit_LSM303.h> 
 
//If you want to change the senser, the only value need to change is MinSensor and MaxSensor.  If you want to test 3 sensors at once, you use value 1 and 3.  If you want to test sensor 1 only, you use 1 and 1.  If you want to test sensor 2 only, you use 2 and 2.  If you want to test sensor 3 only, you use 3 and 3.
#define MinSensor 1  //number of sensor at start.  usually equal to 1
#define MaxSensor 3  //number of sensors used

//Flex sensors 
int flexSensorPin[3] = {A0, A1, A2};  //analog pin 0, 1, 2
int flexSensorReading[MaxSensor];
int flex0to100_sensor[MaxSensor];
int flexStep[MaxSensor];
 
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
  int i;
  for (i = MinSensor ; i <= MaxSensor ; i = i + 1) {
    flexSensorReading[i] = analogRead(flexSensorPin[i]);
  }  
  //lsm.read(); 
 
  for (i = MinSensor ; i <= MaxSensor ; i = i + 1) {
    //Serial.println(flexSensorReading[i]);                                     // Raw data before mapping
  } 
 
  for (i = MinSensor ; i <= MaxSensor ; i = i + 1) {
    //flex0to100_sensor[i] = map(flexSensorReading[i], 790, 873, 0, 100);        // Mapping Raw data from 0 to 100
    flex0to100_sensor[i] = map(flexSensorReading[i], 807, 895, 0, 100);        // Mapping Raw data from 0 to 100
    //flex0to100_sensor[i] = constrain(flexSensorReading[i], 0, 100);        // Mapping Raw data from 0 to 100
  }
  
  //Print out the value from 0 to 100 before the step  
  /*
  for (i = MinSensor ; i <= MaxSensor ; i = i + 1) {
    Serial.print("Sensor ");
    Serial.println(i);
    Serial.print(" scale is: ");
    Serial.print("\t");                                            // prints a tab
    Serial.println(flex0to100_sensor[i]);                          // prints out value from 0 to 100
    Serial.print(" % bend. ");
    Serial.println("");                                            // prints another carriage return
    //Serial.println(constrain(flex0to100_sensor[i], 0, 100)); 
  }
  */
 
   for (i = MinSensor ; i <= MaxSensor ; i = i + 1) {
    if (flex0to100_sensor[i] < 5) {
      flexStep[i] = 0;
    }
    else if (flex0to100_sensor[i] < 15) {
      flexStep[i] = 10;
    }
    else if (flex0to100_sensor[i] < 25) {
      flexStep[i] = 20;
    }
    else if (flex0to100_sensor[i] < 35) {
      flexStep[i] = 30;
    }
    else if (flex0to100_sensor[i] < 45) {
      flexStep[i] = 40;
    }
    else if (flex0to100_sensor[i] < 55) {
      flexStep[i] = 50;
    }
    else if (flex0to100_sensor[i] < 65) {
      flexStep[i] = 60;
    }
    else if (flex0to100_sensor[i] < 75) {
      flexStep[i] = 70;
    }
    else if (flex0to100_sensor[i] < 85) {
      flexStep[i] = 80;
    }
    else if (flex0to100_sensor[i] < 95) {
      flexStep[i] = 90;
    }
    else if (flex0to100_sensor[i] <= 100) {
      flexStep[i] = 100;
    }
  } 
  
   for (i = MinSensor ; i <= MaxSensor ; i = i + 1) {
    Serial.print("Sensor ");
    Serial.println(i);
    Serial.print(" step is: ");
    Serial.print("\t");                                            // prints a tab
    Serial.print("\t");                                            // prints a tab
    Serial.print("\t");                                            // prints a tab
    Serial.println(flexStep[i]);                                    // prints out value from 0 to 100
    Serial.print(" % bend. ");
    Serial.println("");                                            // prints another carriage return
  }
 
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
 //Serial.println(flexSensor1Reading); 
 
 
 //A2
 //no constrains 
 //Serial.println(flex0to100_sensor2); 
 
 //with constrains
 
  
 //Serial.println(constra(flex0to100_sensor2, 0, 100)); 
 
 //pin reading
 //Serial.println(flexSensor2Reading); 
   
  //Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" "); 
  //Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" "); 
  //Serial.print("Z: "); Serial.println((int)lsm.accelData.z);     Serial.print(" "); 
  //Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" "); 
  //Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" "); 
  //Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" "); 
 
 
  delay(500); //just here to slow down the output for easier reading 
} 
 

