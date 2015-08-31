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

const int CSn = 2; // Chip select
const int CLK = 8; // Clock signal
const int DO = 9; // Digital Output from the encoder which delivers me a 0 or 1, depending on the bar angle..



uint8_t servo1 = 0;
uint8_t servo2 = 3;


int angle1=1;
int angle3=280;



int lastval1 = SERVOMIN;
int lastval2 = SERVOMIN;





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


  if(lastval1<=335){
  for (uint16_t pulselen = lastval1; pulselen <= 335; pulselen++) {
    pwm.setPWM(servo1, 0, pulselen);
  }
  //delay(1);
  }
  else{
  for (uint16_t pulselen = lastval1; pulselen >= 335; pulselen--) {
    pwm.setPWM(servo1, 0, pulselen);
  }
  }
  
  if(lastval2<=150){
  for (uint16_t pulselen = lastval2; pulselen < 150; pulselen++) {
    pwm.setPWM(servo2, 0, pulselen);
  }
  //delay(1);
  }
  else{
  for (uint16_t pulselen = lastval2; pulselen > 150; pulselen--) {
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
      
      for(i=0;i<3;i++){
      
       for (uint16_t pulselen = 150; pulselen < 300; pulselen++) {
    pwm.setPWM(servo2, 0, pulselen);
    encoderval = readSensor();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(encoderval);
   // pwm.setPWM(servo1, 0, pulselen);
    delay(1);
  }
    
   delay(200) ;
     for (uint16_t pulselen = 300; pulselen > 150; pulselen--) {
    pwm.setPWM(servo2, 0, pulselen);
    encoderval = readSensor();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(encoderval);
    //pwm.setPWM(servo1, 0, pulselen);
    delay(1);
  }
  
  delay(200) ;
    
    }
    }
    
    
    
    if (buttons & BUTTON_UP) {
      
 for(i=0;i<4;i++){
      
       for (uint16_t pulselen = 150; pulselen < 300; pulselen++) {
    pwm.setPWM(servo2, 0, pulselen);
    delay(5);
  }
    
   delay(300) ;
     for (uint16_t pulselen = 300; pulselen > 150; pulselen--) {
    pwm.setPWM(servo2, 0, pulselen);
    delay(5);
  }
  
  delay(300) ;
    
    }

    }
    
    
    
    
    if (buttons & BUTTON_LEFT) {
      
      
      
      
      for(i=0;i<10;i++){
      
       for (uint16_t pulselen = 150; pulselen < 300; pulselen++) {
    pwm.setPWM(servo2, 0, pulselen);
    delay(1);
  }
    
   delay(200) ;
     for (uint16_t pulselen = 300; pulselen > 150; pulselen--) {
    pwm.setPWM(servo2, 0, pulselen);
    delay(1);
  }
  
  delay(200) ;
    
    }
    
    
    
   for(i=0;i<4;i++){
      
       for (uint16_t pulselen = 150; pulselen < 300; pulselen++) {
    pwm.setPWM(servo2, 0, pulselen);
    delay(10);
  }
    
   delay(300) ;
     for (uint16_t pulselen = 300; pulselen > 150; pulselen--) {
    pwm.setPWM(servo2, 0, pulselen);
    delay(10);
  }
  
  delay(300) ;
    
    }
    
    
     
    }
    
    
    
    
    if (buttons & BUTTON_RIGHT) {
     
    }
    
    
    
    
    if (buttons & BUTTON_SELECT) {
      
    }
  } 
  
  
  
  
 



  
}

float readSensor(){
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
        dataOut=dataOut+42;
        if(dataOut>4095){
          dataOut=dataOut-4096;}
          float ratio=4096/360;
          float datafloat=float(dataOut);
          datafloat=datafloat/ratio;
          //if(dataOut<55) dataOut=55;
        //dataOut=map(dataOut,55,1965,0,150);
        String sendval = String(datafloat);
        if(datafloat<100){
          sendval='0'+sendval;}
        if(datafloat<10){
          sendval='0'+sendval;}
          sendval=sendval+'\r';
          sendval=sendval+'\n';
       Serial.print(sendval);
	return datafloat;

}


