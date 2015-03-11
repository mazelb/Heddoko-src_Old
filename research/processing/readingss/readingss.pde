/*
 * Heddoko, March 9, 2015
 *
 * Reading StretchSense data in Processing.
 *
 *  1. Set the port name.
 *  2. Run the program and right-click the mouse.
 *  3. Read the StretchSense data stored in the "ch" variable.
 *    3.1 Time is stored in ch[0] in milliseconds.
 *    3.2 Channel data is stored in ch[1] through ch[5].
 *
 */
import processing.serial.*;

// Serial port name
String portName = "COM5";

// Other variables
Serial port;
int time;
String rawData = "";
int[] ch = new int[6];
Boolean isDataReady = false;

void setup() {
  // Initialize everything...
  size(800, 650);
  background(#eeeeee);
  stroke(#333333);
  time = millis();
  
  // Open port
  println("Starting program...");
  println(Serial.list());
  port = new Serial(this, portName, 115200);
}

void draw() {
  line(150, 25, mouseX, mouseY);
  
  // Break up data string
  if (isDataReady == true) {
    stroke(#009900);
    
    // Channel data
    ch[0] = int(rawData.substring(20)); // Time (in milliseconds)
    ch[1] = int(rawData.substring(0, 4));  // Channel 1
    ch[2] = int(rawData.substring(4, 8));  // Channel 2
    ch[3] = int(rawData.substring(8, 12));  // Channel 3
    ch[4] = int(rawData.substring(12, 16)); // Channel 4
    ch[5] = int(rawData.substring(16, 20)); // Channel 5
    
    // Output result
    println(ch[1] +"\t"+ ch[2] +"\t"+ ch[3] +"\t"+ ch[4] +"\t"+ ch[5] +"\t"+ ch[0]);
    
    // Reset everything
    isDataReady = false;
  }
}

void serialEvent(Serial port) {
  // Wait for data to be used before fetching new data
  if (isDataReady) {
    port.clear();
    return;
  }
  
  int rawValue = port.read();
  char value = char(rawValue);
  
  // Begin a new data string
  if (value == '!') {
    rawData = "";
  }
  
  // Combine raw data into a string
  else {
    rawData = rawData + value;
  }
  
  // Check if we have a full data string
  if (rawData.length() == 28) {
    port.clear();
    isDataReady = true;
  }
}

void mousePressed() {
  background(#eeeeee);
  stroke(#333333);
  
  // Send start command to StretchSense module
  if (port != null) {
    port.write("#s\n");
  }
}
