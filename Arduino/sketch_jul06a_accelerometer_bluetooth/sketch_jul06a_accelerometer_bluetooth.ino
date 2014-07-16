#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);



Adafruit_LSM303 lsm;

void setup() 
{
  Serial.begin(9600);
  
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600); 
  
  
  
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }
}

void loop() 
{
  
   if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());  
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print((char)Serial.read());
  }
  
  
  
  
  lsm.read();
  bluetooth.print("Accel X: "); bluetooth.print((int)lsm.accelData.x); bluetooth.print(" ");
  bluetooth.print("Y: "); bluetooth.print((int)lsm.accelData.y);       bluetooth.print(" ");
  bluetooth.print("Z: "); bluetooth.println((int)lsm.accelData.z);     bluetooth.print(" ");
  bluetooth.print("Mag X: "); bluetooth.print((int)lsm.magData.x);     bluetooth.print(" ");
  bluetooth.print("Y: "); bluetooth.print((int)lsm.magData.y);         bluetooth.print(" ");
  bluetooth.print("Z: "); bluetooth.println((int)lsm.magData.z);       bluetooth.print(" ");
  delay(1000);
}
