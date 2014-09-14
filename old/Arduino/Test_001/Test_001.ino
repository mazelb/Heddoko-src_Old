/*
 * Heddoko stuff
 *
 */

// Analog pin for voltage divider
int analogPin = A0;
int mstart, msec;

void setup()
{
  // Setup serial
  Serial.begin(9600);
  
  // Setup timer
  mstart = millis();
  
  Serial.println("Start ===========");
}

void loop()
{
  int sensorReading = analogRead(analogPin); 

  Serial.print(sensorReading);
  
  // Timer
  //Serial.print("\t");
  //msec = (int)((millis() - mstart) / 1000);
  //Serial.print(msec);
  Serial.print("\n");


  // Mapping
  //int mapped = map(sensorReading, 800, 1000, 0, 100);
  //Serial.println(mapped);
  
  // Delay (important to not burn chips)
  delay(250);
}
