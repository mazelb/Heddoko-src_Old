#include <Wire.h> 
#include <Adafruit_LSM303.h> 
#include <SoftwareSerial.h> 
//#include <String.h>
//If you want to change the senser, the only value need to change is MinSensor and MaxSensor.  If you want to test 3 sensors at once, you use value 1 and 3.  If you want to test sensor 1 only, you use 1 and 1.  If you want to test sensor 2 only, you use 2 and 2.  If you want to test sensor 3 only, you use 3 and 3.
#define MinSensor 1  //number of sensor at start.  usually equal to 1
#define MaxSensor 1  //number of sensors used

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

char str[25];
int j=0;
int x=0;


SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
//Flex sensors 
int flexSensorPin[1] = {A0};  //analog pin 0, 1
int flexSensorReading[MaxSensor];
int flex0to100_sensor[MaxSensor];
int flexStep[MaxSensor];
 
//Accelerometer 
//Adafruit_LSM303 lsm; 
 
void setup(){ 
  Serial.begin(9600); 
  
  
  bluetooth.begin(9600);
  
  //bluetooth.begin(115200);

} 
 
 
void loop() {
  int i=0;

  if (Serial.available()) {
   delay(100); //allows all serial sent to be received together
 
  if (Serial.read()=='w'){
      str[i++]='\0';
      Serial.read();Serial.read();Serial.read();
    }
    
  
  while(Serial.available()) {
      str[i] = Serial.read();
  i++;  
  }
    
    if (Serial.read()=='.'){
      str[i++]='\0';
    }
  
  //Serial.print(str);
  //Serial.print('\n');
 
    
  bluetooth.print(str);
  bluetooth.print("This is data is from this arduino \n");
  
  //bluetooth.print(str);
  
}

delay(300);

}
   
/*if(datalily1=='^') {
bluetooth.print("bendsensor");
datalily1=j;
j++;
}
if (j==2){j=0;}
  
if(datalily1=='*') {
bluetooth.print("Accel");
datalily1=x;
x++;
}
if (x==3){x=0;} 
*/  
  
   

//bluetooth.print(datalily1);


