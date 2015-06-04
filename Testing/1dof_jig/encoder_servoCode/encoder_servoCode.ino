// Read in 12-bits Magnetic Encoder AEAT-6012-A06  into Arduino Uno
// RobinL

// Declarate

#include <Wire.h>
#include <String.h>
#include <Adafruit_PWMServoDriver.h>
int val1;
String str;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  120 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // this is the 'maximum' pulse length count (out of 4096)

uint8_t servonum = 0;
int lastval = SERVOMIN;



const int CSn = 4; // Chip select
const int CLK = 8; // Clock signal
const int DO = 9; // Digital Output from the encoder which delivers me a 0 or 1, depending on the bar angle..

unsigned int sensorWaarde = 0;

void setup(){

	//Fix de tris
        Serial.begin(115200);
        
        pwm.begin();
        pwm.setPWMFreq(60);
        
        
	pinMode(CSn, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(DO, INPUT);

	//Let's start here
	digitalWrite(CLK, HIGH);
	digitalWrite(CSn, HIGH);
}



void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}




void loop() {
  
  
  if (Serial.available())
  {
    
 str = Serial.readStringUntil('\n');

 int str_len = str.length() + 1;
 char char_array[str_len];
 
 str.toCharArray(char_array, str_len);
 
 String send1;


 
 send1 = strtok(char_array, " ,");

 
 
 int angle1;
  
 angle1=send1.toInt();  
   
 angle1 = map(angle1, 0, 180, SERVOMIN, SERVOMAX);
 

  Serial.println(servonum);
  
  if(lastval<=angle1){
  for (uint16_t pulselen = lastval; pulselen < angle1; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  delay(100);
  }
  else{
  for (uint16_t pulselen = lastval; pulselen > angle1; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  }
  delay(100);

lastval=angle1;

}
  
  
  

	sensorWaarde = readSensor();
	delayMicroseconds(1); //Tcs waiting for another read in
}

unsigned int readSensor(){
	unsigned int dataOut = 0;

	digitalWrite(CSn, LOW);
	delayMicroseconds(1); //Waiting for Tclkfe

	//Passing 12 times, from 0 to 9
	for(int x=0; x<12; x++){
		digitalWrite(CLK, LOW); 
		delayMicroseconds(1); //Tclk/2
		digitalWrite(CLK, HIGH);
		delayMicroseconds(1); //Tdo valid, like Tclk/2
		dataOut = (dataOut << 1) | digitalRead(DO); //shift all the entering data to the left and past the pin state to it. 1e bit is MSB
	}

	digitalWrite(CSn, HIGH); // 
       Serial.println(dataOut);
	return dataOut;

}
