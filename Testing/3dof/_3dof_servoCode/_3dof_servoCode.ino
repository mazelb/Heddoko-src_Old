#include <Servo.h>
#include <String.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;

int val1, val2, val3;
String str;



void setup() {

  Serial.begin(9600);

  myservo1.attach(9);
  myservo2.attach(10);
  myservo3.attach(11);
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
 String send2;
 String send3;

 
 send1 = strtok(char_array, " ,");
 send2 = strtok(NULL, ",");
 send3 = strtok(NULL, ",");
 
 
 int angle1;
 int angle2;
 int angle3;
 
 angle1=send1.toInt();
 angle2=send2.toInt();
 angle3=send3.toInt();
 
 myservo1.write(angle1);
 myservo2.write(angle2);
 myservo3.write(angle3);
 
 delay(1);
 
 /*w
  myservo1.write(0);
  //myservo2.write(0);
  //myservo3.write(0);

  delay(999);
  myservo1.write(180);
  //myservo2.write(180);
  //myservo3.write(180);
  delay(999);

*/
/*
for(int i=0;i<180;i++)
{
 myservo2.write(i); 
 delay(20);
}

for(int i=179;i>0;i--)
{
 myservo2.write(i); 
 delay(20);
}
*/
}
}
