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
    //==============================CUBE 7====================================//
    //========================================================================//
    
    // Set a new co-ordinate space
    pushMatrix();
    
      // center drawing start point in screen
      translate(500, 300, 0); 
      
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
      //============================= LEFT ARM =================================//
      //========================================================================//
         
      //========================================================================//
      //==============================CUBE 1====================================//
      //========================================================================//
    
      // Set a new co-ordinate space
      pushMatrix();
      
        // center drawing start point in screen
        translate(-350, -100, 0); 
        
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
//        rotateX(radians(roll[0]));   
//        rotateZ(radians(pitch[0]));  
//        rotateY(radians(yaw[0]));    
//        rotateX(rotx);   
//        rotateY(rotz);  
//        rotateZ(roty);    
      
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
      
        //popMatrix();
      
      
        //========================================================================//
        //==============================CUBE 2====================================//
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
//          rotateX(radians(roll[1]));   
//          rotateZ(radians(pitch[1]));  
//          rotateY(radians(yaw[1]));  
//          rotateX(rotx);   
//          rotateY(rotz);  
//          rotateZ(roty); 

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
             
      
          //========================================================================//
          //==============================CUBE 3====================================//
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
//            rotateX(radians(roll[2]));   
//            rotateZ(radians(pitch[2]));  
//            rotateY(radians(yaw[2]));    
//            rotateX(rotx);   
//            rotateY(rotz);  
//            rotateZ(roty); 
  
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
        popMatrix();
      popMatrix();
     
      //========================================================================//
      //============================ RIGHT ARM =================================//
      //========================================================================//
  
  
      //========================================================================//
      //==============================CUBE 4====================================//
      //========================================================================//
      
      // Set a new co-ordinate space
      pushMatrix();
      
        // center drawing start point in screen
        translate(350, -100, 0); 
         
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
//        rotateX(rotx);   
//        rotateY(rotz);  
//        rotateZ(roty); 

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
        //==============================CUBE 5====================================//
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
          rotateX(radians(roll[4]));   
          rotateZ(radians(pitch[4]));  
          rotateY(radians(yaw[4]));     
//          rotateX(rotx);   
//          rotateY(rotz);  
//          rotateZ(roty); 

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
          //==============================CUBE 6====================================//
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
            rotateX(radians(roll[5]));   
            rotateZ(radians(pitch[5]));  
            rotateY(radians(yaw[5]));     
//            rotateX(rotx);   
//            rotateY(rotz);  
//            rotateZ(roty); 

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
        popMatrix();      
      popMatrix();
    popMatrix();

    //========================================================================//
    //=========================== LOWER TORSO ================================//
    //========================================================================//


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
//      rotateX(radians(roll[7]));   
//      rotateZ(radians(pitch[7]));  
//      rotateY(radians(yaw[7]));     
      
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
//      rotateX(radians(roll[8]));   
//      rotateZ(radians(pitch[8]));  
//      rotateY(radians(yaw[8]));    
      
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

void serialEvent(Serial p) 
{
  String incoming = p.readString();
  if (printSerial) 
  {
    println(incoming);
  }
  
  if ((incoming.length() > 300))
  {
    String[] list = split(incoming, "\n");
    if ( list.length == 43 )  // 43 is the moment where the serial input stabilizes
    { 
      //fill the data
      {
        roll[0]  =  float(list[3]);  roll[1]  =  float(list[7]);  roll[2]  =  float(list[11]);
        roll[3]  =  float(list[17]); roll[4]  =  float(list[21]); roll[5]  =  float(list[25]);
        roll[6]  =  float(list[31]); roll[7]  =  float(list[35]); roll[8]  =  float(list[39]);
        
        pitch[0]  =  float(list[4]);  pitch[1]  =  float(list[8]);  pitch[2]  =  float(list[12]);
        pitch[3]  =  float(list[18]); pitch[4]  =  float(list[22]); pitch[5]  =  float(list[26]);
        pitch[6]  =  float(list[32]); pitch[7]  =  float(list[36]); pitch[8]  =  float(list[40]);

        yaw[0]  =  float(list[5]);  yaw[1]  =  float(list[9]);  yaw[2]  =  float(list[13]);
        yaw[3]  =  float(list[19]); yaw[4]  =  float(list[23]); yaw[5]  =  float(list[27]);
        yaw[6]  =  float(list[33]); yaw[7]  =  float(list[37]); yaw[8]  =  float(list[41]);

        bend[0] = float(list[15]); bend[1] = float(list[29]);
      }      

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

