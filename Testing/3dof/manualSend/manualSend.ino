#include <Servo.h>
#include <String.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;



void setup() {

  Serial.begin(9600);

  myservo1.attach(9);
  myservo2.attach(10);
  myservo3.attach(11);
}

void loop() {
  
  

 
 myservo1.write(0);
 myservo2.write(10);
 myservo3.write(10);
}