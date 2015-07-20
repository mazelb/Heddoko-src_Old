#include <Wire.h>
#include <String.h>
#include <Adafruit_PWMServoDriver.h>

#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


#define SERVOMIN  135 
#define SERVOMAX  500



uint8_t servo1 = 0;
uint8_t servo2 = 3;


int angle1=1;
int angle3=280;



int lastval1 = SERVOMIN;
int lastval2 = SERVOMIN;



const int CSn = 2; // Chip select
const int CLK = 8; // Clock signal
const int DO = 9; // Digital Output from the encoder which delivers me a 0 or 1, depending on the bar angle..

float encoderval = 0.0;


void setup(){

	
        Serial.begin(115200);
        
        pwm.begin();
        pwm.setPWMFreq(60);
        
        
        lcd.begin(16, 2);
        lcd.setBacklight(WHITE);
        lcd.setCursor(0,0);
        lcd.print("Heddoko Elbow");
        
        
        
	pinMode(CSn, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(DO, INPUT);

	//Let's start here
	digitalWrite(CLK, HIGH);
	digitalWrite(CSn, HIGH);




 int angle2 = map(angle1, 0, 180, SERVOMIN, SERVOMAX);
 int angle4 = map(angle3, 0, 180, SERVOMIN, SERVOMAX);


  if(lastval1<=angle2){
  for (uint16_t pulselen = lastval1; pulselen < angle2; pulselen++) {
    pwm.setPWM(servo1, 0, pulselen);
  }
  //delay(1);
  }
  else{
  for (uint16_t pulselen = lastval1; pulselen > angle2; pulselen--) {
    pwm.setPWM(servo1, 0, pulselen);
  }
  }
  
  if(lastval2<=angle4){
  for (uint16_t pulselen = lastval2; pulselen < 680; pulselen++) {
    pwm.setPWM(servo2, 0, pulselen);
  }
  //delay(1);
  }
  else{
  for (uint16_t pulselen = lastval2; pulselen > 680; pulselen--) {
    pwm.setPWM(servo2, 0, pulselen);
  }
  }
lastval1=angle2;
angle2=angle1;
lastval2=angle4;
angle4=angle3;



    
  


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



uint8_t i=0;






void loop() {
 
 
 uint8_t buttons = lcd.readButtons();


if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    
    
    if (buttons & BUTTON_DOWN) {
      
      for(i=0;i<10;i++){
      
       for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servo1, 0, pulselen);
    delay(1);
  }
    
   delay(100) ;
     for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servo1, 0, pulselen);
    delay(1);
  }
  
  delay(10) ;
    
    }
    }
    
    
    
    if (buttons & BUTTON_UP) {
      
 for(i=0;i<4;i++){
      
       for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servo1, 0, pulselen);
    delay(5);
  }
    
   delay(1500) ;
     for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servo1, 0, pulselen);
    delay(5);
  }
  
  delay(1500) ;
    
    }

    }
    
    
    
    
    if (buttons & BUTTON_LEFT) {
      
      
      
      
      for(i=0;i<4;i++){
      
       for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servo1, 0, pulselen);
  }
    
   delay(1500) ;
     for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servo1, 0, pulselen);
  }
  
  delay(1500) ;
    
    }
    
    
    
   for(i=0;i<4;i++){
      
       for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servo1, 0, pulselen);
    delay(10);
  }
    
   delay(1500) ;
     for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servo1, 0, pulselen);
    delay(10);
  }
  
  delay(1500) ;
    
    }
    
    
     
    }
    
    
    
    
    if (buttons & BUTTON_RIGHT) {
     
    }
    
    
    
    
    if (buttons & BUTTON_SELECT) {
      
    }
  } 
  
  
  
  
 



  
}




