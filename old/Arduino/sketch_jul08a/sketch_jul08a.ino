int flexSensorPin = A0; //analog pin 0
int numOfBars = 0;

void setup(){
  Serial.begin(9600);
}

void loop()
{
  int flexSensorReading = analogRead(flexSensorPin);
  int mappedValue = map(flexSensorReading, 50, 60, 0, 100);
  
  //Serial.println(flexSensorReading);
  Serial.println(constrain(mappedValue, 0, 100));
  
  // Or just print the raw value
  //Serial.println(flexSensorReading);
  
  delay(30); //just here to slow down the output for easier reading
}
