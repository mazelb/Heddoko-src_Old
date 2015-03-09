//
// Heddoko
// March 9, 2015

import processing.serial.*;

// Serial port info
Serial port;
String portName = "COM5";
int baudRate = 115200;

// StretchSense data
String[] rawData = new String[7];
int[] ch = new int[7];

void setup() {
  size(800, 650);
  
  // println(Serial.list());
  port = new Serial(this, portName, baudRate);
  //port.write("#s\n");
}

void draw() {
  
}

// Called whenever there is something available to read
void serialEvent(Serial port) {
  int rawValue = port.read();
  char value = char(rawValue);
  println("Input: "+ value);
}
