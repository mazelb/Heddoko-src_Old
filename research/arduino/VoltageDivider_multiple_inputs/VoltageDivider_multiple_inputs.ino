/*
 * Heddoko stuff
 *
 */

// Analog pin for voltage divider
int probeOne = A0;
int probeTwo = A2;
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
  int readingOne = analogRead(probeOne); 
  int readingTwo = analogRead(probeTwo);
  
  // Probe one
  Serial.print(readingOne);
  Serial.print("\t");
  Serial.print(readingTwo);
  
  // Graph
  /*int graph = map(readingOne, 810, 920, 0, 125);
  while (graph > 0)  {
    Serial.print("=");
    graph--;
  }*/
  
  // Probe two
  /*Serial.print("\n");
  Serial.print(readingTwo);
  Serial.print("\t");*/
  
  // Graph
  /*graph = map(readingTwo, 810, 920, 0, 125);
  while (graph > 0)  {
    Serial.print("+");
    graph--;
  }*/
  
  // Timer
  //Serial.print("\t");
  //msec = (int)((millis() - mstart) / 1000);
  //Serial.print(msec);
  Serial.print("\n");

  // Delay (important to not burn chips)
  delay(10);
}
