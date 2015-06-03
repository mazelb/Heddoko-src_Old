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

	// Data from StretchSense module.
	public static int[] moduleData = new int[6];

	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints () 
	{
		// Open COM port
		if (usingCOMPort && !String.IsNullOrEmpty(COMPort))
		{
		    print("Opening COM port: "+ COMPort);
			mPortStream = new SerialPort(COMPort, baudeRate);
			if (mPortStream.IsOpen) mPortStream.Close();
			mPortStream.DataBits = 8;
			mPortStream.StopBits = StopBits.One;
			
			// Try to open COM port and send start command
			try {
				mPortStream.Open();
				mPortStream.Write("#s\r\n");
				print("Successfully connected.");
			}
			catch (Exception e) {
			    print("Could not open COM port: "+ e.Message);
			}
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
		if (mPortStream != null && mPortStream.IsOpen)
		{
			// TODO: how to clean buffer in C#? Is this necessary?
			mPortStream.DiscardInBuffer ();
			mPortStream.DiscardOutBuffer ();
			
		    string rawData = mPortStream.ReadLine();
			print (rawData);
			if (rawData.Length >= 21 && rawData[0] == '!')
			{
			    moduleData[1] = Convert.ToInt32(rawData.Substring(1, 4));
			    moduleData[2] = Convert.ToInt32(rawData.Substring(5, 4));
			    moduleData[3] = Convert.ToInt32(rawData.Substring(9, 4));
			    moduleData[4] = Convert.ToInt32(rawData.Substring(13, 4));
			    moduleData[5] = Convert.ToInt32(rawData.Substring(17, 4));

			    print(
			        " #1: "+ moduleData[1] +
			        " #2: "+ moduleData[2] +
			        " #3: "+ moduleData[3] +
			        " #4: "+ moduleData[4] +
			        " #5: "+ moduleData[5]
			    );
			}
			else {
			    print("Invalid incoming data.");
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
