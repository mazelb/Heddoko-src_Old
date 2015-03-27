import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 
import java.awt.datatransfer.*; 
import java.awt.Toolkit; 
import processing.opengl.*; 
import saito.objloader.*; 
import g4p_controls.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class IMU_TEST_V2_1 extends PApplet {








float roll[]  = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
float pitch[] = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
float yaw[]   = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
float bend[]  = {0.0F, 0.0F};

// TEMP
int rotzfactor = 0; //z rotation factor controlled by up/down arrows

// Serial port state.
Serial       port;
String       buffer = "";
final String serialConfigFile = "serialconfig.txt";
boolean      printSerial = false;

// UI controls.
GPanel    configPanel;
GDropList serialList;
GLabel    serialLabel;
GCheckbox printSerialCheckbox;

public void setup()
{
    // Resolution by default is 1000x800, using P3D which tries to use OpenGL if possible
  size(800, 1000, OPENGL);
  frameRate(30);

  
  // Serial port setup.
  // Grab list of serial ports and choose one that was persisted earlier or default to the first port.
  int selectedPort = 0;
  String[] availablePorts = Serial.list();
  if (availablePorts == null) 
  {
    println("ERROR: No serial ports available!");
    exit();
  }
  
  String[] serialConfig = loadStrings(serialConfigFile);
  if (serialConfig != null && serialConfig.length > 0) 
  {
    String savedPort = serialConfig[0];
    // Check if saved port is in available ports.
    for (int i = 0; i < availablePorts.length; ++i) 
    {
      if (availablePorts[i].equals(savedPort)) 
      {
        selectedPort = i;
      } 
    }
  }

  // Build serial config UI.
  configPanel = new GPanel(this, 10, 10, width-20, 90, "Configuration (click to hide/show)");
  serialLabel = new GLabel(this,  0, 20, 80, 25, "Serial port:");
  configPanel.addControl(serialLabel);
  serialList = new GDropList(this, 90, 20, 200, 200, 6);
  serialList.setItems(availablePorts, selectedPort);
  configPanel.addControl(serialList);
  printSerialCheckbox = new GCheckbox(this, 5, 50, 200, 20, "Print serial data");
  printSerialCheckbox.setSelected(printSerial);
  configPanel.addControl(printSerialCheckbox);
  
  // Set serial port.
  setSerialPort(serialList.getSelectedText());
}
 
public void draw()
{
  background(0.5f,0.5f,0.5f);
  
  // Set a new co-ordinate space
  pushMatrix();

  //First I find all my rotation angles in radians so that center screen is (0,0)
  float rotx = (mouseY/360.0f)*-2*PI+PI;
  float roty = (mouseX/420.0f)*2*PI-PI;
  float rotz = rotzfactor*PI/36;
    
  background(0);
  stroke(0, 0, 200);
  line (0,20,420,20); // monitor bar
  fill(255);
  textSize(10);
  text (" rotateX(" + rotx +" pi)"+" , rotateY("+roty/PI+" pi)"+" , rotateZ("+rotzfactor+"pi/36)", 0,10);
  fill(0, 0, 200);
  text (" Mouse controls X and Y, UP and DOWN controls Z", 0,22);
   
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(210, 180, 0); 
  
  
  //========================================================================//
  //==============================CUBE 1====================================//
  //========================================================================//

  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60);
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[0]));   
  rotateZ(radians(pitch[0]));  
  rotateY(radians(yaw[0]));    
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150);
  
  // draw rotating box
  strokeWeight(2);
  stroke(255, 0, 0);
  noFill();
  box(140);
  
  popMatrix();
  
  
  
  //========================================================================//
  //==============================CUBE 2====================================//
  //========================================================================//
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(420, 180, 0); 
   
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0); 
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60);
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[1]));   
  rotateZ(radians(pitch[1]));  
  rotateY(radians(yaw[1]));     
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150);
  
  // draw rotating box
  strokeWeight(2);
  stroke(255, 0, 0);
  noFill();
  box(140); 
  
  popMatrix();
  
  
  
  //========================================================================//
  //==============================CUBE 3====================================//
  //========================================================================// 
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(630, 180, 0); 
  
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60); 
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[2]));   
  rotateZ(radians(pitch[2]));  
  rotateY(radians(yaw[2]));    
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);       
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150); 
  
  // draw rotating box
  strokeWeight(2);
  stroke(255, 0, 0);
  noFill();
  box(140); 
    
  popMatrix();
 

  //========================================================================//
  //==============================CUBE 4====================================//
  //========================================================================//
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(210, 460, 0); 
   
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);       
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60); 
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[3]));   
  rotateZ(radians(pitch[3]));  
  rotateY(radians(yaw[3]));      
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);       
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150); 
  
  // draw rotating red box
  strokeWeight(2);
  stroke(0, 255, 0);
  noFill();
  box(140); 
    
  popMatrix();
 
 
 
  //========================================================================//
  //==============================CUBE 5====================================//
  //========================================================================//
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(420, 460, 0); 
  
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);       
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60); 
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[4]));   
  rotateZ(radians(pitch[4]));  
  rotateY(radians(yaw[4]));     
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);       
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150); 
  
  // draw rotating red box
  strokeWeight(2);
  stroke(0, 255, 0);
  noFill();
  box(140); 
    
  popMatrix();
  
 
  //========================================================================//
  //==============================CUBE 6====================================//
  //========================================================================//
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(630, 460, 0); 
   
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);       
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60); 
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[5]));   
  rotateZ(radians(pitch[5]));  
  rotateY(radians(yaw[5]));     
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);       
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150); 
  
  // draw rotating red box
  strokeWeight(2);
  stroke(0, 255, 0);
  noFill();
  box(140); 
    
  popMatrix();
  
  
  
  //========================================================================//
  //==============================CUBE 7====================================//
  //========================================================================//
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(210, 740, 0); 
  
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);       
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60); 
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[6]));   
  rotateZ(radians(pitch[6]));  
  rotateY(radians(yaw[6]));    
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);       
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150); 
  
  // draw rotating red box
  strokeWeight(2);
  stroke(0, 0, 255);
  noFill();
  box(140); 
    
  popMatrix();


  
  //========================================================================//
  //==============================CUBE 8====================================//
  //========================================================================//
  
  // Set a new co-ordinate space
  pushMatrix();
  
  // center drawing start point in screen
  translate(420, 740, 0); 
   
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);       
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60); 
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[7]));   
  rotateZ(radians(pitch[7]));  
  rotateY(radians(yaw[7]));     
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);       
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150); 
  
  // draw rotating red box
  strokeWeight(2);
  stroke(0, 0, 255);
  noFill();
  box(140); 
    
  popMatrix();


  
  //========================================================================//
  //==============================CUBE 9====================================//
  //========================================================================//
  
  // Set a new co-ordinate space
  pushMatrix();

  // center drawing start point in screen
  translate(630, 740, 0); 
   
  // draw stationary axis lines
  strokeWeight(1);
  stroke(50);
  line(-60, 0, 60, 0);       
  line(0, 60, 0, -60);       
  line(0, 0, -60, 0, 0, 60); 
  
  // draw stationary box
  strokeWeight(1);
  stroke(0, 150, 0);
  noFill();
  box(140);                  
   
  // rotate drawing coordinates according to user input variables
  rotateX(radians(roll[8]));   
  rotateZ(radians(pitch[8]));  
  rotateY(radians(yaw[8]));    
  
  // draw the rotating axis lines
  strokeWeight(2);
  stroke(255);
  line(-150, 0, 150, 0);       
  line(0, 150, 0, -150);       
  line(0, 0, -150, 0, 0, 150); 
  
  // draw rotating red box
  strokeWeight(2);
  stroke(0, 0, 255);
  noFill();
  box(140); 
  
  popMatrix();

  popMatrix();
}

