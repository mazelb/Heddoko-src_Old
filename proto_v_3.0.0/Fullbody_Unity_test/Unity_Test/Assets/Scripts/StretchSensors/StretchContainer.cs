using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;
using System.Threading;

public class StretchContainer : MonoBehaviour 
{
	//
	// Information to setup COM port.
	//
	public bool usingCOMPort = false;
	public bool printChannelData = true;
	public string COMPort;
	public int COMBaudRate = 115200;
	public int COMReadTimeout = 200;
	private SerialPort mPortStream = null;

    // Shortcut to apply CSV data sets to all sensors at the same time.
    public string CSVDataSet = "default";

	// Each joint can be composed of one or multiple sensors simultaneously
	private StretchJoint[] mStretchJoints;

	// Data from StretchSense module.
	public static int[] moduleData = new int[6];
	int olddata;
	int newdata;

	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints () 
	{
		// Set up the port stream if we're going to need it.
		if (usingCOMPort && !String.IsNullOrEmpty(COMPort)) {
			StartCOMPort();
		}

		// Loop through joint scripts to initialize them.
		for (int ndx = 0; ndx < mStretchJoints.Length; ndx++) 
		{
			if(!mStretchJoints[ndx].independentUpdate)
			{
				// The CSV data set allows us to set specific files (by folder) to all sensors.
				mStretchJoints[ndx].StartJoint(CSVDataSet);
			}
		}
	}
	
	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public void UpdateJoints () 
	{
		for (int ndx = 0; ndx < mStretchJoints.Length; ndx++) 
		{
			if(!mStretchJoints[ndx].independentUpdate)
			{
				mStretchJoints[ndx].UpdateJoint();
			}
		}
	}
	
	/// <summary>
	/// Reset the stretch joint sensors.
	/// </summary>
	public void ResetJoints ()
	{
		for (int ndx = 0; ndx < mStretchJoints.Length; ndx++) 
		{
			if(!mStretchJoints[ndx].independentUpdate)
			{
				mStretchJoints[ndx].ResetJoint();
			}
		}
	}

	private void StartCOMPort()
	{
		print("Using COM port: "+ COMPort);
		mPortStream = new SerialPort(COMPort, COMBaudRate, Parity.None, 8, StopBits.One);
		if (mPortStream.IsOpen) {
			mPortStream.Close();
		}
		
		// Reduce the read timeout.
		mPortStream.ReadTimeout = COMReadTimeout;
		
		// Try to open COM port and send start command.
		try {
			mPortStream.Open();
			mPortStream.Write("#s\r\n");
		}
		catch (Exception e) {
			print("Could not open COM port: "+ e.Message);
		}
		
		// Use threading to read data.
		Thread readThread = new Thread(ReadCOMPort);
		readThread.Start();
	}

	public void ReadCOMPort()
	{
		// Update channel data
		if (mPortStream != null && mPortStream.IsOpen)
		{
			while (true)
			{
				string rawData = mPortStream.ReadLine();

				// Read a data line.
				if (rawData.Length >= 21 && rawData[0] == '!')
				{
					moduleData[1] = Convert.ToInt32(rawData.Substring(1, 4));
					moduleData[2] = Convert.ToInt32(rawData.Substring(5, 4));
					moduleData[3] = Convert.ToInt32(rawData.Substring(9, 4));
					moduleData[4] = Convert.ToInt32(rawData.Substring(13, 4));
					moduleData[5] = Convert.ToInt32(rawData.Substring(17, 4));

					if (printChannelData)
					{
						print(
							" #1: "+ moduleData[1] +
							" #2: "+ moduleData[2] +
							" #3: "+ moduleData[3] +
							" #4: "+ moduleData[4] +
							" #5: "+ moduleData[5]
						);
					}
				}

				// Read a comment line.
				else if (rawData[0] == '@')
				{
				    print(rawData);
				}

				// Anything else will be unusable.
				else {
					print("Invalid incoming data.");
				}
			}
		}
	}

	//
	// Close COM port.
	//
	public void OnApplicationQuit()
	{
		if (mPortStream != null)
		{
			mPortStream.Close();
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
	//////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Awake this instance.
	/// </summary>
	void Awake ()
	{
		Application.targetFrameRate = 300;
		mStretchJoints = GetComponentsInChildren<StretchJoint>();
	}
	
	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start() 
	{
		ResetJoints();
		StartJoints();
	}
	
	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update() 
	{
		UpdateJoints();
	}
	
	/// <summary>
	/// GUI Update.
	/// </summary>
	void OnGUI()
	{
		if (GUI.Button (new Rect (20, 20, 150, 30), "Start Sensors"))
		{
			ResetJoints();
			StartJoints();
		}

		if (GUI.Button (new Rect (20, 52, 150, 30), "Reset Sensors "))
		{
			ResetJoints();        
		}
	}
}
