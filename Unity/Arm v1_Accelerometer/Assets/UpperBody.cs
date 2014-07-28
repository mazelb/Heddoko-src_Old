using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class UpperBody : MonoBehaviour, Move
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
	}
	
	// Update is called once per frame
	void Update ()
	{


		// Serial test
		if (sp.IsOpen)
		{
			try
			{
				Debug.Log(sp.ReadByte ());
			}
			catch (System.Exception) {}
		}
	}
}
