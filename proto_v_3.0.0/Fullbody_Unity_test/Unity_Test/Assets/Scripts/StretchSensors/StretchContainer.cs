using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class StretchContainer : MonoBehaviour 
{
	//
	public bool usingCOMPort = false;

	public string COMPort;
	public int baudeRate = 115200;
	private SerialPort mPortStream = null;

    // Shortcut to apply data sets to all sensors at the same time.
    public string CSVDataSet = "default";

	// Each joint can be composed of one or multiple sensors simultaneously
	private StretchJoint[] mStretchJoints;

	// Channel data
	public static int[] mData;

	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints () 
	{
		// Open COM port
		if (usingCOMPort && !String.IsNullOrEmpty(COMPort))
		{
			mPortStream = new SerialPort(COMPort, baudeRate);
			mPortStream.DataBits = 8;
			mPortStream.StopBits = StopBits.One;
			if (mPortStream.IsOpen) mPortStream.Close();
			
			// Try to open COM port and send start command
			try {
				mPortStream.Open();
				mPortStream.Write("#s\r\n");
			}
			catch (Exception error) {}
		}

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
		// test
		if (usingCOMPort && !String.IsNullOrEmpty(COMPort)) {
			
			print (mPortStream.ReadLine());
		}

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
