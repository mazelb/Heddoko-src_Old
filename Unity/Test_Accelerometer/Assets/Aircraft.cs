using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class Aircraft : MonoBehaviour
{
	public string serialPort = "COM3";
	public int baudRate = 9600;
	private SerialPort sp;

	// Use this for initialization
	void Start ()
	{
		sp = new SerialPort (serialPort, baudRate);
		sp.Open ();
		sp.ReadTimeout = 1;
	}
	
	// Update is called once per frame
	void Update ()
	{
		// Make sure serial port is open
		if (!sp.IsOpen) {
			return;
		}

		// Try to read bytes
		try
		{
			Debug.Log (sp.ReadByte ());

		} catch(System.Exception) {}
	}
}
