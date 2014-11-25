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
	private float[] acc = {0, 0, 0};
	private float[] accUpdated = {0, 0, 0};

	// Rotational position
	private float[] currPos = {0, 0, 0};
	private bool newPosReceived = false;

	// Use this for initialization
	void Start ()
	{
		sp = new SerialPort (serialPort, baudRate);
		sp.Open ();
		sp.ReadTimeout = 20;
		
		// Read from the serial port
		InvokeRepeating ("ReceiveData", 1, 0.05f);
	}
	
	// Update is called once per frame
	void Update ()
	{
		// Update position
		if (newPosReceived) {
			UpdatePosition();
		}
	}

	void ReceiveData()
	{
		// Make sure the port is open
		if (!sp.IsOpen) {
			Debug.Log ("Serial port not open");
			return;
		}

		// Try to read from the port
		try
		{
			string[] data = sp.ReadLine().Split('/');

			// Check number of data bits we've received
			// Expecting 5 strings: header, ax, ay, az, and the footer
			if (data.Length != 5) {
				Debug.Log ("Failed length check");
				Debug.Log (data.Length);
				return;
			}

			// Check data header
			// Expecting "xyz"
			if (String.IsNullOrEmpty(data[0]) || data[0] != "xyz") {
				Debug.Log ("Failed header check");
				Debug.Log (data[0]);
				return;
			}

			// Check data footer
			// Expecting "end"
			if (String.IsNullOrEmpty(data[4]) || data[4] != "end") {
				Debug.Log ("Failed footer check");
				Debug.Log (data[4]);
				return;
			}

			// Update the position
			accUpdated[0] = int.Parse (data[1]);
			accUpdated[1] = int.Parse (data[2]);
			accUpdated[2] = int.Parse (data[3]);
			Debug.Log ("Data received :)");
			Debug.Log ("To do: a real checksum would be safer for data integrity");
			newPosReceived = true;
		}

		catch (System.Exception) {}
	}

	void UpdatePosition()
	{
		Debug.Log ("Updating position...");

		// Apply a low-pass filter to accelerometer data
		// NewValue = Prev + α(Raw – Prev) = Prev(1 - α) + αRaw
		float alpha	= 0.7f;
		float ax = acc [0] + alpha * (accUpdated[0] - acc [0]);
		float ay = acc [1] + alpha * (accUpdated[1] - acc [1]);
		float az = acc [2] + alpha * (accUpdated[2] - acc [2]);
		/*float ax = acc [0];
		float ay = acc [1];
		float az = acc [2];*/

		// Initialize stuff
		float[] delta = {0, 0, 0};
		float[] moveTo = {0, 0, 0};
		
		// Pitch (about x-axis)
		moveTo[0] = Mathf.Atan ( ax / Mathf.Sqrt(ay*ay + az*az) ) * Mathf.Rad2Deg;
		
		// Roll (about y-axis)
		moveTo[1] = Mathf.Atan ( ay / Mathf.Sqrt(ax*ax + az*az) ) * Mathf.Rad2Deg;
		
		// Yaw (about z-axis)
		moveTo[2] = Mathf.Atan ( Mathf.Sqrt(ax*ax + ay*ay) / az ) * Mathf.Rad2Deg;

		// More calculations
		delta [0] = moveTo [0] - currPos [0];
		delta [1] = moveTo [1] - currPos [1];
		delta [2] = moveTo [2] - currPos [2];

		// Rotate
		transform.Rotate (Vector3.left, delta [0]);
		transform.Rotate (Vector3.forward, delta [1]);
		transform.Rotate (Vector3.up, delta [2]);

		// Update position data for next movement
		currPos [0] = moveTo [0];
		currPos [1] = moveTo [1];
		currPos [2] = moveTo [2];
		acc [0] = ax;
		acc [1] = ay;
		acc [2] = az;

		Debug.Log ("Position updated to: "+ moveTo [0] +", "+ moveTo [1] +", "+ moveTo [2]);
	}
}