public void serialEvent(Serial p) 
{
  String incoming = p.readString();
  if (printSerial) 
  {
    println(incoming);
  }
  //println(incoming.length());
  
  if ((incoming.length() > 300))
  {
    String[] list = split(incoming, "\n");
    if ( list.length == 43 )  // 43 is the moment where the serial input stabilizes
    { 
      //fill the data
      {
        roll[0]  =  PApplet.parseFloat(list[3]);  roll[1]  =  PApplet.parseFloat(list[7]);  roll[2]  =  PApplet.parseFloat(list[11]);
        roll[3]  =  PApplet.parseFloat(list[17]); roll[4]  =  PApplet.parseFloat(list[21]); roll[5]  =  PApplet.parseFloat(list[25]);
        roll[6]  =  PApplet.parseFloat(list[31]); roll[7]  =  PApplet.parseFloat(list[35]); roll[8]  =  PApplet.parseFloat(list[39]);
        
        pitch[0]  =  PApplet.parseFloat(list[4]);  pitch[1]  =  PApplet.parseFloat(list[8]);  pitch[2]  =  PApplet.parseFloat(list[12]);
        pitch[3]  =  PApplet.parseFloat(list[18]); pitch[4]  =  PApplet.parseFloat(list[22]); pitch[5]  =  PApplet.parseFloat(list[26]);
        pitch[6]  =  PApplet.parseFloat(list[32]); pitch[7]  =  PApplet.parseFloat(list[36]); pitch[8]  =  PApplet.parseFloat(list[40]);

        yaw[0]  =  PApplet.parseFloat(list[5]);  yaw[1]  =  PApplet.parseFloat(list[9]);  yaw[2]  =  PApplet.parseFloat(list[13]);
        yaw[3]  =  PApplet.parseFloat(list[19]); yaw[4]  =  PApplet.parseFloat(list[23]); yaw[5]  =  PApplet.parseFloat(list[27]);
        yaw[6]  =  PApplet.parseFloat(list[33]); yaw[7]  =  PApplet.parseFloat(list[37]); yaw[8]  =  PApplet.parseFloat(list[41]);

        bend[0] = PApplet.parseFloat(list[15]); bend[1] = PApplet.parseFloat(list[29]);
      }      

      buffer = incoming;
    }
  }
}

// Set serial port to desired value.
public void setSerialPort(String portName) 
{
  // Close the port if it's currently open.
  if (port != null) 
  {
    port.stop();
  }
  try 
  {
    // Open port.
    port = new Serial(this, portName, 9600);
    port.bufferUntil('!');
        
    // Persist port in configuration.
    saveStrings(serialConfigFile, new String[] { portName });
  }
  catch (RuntimeException ex) 
  {
    // Swallow error if port can't be opened, keep port closed.
    port = null; 
  }
}

// UI event handlers

public void handlePanelEvents(GPanel panel, GEvent event) 
{
  // Panel events, do nothing.
}

public void handleDropListEvents(GDropList list, GEvent event) 
{ 
  // Drop list events, check if new serial port is selected.
  if (list == serialList) 
  {
    setSerialPort(serialList.getSelectedText()); 
  }
}

public void handleToggleControlEvents(GToggleControl checkbox, GEvent event) 
{ 
  // Checkbox toggle events, check if print events is toggled.
  if (checkbox == printSerialCheckbox) 
  {
    printSerial = printSerialCheckbox.isSelected(); 
  }
}

  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "--full-screen", "--bgcolor=#666666", "--stop-color=#cccccc", "IMU_TEST_V2_1" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
