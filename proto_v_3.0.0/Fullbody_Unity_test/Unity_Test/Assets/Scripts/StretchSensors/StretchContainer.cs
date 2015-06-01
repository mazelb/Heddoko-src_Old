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
	public bool usingCOMPort = false;

	public string COMPort;
	public int baudeRate = 115200;
	private SerialPort mPortStream = null;

    // Shortcut to apply CSV data sets to all sensors at the same time.
    public string CSVDataSet = "default";

	// Each joint can be composed of one or multiple sensors simultaneously
	private StretchJoint[] mStretchJoints;

	// Channel data
	public static int[] mData;
	private char[] dataBuffer = new char[21];

	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints () 
	{
		// Open COM port
		if (usingCOMPort && !String.IsNullOrEmpty(COMPort))
		{
		    print("Using COM port: "+ COMPort);
			mPortStream = new SerialPort(COMPort, baudeRate);
			if (mPortStream.IsOpen) mPortStream.Close();
			mPortStream.DataBits = 8;
			mPortStream.StopBits = StopBits.One;
			
			// Try to open COM port and send start command
			try {
				mPortStream.Open();
				mPortStream.Write("#s\r\n");
			}
			catch (Exception) {}
			print(mPortStream.IsOpen ? "Successfully connected." : "Could not open COM port.");

		    print("Testing COM port.");
		    mPortStream.Write("Testing COM port.\r\n");
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

	public void ReadCOMPort()
	{
		// Update channel data
		if (mPortStream.IsOpen)
		{
		    print("Reading line...");
		    string rawData = mPortStream.ReadLine();
//		    string rawData = mPortStream.ReadTo("\r");
//		    Int32 num = mPortStream.Read(dataBuffer, 0, 21);
//		    string rawData = new string(dataBuffer);
			print (rawData);
			if (rawData.Length >= 21 && rawData[0] == '!')
			{
			    mData[1] = Convert.ToInt32(rawData.Substring(1, 4));
			    mData[2] = Convert.ToInt32(rawData.Substring(5, 4));
			    mData[3] = Convert.ToInt32(rawData.Substring(9, 4));
			    mData[4] = Convert.ToInt32(rawData.Substring(13, 4));
			    mData[5] = Convert.ToInt32(rawData.Substring(17, 4));
			}
			else {
			    print("Invalid data.");
			}
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
	    if (usingCOMPort) {
	        ReadCOMPort();
	    }

		UpdateJoints();
	}
	
	/// <summary>
	/// GUI Update.
	/// </summary>
	void OnGUI()
	{
		if (GUI.Button (new Rect (20, 20, 200, 50), "Start StretchSensors"))
		{
			ResetJoints();
			StartJoints();
		}

		if (GUI.Button (new Rect (220, 20, 200, 50), "Reset StretchSensors "))
		{			
			ResetJoints();        
		}
	}
}
