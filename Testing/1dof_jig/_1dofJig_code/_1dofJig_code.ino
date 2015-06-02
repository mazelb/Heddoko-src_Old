#include <Servo.h>
#include <String.h>

Servo myservo1;


int val1;
String str;



void setup() {

  Serial.begin(9600);

  myservo1.attach(9);
}

void loop() {
  
  
  if (Serial.available())
  {
    
 str = Serial.readStringUntil('\n');
 //str="90,90,90,";
 int str_len = str.length() + 1;
 char char_array[str_len];
 
 str.toCharArray(char_array, str_len);
 
 String send1;


 
 send1 = strtok(char_array, " ,");

 
 
 int angle1;

 
 angle1=send1.toInt();

 
 myservo1.write(angle1);

 
 delay(1);
 

}
}
