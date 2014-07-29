using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class UpperBody : MonoBehaviour
{
	// Serial port details
	public string serialPort = "COM3";
	public int baudRate = 9600;
	protected SerialPort sp;

	// Use this for initialization
	void Start ()
	{
		sp = new SerialPort (serialPort, baudRate);
		sp.Open ();
		sp.ReadTimeout = 1;

		// Read from the serial port
		InvokeRepeating ("ReadArduino", 2, 0.5f);
	}

	void ReadArduino()
	{
		if (sp.IsOpen) {
			try {
				Debug.Log (sp.ReadLine ());
			} catch (System.Exception) {}
		}
	}
	
	// Update is called once per frame
	void FixedUpdate ()
	{
		
	}
}
