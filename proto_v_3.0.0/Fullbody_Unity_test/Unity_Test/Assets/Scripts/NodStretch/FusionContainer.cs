using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic; 
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;
using System.Threading;

public class FusionContainer : MonoBehaviour 
{
	//each joint can be composed of one or multiple sensors simultaneously
	private static FusionNodJoint[] mNodJoints;

	// This variable is used to specify the angle information of which part of body be shown on the screen
	public static float vKey;

	/**
     * Information to setup COM port.
     */
	public bool vUsingStretchSense = true;
	public string vStretchSensePort;
	public int vBaudRate = 115200;
	public int vReadTimeout = 200;
	public bool vPrintChannelData = false;
	private SerialPort mPortStream = null;

	/**
     * Data from StretchSense module.
     * This is accessible from other scripts.
     */
	public static int[] svaModuleData = new int[6];


	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints () 
	{
		// Set up the port stream if we're going to read from the StretchSense module.
		if (vUsingStretchSense && !String.IsNullOrEmpty(vStretchSensePort))
		{
			StartCOMPOrt();
		}

		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].StartJoint();
			}
		}
	}
	
	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public void UpdateJoints () 
	{
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].UpdateJoint();
			}
		}
	}
	
	/// <summary>
	/// Reset the stretch joint sensors.
	/// </summary>
	public void ResetJoints ()
	{
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].ResetJoint();
			}
		}
	}



	//	/ <summary>
	//	/ Provides the Torso Orientation for other joints 
	//	/ </summary>
	public static float [,] GetTorsoOrientation ()
	{
		return mNodJoints [0].ReturnTorsoOrientation();
	}

	/**
     * @brief           Opens the COM port and sends the "Start" command to the StretchSensr module so we can read it later.
     * @return void
     */
	private void StartCOMPOrt()
	{
		mPortStream = new SerialPort(vStretchSensePort, vBaudRate, Parity.None, 8, StopBits.One);

		if (mPortStream.IsOpen)
		{
			mPortStream.Close();
		}

		// Reduce the read timeout.
		mPortStream.ReadTimeout = vReadTimeout;

		// Try to open COM port and send start command.
		try
		{
			mPortStream.Open();
			mPortStream.Write("#s\r\n");
		}
		catch (Exception e)
		{
			print("Could not open COM port: "+ e.Message);
		}

		// Use threading to read data.
		Thread vReadThread = new Thread(ReadCOMPort);
		vReadThread.Start();
	}

	/**
     * @brief           Reads data from the assigned COM port.
     * @return void
     */
	public void ReadCOMPort()
	{
		// Performance check.
		if (mPortStream == null || !mPortStream.IsOpen)
		{
			return;
		}

		// Update channel data
		while (true)
		{
			string rawData = mPortStream.ReadLine();

			// Read a data line.
			if (rawData.Length >= 21 && rawData[0] == '!')
			{
				svaModuleData[1] = Convert.ToInt32(rawData.Substring(1, 4));
				svaModuleData[2] = Convert.ToInt32(rawData.Substring(5, 4));
				svaModuleData[3] = Convert.ToInt32(rawData.Substring(9, 4));
				svaModuleData[4] = Convert.ToInt32(rawData.Substring(13, 4));
				svaModuleData[5] = Convert.ToInt32(rawData.Substring(17, 4));

				if (vPrintChannelData)
				{
					print(
						" #1: "+ svaModuleData[1] +
						" #2: "+ svaModuleData[2] +
						" #3: "+ svaModuleData[3] +
						" #4: "+ svaModuleData[4] +
						" #5: "+ svaModuleData[5]
					);
				}
			}

			// Read a comment line.
			else if (rawData[0] == '@')
			{
				print(rawData);
			}

			// Anything else will be unusable.
			else
			{
				print("Invalid incoming data.");
			}
		}
	}

	/**
     * @brief           Called once when the application quits.
     * @return void
     */
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
		mNodJoints = GetComponentsInChildren<FusionNodJoint>();
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
		if (GUI.Button (new Rect (20, 70, 200, 50), "Start Sensors"))
		{
			ResetJoints();
			StartJoints();
		}

		if (GUI.Button (new Rect (220, 70, 200, 50), "Reset Sensors "))
		{			
			ResetJoints();        
		}


		if (GUI.Button (new Rect (880, 550, 120 , 25), "Thoracolumbar"))
		{			
			vKey = 1;        
		}

		if (GUI.Button (new Rect (1005, 550, 110 , 25), "Right Arm"))
		{			
			vKey = 2;        
		}


		if (GUI.Button (new Rect (1120, 550, 110 , 25), "Left Arm"))
		{			
			vKey = 3;         
		}


		if (GUI.Button (new Rect (1235, 550, 110 , 25), "Right Leg"))
		{			
			vKey = 4;        
		}

		if (GUI.Button (new Rect (1350, 550, 110 , 25), "Left Leg"))
		{			
			vKey = 5;         
		}


//		string[] names = new string[] {"Matt", "Joanne", "Robert", "kazem"};
//		GUI.SelectionGrid (new Rect (1000, 500, 200, 40), 0, names , 2);
//		//GUI.Label (new Rect (1000, 520, 150, 50), "angle: " + Time.time.ToString ());
//
//
//
//		float progress = Time.time * 0.05f;
//		var progressBarEmpty = new Texture2D(0,0);
//		var progressBarFull = new Texture2D(0,0);
//
//
//		GUI.BeginGroup (new Rect (20, 40, 400, 20));
//		GUI.Box (new Rect (0,0,  400, 20),progressBarEmpty);
//
//		// draw the filled-in part:
//		GUI.BeginGroup (new Rect (200 - (400 * progress), 0, 400 * progress, 20f));
//		GUI.Box (new Rect (0,0,  400, 20),progressBarFull);
//		GUI.EndGroup ();
//
//		GUI.EndGroup ();
//
	}
}
