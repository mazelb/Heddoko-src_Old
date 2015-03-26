/**
 * Retrieves channel data from buffer.
 */
public void fetchData() {
  
  // Retrieve channel data.
  ch[0] = int(buffer.substring(20));     // Time in milliseconds
  ch[1] = int(buffer.substring(0, 4));   // Channel 1
  ch[2] = int(buffer.substring(4, 8));   // Channel 2
  ch[3] = int(buffer.substring(8, 12));  // Channel 3
  ch[4] = int(buffer.substring(12, 16)); // Channel 4
  ch[5] = int(buffer.substring(16, 20)); // Channel 5
  
  // Print data to screen.
  fill(50);
  textSize(12);
  text("#1: "+ ch[1] +
        "\t#2: "+ ch[2] +
        "\t#3: "+ ch[3] +
        "\t#4: "+ ch[4] +
        "\t#5: "+ ch[5] +
        "\tTime: "+ ch[0], 20, 580);
  
  // Reset
  isDataReady = false;
}

/**
 * Rotates the cube to given angles.
 * @float x  Angle in radians to rotate about x-axis.
 * @float y  Angle in radians to rotate about y-axis.
 * @float z  Angle in radians to rotate about z-axis.
 */
void cube(float x, float y, float z)
{
  pushMatrix();
    
    // Center drawing start point in screen.
    translate(300, 300, 0); 
    
    // draw stationary axis lines
    strokeWeight(1);
    stroke(50);
    line(-60, 0, 60, 0);
    line(0, 60, 0, -60);
    line(0, 0, -60, 0, 0, 60); 
    
    // Shadow box
    strokeWeight(1);
    stroke(0, 0, 255);
    noFill();
    box(140);
     
    // rotate drawing coordinates according to user input variables
    rotateX(x);
    rotateY(y);
    rotateZ(z);
    
    // draw the rotating axis lines
    strokeWeight(2);
    stroke(255);
    line(-150, 0, 150, 0);
    line(0, 150, 0, -150);
    line(0, 0, -150, 0, 0, 150);
    
    // draw rotating red box
    strokeWeight(2);
    stroke(0, 150, 0);
    noFill();
    box(140);
     
  popMatrix();
}

/**
 * Sets serial port to desired value.
 */
public void setSerialPort(String portName) 
{
  // Close the port if it's currently open.
  if (port != null) {
    port.stop();
  }
  
  // Open port.
  try {
    port = new Serial(this, portName, 115200);
    port.bufferUntil('\n');
        
    // Persist port in configuration.
    saveStrings("SerialConfig.txt", new String[] { portName });
  }
  
  catch (RuntimeException ex) {
    // Swallow error if port can't be opened, keep port closed.
    port = null; 
  }
}

