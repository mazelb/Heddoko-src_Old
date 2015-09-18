
#include <Wire.h>
#include <String.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define SERVOMIN  120 
#define SERVOMAX  500 

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servo1 = 3;
uint8_t servo2 = 0;

int angle1    = 250;
int angle2    = 8;
int increment = 100;

int lastval1 = map(angle1, 0, 180, SERVOMIN, SERVOMAX);
int lastval2 = map(angle2, 0, 180, SERVOMIN, SERVOMAX);

const int CSn = 2; 
const int CLK = 8; 
const int DO  = 9; 

float encoderval = 0.0;


void setup()
{
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

  digitalWrite(CLK, HIGH);
  digitalWrite(CSn, HIGH);
}


void setServoPulse(uint8_t n, double pulse) 
{
  double pulselength;
  
  pulselength = 1000000;   
  pulselength /= 60;   
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}




void loop() 
{
   
  int angle1Transformed = map(angle1, 0, 180, SERVOMIN, SERVOMAX);
  int angle2Transformed = map(angle2, 0, 180, SERVOMIN, SERVOMAX);

  uint8_t buttons = lcd.readButtons();
  
  if (buttons) 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    
    if (buttons & BUTTON_DOWN) 
    {
      angle1 = angle1 + increment;
      if(angle1 > 179)
      {
        angle1 = 180;
      }

    }
    
    if (buttons & BUTTON_UP) 
    {
      angle1 = angle1 - increment;
      if(angle1 < 1)
      {
        angle1 = 0;
      }
    }
    
    if (buttons & BUTTON_LEFT) 
    {
      angle2 = angle2 - increment;
    }
   
    if (buttons & BUTTON_RIGHT) 
    {
      angle2 = angle2 + increment;
    }
    
  }
  
  if(lastval1 <= angle1Transformed)
  {
    for(uint16_t pulselen = lastval1; pulselen < angle1Transformed; pulselen++) 
    {
      readSensor();
      delay(15);
      pwm.setPWM(servo1, 0, pulselen);
      delay(45);
    }
  }
  
  else
  {
    for(uint16_t pulselen = lastval1; pulselen > angle1Transformed; pulselen--) 
    {
      readSensor();
      delay(15);
      pwm.setPWM(servo1, 0, pulselen);
      delay(45);
    }
  }

  if(lastval2 <= angle2Transformed)
  {
    for(uint16_t pulselen = lastval2; pulselen < angle2Transformed; pulselen++) 
    {
      readSensor();
      delay(15);
      pwm.setPWM(servo2, 0, pulselen);
      delay(45);
    }
  }
  
  else
  {
    for(uint16_t pulselen = lastval2; pulselen > angle2Transformed; pulselen--) 
    {
      readSensor();
      delay(15);
      pwm.setPWM(servo2, 0, pulselen);
      delay(45);
    }
  }

  lastval1  = angle1Transformed;
  //angle1Transformed    = angle1;
  lastval2  = angle2Transformed;
  //angle2Transformed    = angle2;
  //delayMicroseconds(1);
}


float readSensor()
{
  unsigned int dataOut = 0;
  digitalWrite(CSn, LOW);
  delayMicroseconds(1); 
  
  for(int x = 0; x < 12; x++)  
  {
    digitalWrite(CLK, LOW); 
    delayMicroseconds(1); //Tclk/2
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    dataOut = (dataOut << 1) | digitalRead(DO); //shift all the entering data to the left and past the pin state to it. 1e bit is MSB
  }

  digitalWrite(CSn, HIGH); // 
  dataOut = dataOut+42;
  
  if(dataOut > 4095)
  {
    dataOut = dataOut-4096;
  }

  float ratio      = 4096/360;
  float datafloat  = float(dataOut);
  datafloat        = datafloat/ratio;
  datafloat        = datafloat/372.27;
  datafloat        = datafloat*360;
  String sendval   = String(datafloat);
  
  if(datafloat < 100)
  {
    sendval = '0' + sendval;
  }

  if(datafloat < 10)
  {
    sendval = '0' + sendval;
  }

  sendval = sendval + '\r';
  sendval = sendval + '\n';

  Serial.print(sendval);
  return datafloat;
}
