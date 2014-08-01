#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
#include <String.h>

#define noOfSensor 1  
 //number of sensors used

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

char str[25];
String data;

int i;


SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

//Flex sensors 
int flexSensorPin[noOfSensor] = {A0};  //analog pin 0
int flexSensorReading[noOfSensor];
int flex0to100_sensor[noOfSensor];

 
//Accelerometer 
//Adafruit_LSM303 lsm; 
 
void setup(){ 
  Serial.begin(9600); 
  
  
  bluetooth.begin(9600);
  
 //bluetooth.begin(115200);

} 
 
 
void loop() {

 
  
flexSensorReading[0] = analogRead(flexSensorPin[0]);  
flex0to100_sensor[0] = map(flexSensorReading[0], 0, 968, 0, 100);
    
  
data=data+flex0to100_sensor[0];  
  
int j=0;

if (Serial.available()) {
  delay(100); //allows all serial sent to be received together
  
  while(Serial.available()) {
      
  str[j] = Serial.read();

  j++;  
  }
    


//Serial.print(str);
//Serial.print(flex0to100_sensor[0]);
//Serial.print('\n');
data =(String)(flex0to100_sensor[0])+'.';
bluetooth.print(data);
bluetooth.print(str);

  
}

//delay(100);
//Serial.print(data);
//Serial.print('\n');

delay(300);

}
 
