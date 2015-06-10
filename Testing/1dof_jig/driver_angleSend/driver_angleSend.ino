#include <Wire.h>
#include <String.h>
#include <Adafruit_PWMServoDriver.h>

int val1;
String str;



Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();



#define SERVOMIN  120 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;
int lastval = SERVOMIN;

void setup() {
  Serial.begin(115200);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
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
  // Drive each servo one at a time
   
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
}
