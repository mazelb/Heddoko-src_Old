/*
 * Heddoko, March 23, 2015
 *
 * Description...
 * Borrows code from "Heddoko-src/proto_v_2.0.0/Test_suite/TEST_3D_RIGHT_ARM"
 * and "Heddoko-src/research/processing/readingss"
 *
 * 1. 
 * 2. Import G4P library through "Sketch > Import Library > Add Library"
 */
import processing.serial.*;
import processing.opengl.*;
import g4p_controls.*;

// Some variables.
Serial port;
Boolean isDataReady = false;
String buffer = "";
int[] ch = new int[6];
int bgColour = 130, live = 1;
float rotx, roty, rotz;

// UI controls.
GPanel configPanel;
GLabel serialLabel, buttonsLabel;
GDropList serialList;
GButton startButton, stopButton;

void setup()
{
  size(600, 600, OPENGL);
  background(bgColour);
  
  // 
  //init();
  
  // Serial port setup. Grab list of serial ports and choose one that was persisted
  // earlier or default to the first port.
  int selectedPort = 0;
  String[] availablePorts = Serial.list();
  if (availablePorts == null || availablePorts.length == 0) {
    println("ERROR: No serial ports available!");
    availablePorts[0] = "--";
    live = 0;
  }
  
  String[] serialConfig = loadStrings("SerialConfig.txt");
  if (serialConfig != null && serialConfig.length > 0) {
    String savedPort = serialConfig[0];
    
    // Check if saved port is in available ports.
    for (int i = 0; i < availablePorts.length; ++i) {
      if (availablePorts[i].equals(savedPort)) {
        selectedPort = i;
      } 
    }
  }

  // Build serial config UI.
  configPanel = new GPanel(this, 10, 10, 300, 100, "Configuration (click to hide/show)");
  serialLabel = new GLabel(this,  0, 30, 80, 25, "Serial port:");
  configPanel.addControl(serialLabel);
  serialList = new GDropList(this, 90, 30, 200, 200, 6);
  serialList.setItems(availablePorts, selectedPort);
  configPanel.addControl(serialList);
  buttonsLabel = new GLabel(this, 0, 60, 100, 25, "SS commands:");
  configPanel.addControl(buttonsLabel);
  startButton = new GButton(this, 110, 60, 60, 25, "start");
  configPanel.addControl(startButton);
  stopButton = new GButton(this, 180, 60, 60, 25, "stop");
  configPanel.addControl(stopButton);
  
  // Set serial port.
  if (live == 1) {
    setSerialPort(serialList.getSelectedText());
  }
}

/**
 *
 */
void draw() {
  
  if (isDataReady) {
    background(bgColour);
    
    // Retrieve channel data.
    fetchData();
    
    // Calculate frontal plane angle
    rotx = (mouseY/360.0)*-2*PI+PI;
    rotx = map(ch[1], 1080, 1400, 0.0, PI);
    
    // Calculate axial plane angle
    roty = (mouseX/420.0)*2*PI-PI;
    roty = 0.0;
    
    // Calculate ...
    rotz = 0.0;
    
    // Animate cube.
    cube(rotx, roty, 0.0);
  }
  
  // Follow the mouse pointer, for fun (testing).
  else if (live == 0) {
    cube(((mouseY/360.0)*-2*PI+PI), ((mouseX/420.0)*2*PI-PI), 0.0);
  }
}

/**
 *
 */
public void serialEvent(Serial port) {
  
  // Wait for data to be used before fetching new data
  if (isDataReady) {
    port.clear();
    return;
  }
  
  String raw = port.readString();
  
  if (raw.length() >= 29) {
    
    String[] data = split(raw, "!");
    
    if (data[1].length() == 32) {
      buffer = data[1];
      port.clear();
      isDataReady = true;
    }
  }
}
/*void serialEvent(Serial port) {
  
  // Wait for data to be used before fetching new data
  if (isDataReady) {
    port.clear();
    return;
  }
  
  int rawValue = port.read();
  char value = char(rawValue);
  
  // Begin a new data string
  if (value == '!') {
    buffer = "";
  }
  
  // Combine raw data into a string
  else {
    buffer = buffer + value;
  }
  
  // Check if we have a full data string
  if (buffer.length() == 28) {
    port.clear();
    isDataReady = true;
  }
}*/

/**
 *
 */
public void handlePanelEvents(GPanel panel, GEvent event) { /* code */ }

/**
 * Check if new serial port is selected.
 */
public void handleDropListEvents(GDropList list, GEvent event) {
  if (list == serialList) {
    setSerialPort(serialList.getSelectedText()); 
  }
}

/**
 *
 */
public void handleButtonEvents(GButton btn, GEvent event) {
  
  // Send start command to StretchSense module.
  if (btn == startButton && port != null) {
    bgColour = 230;
    port.write("#s\n");
    println("START command sent (#s)");
  }
  
  // Send stop command.
  if (btn == stopButton && port != null) {
    bgColour = 130;
    port.write("#t\n");
    println("STOP command sent (#t)");
  }
  
  if (port == null) {
    print((btn == startButton ? "START cmd" : "STOP cmd"));
    println(" (not connected)");
  }
}


