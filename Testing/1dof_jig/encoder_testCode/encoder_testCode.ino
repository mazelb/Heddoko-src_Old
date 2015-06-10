 // Read in 12-bits Magnetic Encoder AEAT-6012-A06  into Arduino Uno
// RobinL

// Declarate

const int CSn = 2; // Chip select
const int CLK = 8; // Clock signal
const int DO = 9; // Digital Output from the encoder which delivers me a 0 or 1, depending on the bar angle..

unsigned int sensorWaarde = 0;

void setup(){

	//Fix de tris
        Serial.begin(115200);
	pinMode(CSn, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(DO, INPUT);

	//Let's start here
	digitalWrite(CLK, HIGH);
	digitalWrite(CSn, HIGH);
}



void loop() {

	sensorWaarde = readSensor();
	delayMicroseconds(1); //Tcs waiting for another read in
}

unsigned int readSensor(){
	unsigned int dataOut = 0;

	digitalWrite(CSn, LOW);
	delayMicroseconds(1); //Waiting for Tclkfe

	//Passing 12 times, from 0 to 9
	for(int x=0; x<12; x++){
		digitalWrite(CLK, LOW); 
		delayMicroseconds(1); //Tclk/2
		digitalWrite(CLK, HIGH);
		delayMicroseconds(1); //Tdo valid, like Tclk/2
		dataOut = (dataOut << 1) | digitalRead(DO); //shift all the entering data to the left and past the pin state to it. 1e bit is MSB
	}

	digitalWrite(CSn, HIGH); // 
        dataOut=dataOut+42;
        if(dataOut>4095){
          dataOut=dataOut-4096;}
          float ratio=4096/360;
          float datafloat=float(dataOut);
          datafloat=datafloat/ratio;
          dataOut=int(datafloat);
        //if(dataOut<55) dataOut=55;
        //dataOut=map(dataOut,55,1965,0,150);
       Serial.println(dataOut);
	return dataOut;

}
