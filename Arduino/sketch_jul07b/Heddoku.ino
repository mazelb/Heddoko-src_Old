#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

//Declaration
//For bend sensor
/*Note:  If you want to change the senser, the only value need to change is MinSensor and MaxSensor.  
 If you want to test 3 sensors at once, you use value 1 and 3.  
 If you want to test sensor 1 only, you use 1 and 1.  
 If you want to test sensor 2 only, you use 2 and 2.  
 If you want to test sensor 3 only, you use 3 and 3.
 */

#define MinSensor 1  //number of sensor at start.  usually equal to 1
#define MaxSensor 3  //number of sensors used

//Flex sensors 
int flexSensorPin[3] = {
  A0, A1, A2};  //analog pin 0, 1, 2
int flexSensorReading[MaxSensor];
int flex0to100_sensor[MaxSensor];
int flexStep[MaxSensor];

//Accelerometer 
//Adafruit_LSM303 lsm; 

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

/*
//Function
//For Bend Sensor
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
*/

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); 
  Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); 
  Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); 
  Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); 
  Serial.print(sensor.max_value); 
  Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); 
  Serial.print(sensor.min_value); 
  Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); 
  Serial.print(sensor.resolution); 
  Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); 
  Serial.println("");

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();
}

//Main function
void loop(void) 
{
  //For Bend Sensor
  int i;
  for (i = MinSensor ; i <= MaxSensor ; i = i + 1) {
    flexSensorReading[i] = analogRead(flexSensorPin[i-1]);
  }  
  //lsm.read(); 

  //Calibration
  flex0to100_sensor[1] = map(flexSensorReading[1], 773, 891, 0, 100);        // Mapping Raw data from 0 to 100 for A0
  flex0to100_sensor[2] = map(flexSensorReading[2], 555, 724, 0, 100);        // Mapping Raw data from 0 to 100 for A1
  flex0to100_sensor[3] = map(flexSensorReading[3], 820, 936, 0, 100);        // Mapping Raw data from 0 to 100 for A2

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

  //For Accelerator
  /* Get a new sensor event */
  sensors_event_t event; 
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); 
  Serial.print(event.acceleration.x); 
  Serial.print("  ");
  Serial.print("Y: "); 
  Serial.print(event.acceleration.y); 
  Serial.print("  ");
  Serial.print("Z: "); 
  Serial.print(event.acceleration.z); 
  Serial.print("  ");
  Serial.println("m/s^2 ");

  delay(500);  //just here to slow down the output for easier reading 
}

