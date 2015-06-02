#include <Servo.h>

Servo myservo1;




void setup() {

  Serial.begin(9600);

  myservo1.attach(9);

}

void loop() {
  
  

 
 myservo1.write(12);

}
