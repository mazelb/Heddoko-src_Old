#include <SoftwareSerial.h> 

int bluetoothTx=10;
int bluetoothRx=11;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
 
void setup()
{ 
  Serial.begin(9600); 
  
  bluetooth.begin(9600);
  //bluetooth.begin(115200);
} 


 
void loop() 
{
  //Serial.println(bluetooth.available());
  if(bluetooth.available() > 0)  // If the bluetooth sent any characters
  {
    Serial.println("DATA BLUETOOTH***********************************************");
    // Send any characters the bluetooth prints to the serial monitor
    Serial.println((char)bluetooth.read());  
    delay(100);
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    Serial.println("DATA SERIAL***********************************************");
    // Send any characters the Serial monitor prints to the bluetooth
    //bluetooth.println((char)Serial.read());
    bluetooth.write((char)Serial.read());
    delay(100);
  }
  // and loop forever and ever!
}
