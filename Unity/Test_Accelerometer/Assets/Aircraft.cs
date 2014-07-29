using UnityEngine;
using System.Collections;
using System.IO.Ports;
using System;

public class Aircraft : MonoBehaviour
{
	// Serial stuff
	public string serialPort = "COM3";
	public int baudRate = 9600;
	private SerialPort sp;

	// Accelerometer data
	public int[] acc = {0, 0, 0};

	// Rotational position
	public int[] currentPosition = {0, 0, 0};

	// Use this for initialization
	void Start ()
	{
		sp = new SerialPort (serialPort, baudRate);
		sp.Open ();
		sp.ReadTimeout = 1;
		
		// Read from the serial port
		InvokeRepeating ("ReadSerial", 2, 0.5f);
	}
	
	void ReadSerial()
	{
		// Make sure the port is open
		if (sp.IsOpen)
		{
			// Try to read from the port
			try
			{
				string[] data = sp.ReadLine().Split('/');

				// Check amount if information
				// Should get a string for the header, ax, ay, az, and the footer
				if (data.Length != 5) {
					Debug.Log ("Failed length check");
					Debug.Log (data.Length);
					return;
				}

				// Check data header
				if (String.IsNullOrEmpty(data[0]) || data[0] != "xyz") {
					Debug.Log ("Failed header check");
					Debug.Log (data[0]);
					return;
				}

				// Check data footer
				if (String.IsNullOrEmpty(data[4]) || data[4] != "end") {
					Debug.Log ("Failed footer check");
					Debug.Log (data[4]);
					return;
				}

				// Update the position
				Debug.Log ("ok");
				acc[0] = int.Parse (data[1]);
				acc[1] = int.Parse (data[2]);
				acc[2] = int.Parse (data[3]);
				UpdatePosition();
			}
			catch (System.Exception) {}
		}
	}

	void UpdatePosition()
	{
		// To do: apply a low-pass filter to accelerometer data
		int ax = acc [0];
		int ay = acc [1];
		int az = acc [2];

		// Initialize stuff
		int[] delta = {0, 0, 0};
		int[] moveTo = {0, 0, 0};
		
		// Pitch (about x-axis)
		moveTo[0]	= (int)(Mathf.Atan ( ax / Mathf.Sqrt(ay*ay + az*az) ) * Mathf.Rad2Deg);
		
		// Roll (about y-axis)
		moveTo[1]	= (int)(Mathf.Atan ( ay / Mathf.Sqrt(ax*ax + az*az) ) * Mathf.Rad2Deg);
		
		// Yaw (about z-axis)
		moveTo[2]	= (int)(Mathf.Atan ( Mathf.Sqrt(ax*ax + ay*ay) / az ) * Mathf.Rad2Deg);

		// More calculations
		delta [0] = moveTo [0] - currentPosition [0];
		delta [1] = moveTo [1] - currentPosition [1];
		delta [2] = moveTo [2] - currentPosition [2];

		// Rotate
		transform.Rotate (Vector3.left, delta [0]);
		transform.Rotate (Vector3.forward, delta [1]);
		transform.Rotate (Vector3.up, delta [2]);

		// Update position
		currentPosition [0] = moveTo [0];
		currentPosition [1] = moveTo [1];
		currentPosition [2] = moveTo [2];
	}
	
	// Update is called once per frame
	void Update ()
	{

	}
}
