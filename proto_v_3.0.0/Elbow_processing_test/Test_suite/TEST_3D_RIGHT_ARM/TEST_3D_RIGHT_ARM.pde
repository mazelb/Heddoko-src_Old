import processing.serial.*;
import java.awt.datatransfer.*;
import java.awt.Toolkit;
import processing.opengl.*;
import saito.objloader.*;
import g4p_controls.*;

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

void setup()
{
    // Resolution by default is 1000x800, using P3D which tries to use OpenGL if possible
  size(1000, 1000, OPENGL);
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
 
void draw()
{
  background(0.5f,0.5f,0.5f);
  
  // Set a new co-ordinate space
  pushMatrix();

    //First I find all my rotation angles in radians so that center screen is (0,0)
    float rotx = (mouseY/360.0)*-2*PI+PI;
    float roty = (mouseX/420.0)*2*PI-PI;
    float rotz = rotzfactor*PI/36;    
    
    background(0);
    stroke(0, 0, 200);
    line (0,20,420,20); // monitor bar
    fill(255);
    textSize(10);
    text (" rotateX(" + rotx +" pi)"+" , rotateY("+roty/PI+" pi)"+" , rotateZ("+rotzfactor+"pi/36)", 0,10);
    fill(0, 0, 200);
    text (" Mouse controls X and Y, UP and DOWN controls Z", 0,22);
    
    
    //========================================================================//
    //=========================== UPPER TORSO ================================//
    //========================================================================//

    //========================================================================//
    //==============================SPINE=====================================//
    //========================================================================//
    
    // Set a new co-ordinate space
    pushMatrix();
    
      // center drawing start point in screen
      translate(100, 300, 0); 
      
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
//      rotateX(radians(roll[6]));   
//      rotateZ(radians(pitch[6]));  
//      rotateY(radians(yaw[6]));    
//      rotateX(rotx);   
//      rotateY(roty);  
//      rotateZ(rotz);    
      
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
        
     
      //========================================================================//
      //============================ RIGHT ARM =================================//
      //========================================================================//
  
  
      //========================================================================//
      //==============================Shoulder==================================//
      //========================================================================//
      
      // Set a new co-ordinate space
      pushMatrix();
      
        // center drawing start point in screen
        translate(250, 0, 0); 
         
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
//        rotateX(radians(roll[1]));   
//        rotateZ(radians(pitch[1]));    


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
  
        //========================================================================//
        //===============================Upper Arm================================//
        //========================================================================//
        
        // Set a new co-ordinate space
        pushMatrix();
        
          // center drawing start point in screen
          translate(250, 0, 0); 
           
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
       
       
          //========================================================================//
          //=============================ForeArm ===================================//
          //========================================================================//
          
          // Set a new co-ordinate space
          pushMatrix();
          
            // center drawing start point in screen
            translate(0, 300, 0); 
            
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
            rotateX(-radians(roll[0]));    //autour de X   
            rotateZ(radians(pitch[0]));   //autour de Z
  
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
             
            //========================================================================//
            //================================Wrist===================================//
            //========================================================================//
            
            // Set a new co-ordinate space
            pushMatrix();
            
              // center drawing start point in screen
              translate(0, 300, 0); 
               
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
              box(100); 
            popMatrix();
          popMatrix();
        popMatrix();      
      popMatrix();
    popMatrix(); 
  popMatrix();
}

void serialEvent(Serial p) 
{
  String incoming = p.readString();
  if (printSerial) 
  {
    println(incoming);
  }
  //println(incoming.length());
  if ((incoming.length() > 20))
  {
    String[] list = split(incoming, ",");
    //println(list.length);

    if ( list.length >= 9 )
    { 
      //println(list[3]);
      //fill the data
      {
        roll[0]  =  constrain(2*float(list[0]), 0, 180);  
        roll[1]  =  constrain(2*float(list[3]), 0, 360);
        roll[2]  =  float(list[6]);
        
        pitch[0] =  constrain(float(list[1]), 120, 240);
        pitch[1] =  float(list[4]);  
        pitch[2] =  float(list[7]);
        
        yaw[0]   =  float(list[2]);  
        yaw[1]   =  float(list[5]);  
        yaw[2]   =  float(list[8]);
      }
      println(list[3]);      
      println(roll[1]);
      println("--------------");

      buffer = incoming;
    }
  }
}

// Set serial port to desired value.
void setSerialPort(String portName) 
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
    port.bufferUntil('\n');
        
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

void handlePanelEvents(GPanel panel, GEvent event) 
{
  // Panel events, do nothing.
}

void handleDropListEvents(GDropList list, GEvent event) 
{ 
  // Drop list events, check if new serial port is selected.
  if (list == serialList) 
  {
    setSerialPort(serialList.getSelectedText()); 
  }
}

void handleToggleControlEvents(GToggleControl checkbox, GEvent event) 
{ 
  // Checkbox toggle events, check if print events is toggled.
  if (checkbox == printSerialCheckbox) 
  {
    printSerial = printSerialCheckbox.isSelected(); 
  }
}

