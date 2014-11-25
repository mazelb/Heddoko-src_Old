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
  
  Serial.println("xxxxxxxxxxxxxxxxxx Start xxxxxxxxxxxxxxxxxx");
}

void loop()
{
  int sensorReading = analogRead(analogPin); 
  int mapped = map(sensorReading, 880, 950, 0, 100);

  //Serial.print(sensorReading);
  Serial.print(sensorReading);
  Serial.print("\t");
  
  // Graph
  int graph = sensorReading / 5;
  graph = mapped * 2;
  while (graph > 0)  {
    Serial.print("=");
    graph--;
  }
  
  // Timer
  //Serial.print("\t");
  //msec = (int)((millis() - mstart) / 1000);
  //Serial.print(msec);
  Serial.print("\n");


  // Mapping
  //int mapped = map(sensorReading, 800, 1000, 0, 100);
  //Serial.println(mapped);
  
  // Delay (important to not burn chips)
  delay(50);
}
