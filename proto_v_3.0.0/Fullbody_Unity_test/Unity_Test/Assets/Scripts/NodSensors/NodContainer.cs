using UnityEngine;
using System;
using System.Diagnostics;
using System.Collections;
using System.Collections.Generic; 
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;
using System.Threading;

public class NodContainer : MonoBehaviour 
{
	//each joint can be composed of one or multiple sensors simultaneously
	private static NodJoint[] mNodJoints;

	// This variable is used to specify the angle information of which part of body be shown on the screen
	public static float vKey;

	//
	// Variables related to fabric sensors.
	//

	public bool mUsingFabricSensors = true;
	public string mFabricSensorsPort;
	public int mFabricSensorsBaudRate = 115200;
	public bool mPrintFabricSensorsData = false;
	private SerialPort mFabricSensorsPortStream = null;
	private Thread mFabricSensorsThread = null;
	private bool mConnectingFabricSensors = false;

	// Data from the fabric sensors module. This is accessible from other scripts.
	public static int[] svaModuleData = new int[6];

	//
	// Variables related to jig encoder.
	//

	public string mEncoderPort;
	public bool mPrintEncoderData = false;
	private SerialPort mEncoderPortStream = null;
	private Thread mEncoderThread = null;
	private bool mConnectingEncoder = false;

	// Jig encoder data. This is accessible from other scripts.
	public static double smEncoderData = 0.0;

	// Boolean indicating whether any joint is recording data or not.
	private bool mRecordData = false;

	// Indicates whether reccording is live or not.
	public static bool mIsRecording = false;
    public static bool Fusion = false;
    public static bool CountMovement = false;
    public static bool Squat = false;
    public static bool StartSquat = false;
    public static bool PushUp = false;
    public static bool StartPushUp = false;
    
    // IMU Controller. Used to count the # of IMUs connected.
    private NodController mIMUController;


	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints() 
	{
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].StartJoint();
			}
		}

		mIMUController = NodController.GetNodInterface();
	}
	
	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public void UpdateJoints() 
	{
		// Update fabric sensors and encoder data. We update the data once per frame instead
		// of using threads, which don't work as expected in Unity.
		ReadFabricSensorsData();
		ReadEncoder();

		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].UpdateJoint();
			}
		}
	}
	
	/**
	 * Reset the stretch joint sensors.
	 */
	public void ResetJoints()
	{
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].ResetJoint();
			}
		}

		// Reset fabric sensors and jig encoder while we're at it.
		mFabricSensorsThread = null;
		mConnectingFabricSensors = false;

		mEncoderThread = null;
		mConnectingEncoder = false;
	}

	/**
	 * Provides the Torso Orientation for other joints.
	 */
	public static float [,] GetTorsoOrientation ()
	{
		return mNodJoints [0].ReturnTorsoOrientation();
	}

	/**
	 * Reads from fabric sensors.
	 */
	public void ReadFabricSensorsData()
	{
		// Performance check.
		if (!mUsingFabricSensors || mFabricSensorsPort.Length < 4 || mConnectingFabricSensors)
		{
			return;
		}

		// Connect to the fabric sensors in a separate thread, so that the program can continue.
		if (mFabricSensorsPortStream == null || !mFabricSensorsPortStream.IsOpen)
		{
			mFabricSensorsThread = new Thread(ConnectFabricSensors);
			mFabricSensorsThread.Start();

			return;
		}
		
		else
		{
			mFabricSensorsThread = null;
		}

		// Try to read from COM port.
		string vRawData = "";
		try
		{
			vRawData = mFabricSensorsPortStream.ReadLine();
		}
		catch (Exception e)
		{
			print("Could not read from fabric sensors: " + e.Message);

			return;
		}

		if (vRawData.Length >= 21 && vRawData[0] == '!')
		{
			svaModuleData[1] = Convert.ToInt32(vRawData.Substring(1, 4));
			svaModuleData[2] = Convert.ToInt32(vRawData.Substring(5, 4));
			svaModuleData[3] = Convert.ToInt32(vRawData.Substring(9, 4));
			svaModuleData[4] = Convert.ToInt32(vRawData.Substring(13, 4));
			svaModuleData[5] = Convert.ToInt32(vRawData.Substring(17, 4));
			
			if (mPrintFabricSensorsData)
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
		else if (vRawData[0] == '@')
		{
			print(vRawData);
		}
		
		// Anything else will be unusable.
		else
		{
			print("Invalid incoming data from fabric sensors.");
		}
	}
	
	/**
     * Retrieve data from the encoder.
     *
     * See: Heddoko-src/Testing/stretchSense_encoder/dual_csv.py
     */
	public void ReadEncoder()
	{
		// Performance check.
		if (!mRecordData || mEncoderPort.Length < 4 || mConnectingEncoder)
		{
			return;
		}
		
		// Connect to encoder in a separate thread, so that the program can continue.
		if (mEncoderPortStream == null || !mEncoderPortStream.IsOpen)
		{
			mEncoderThread = new Thread(ConnectEncoder);
			mEncoderThread.Start();
			
			return;
		}

		else
		{
			mEncoderThread = null;
		}
		
		// Try to read from COM port.
		string vRawData = "";
		try
		{
			vRawData = mEncoderPortStream.ReadLine();
		}
		catch (Exception e)
		{
			print("Could not read from encoder: " + e.Message);
			
			// Set encoder value to zero and return.
			smEncoderData = 0.0;
			return;
		}
		
		// Update encoder values.
		if (vRawData.Length > 0)
		{
			smEncoderData = Convert.ToDouble(vRawData);

			if (mPrintEncoderData)
			{
				print("Encoder raw data: " + vRawData);
			}
		}

		// Close the port for this frame, since flushing the serial port doesn't seem to work in Unity.
		mEncoderPortStream.Close();
	}

	/**
	 * Connects to fabric sensors.
	 */
	public void ConnectFabricSensors()
	{
		// Alert the program that we are trying to connect to the fabric sensors.
		mConnectingFabricSensors = true;
		print ("Connecting to fabric sensors on port "+ mFabricSensorsPort +"...");

		mFabricSensorsPortStream = new SerialPort(mFabricSensorsPort, mFabricSensorsBaudRate, Parity.None, 8, StopBits.One);
		
		if (mFabricSensorsPortStream.IsOpen)
		{
			mFabricSensorsPortStream.Close();
		}
		
		// Reduce the read timeout.
		//mFabricSensorsPortStream.ReadTimeout = 200;
		
		// Try to open COM port and send start command.
		try
		{
			mFabricSensorsPortStream.Open();
			mFabricSensorsPortStream.Write("#s\r\n");
		}
		catch (Exception e)
		{
			print("Could not connect to fabric sensors on port "+ mFabricSensorsPort +": "+ e.Message);
		}

		mConnectingFabricSensors = false;
	}
	
	/**
	 * Connects to encoder.
	 */
	public void ConnectEncoder()
	{
		// Alert the program that we are trying to connect to the encoder.
		mConnectingEncoder = true;
		print ("Connecting to encoder on port " + mEncoderPort);

		mEncoderPortStream = new SerialPort (mEncoderPort, 115200);
		mEncoderPortStream.ReadTimeout = 200;
		
		if (mEncoderPortStream.IsOpen)
		{
			mEncoderPortStream.Close();
		}
		
		try
		{
			mEncoderPortStream.Open();
			
			// Discard buffer.
			mEncoderPortStream.DiscardInBuffer();
			mEncoderPortStream.BaseStream.Flush();
		}
		catch (Exception e)
		{
			print ("Could not connect to encoder on port " + mEncoderPort + ": " + e.Message);
		}

		mConnectingEncoder = false;
	}
	
	
	
	/////////////////////////////////////////////////////////////////////////////////////
	/// 
	/// 
	/// 
	/// UNITY GENERATED FUNCTIONS 
	/// 
	/// 
	/// 
	//////////////////////////////////////////////////////////////////////////////////////



	/// <summary>
	/// Awake this instance.
	/// </summary>
	void Awake()
	{
		//Application.targetFrameRate = 300;
		mNodJoints = GetComponentsInChildren<NodJoint>();

		// Check joints to see if we'll be recording data and potentially needing the jig encoder.
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++)
		{
			if (mNodJoints[ndx].mRecordData == true)
			{
				mRecordData = true;
				//mEncoderThread = new Thread(ReadEncoder);
				//mEncoderThread.Start ();

				break;
			}
		}
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
        float buttonWidth = 200f;
        float buttonHeight = 50f;
        float sideV = 20f;
        float gapH = buttonHeight + 20f;
        float gapV = buttonWidth + 20f;

        //
        // Sensor start/stop buttons.
        //
        if (GUI.Button (new Rect (sideV, Screen.height * 0.01f, buttonWidth, buttonHeight), "Start Sensors"))
		{
			ResetJoints();
			StartJoints();
		}

		if (GUI.Button (new Rect (sideV + buttonWidth, Screen.height * 0.01f, buttonWidth, buttonHeight), "Reset Sensors "))
		{			
			ResetJoints();
        }

        //
        //for counting special moves
        //

        if ( GUI.Button(new Rect(sideV, Screen.height * 0.4f, buttonWidth * 1.5f, buttonHeight), "Count Movements "))
        {
            CountMovement = true;
        }

        if (CountMovement)
        {
            GUIStyle vRecStyle4 = new GUIStyle(GUI.skin.button);
            vRecStyle4.fontSize = (int)(13.0f);
            vRecStyle4.fontStyle = FontStyle.Bold;
            if (GUI.Button(new Rect(sideV, Screen.height * 0.4f + 60, buttonWidth * 0.5f, buttonHeight), "# Squat: " + NodJointLegRight.nSquatRight, vRecStyle4)) 
            {
                Squat = true;
            }

            if (GUI.Button(new Rect(sideV + buttonWidth * 0.5f, Screen.height * 0.4f + 60, buttonWidth * 0.5f, buttonHeight), "# Push Up: ", vRecStyle4))
            {
                PushUp = true;
            }

        }


        if (PushUp)
        {
            Squat = false;
            StartSquat = false;

            GUIStyle vRecStyle3 = new GUIStyle(GUI.skin.button);
            vRecStyle3.normal.textColor = Color.white;

            if (GUI.Button(new Rect(sideV + buttonWidth * 0.5f, Screen.height * 0.4f + 120, buttonWidth*0.5f, buttonHeight*0.5f), "Start ", vRecStyle3))
            {
                StartPushUp = true;
                StartSquat = false;
            }
        }

        if (StartPushUp)
        {
            GUIStyle vRecStyle4 = new GUIStyle(GUI.skin.button);
            vRecStyle4.normal.textColor = Color.white;
            if (GUI.Button(new Rect(sideV + buttonWidth * 0.5f, Screen.height * 0.4f + 145, buttonWidth*0.5f, buttonHeight * 0.5f), "End ", vRecStyle4))
            {
                StartPushUp = false;
                PushUp = false;
            }
        }

        if (Squat)
        {
            PushUp = false;
            StartPushUp = false;
            GUIStyle vRecStyle3 = new GUIStyle(GUI.skin.button);
            vRecStyle3.normal.textColor = Color.white;

            if (GUI.Button(new Rect(sideV, Screen.height * 0.4f + 120, buttonWidth*0.5f, buttonHeight * 0.5f), "Start ", vRecStyle3))
            {
                StartSquat = true;
                StartPushUp = false;             
            }
        }

        if (StartSquat)
        {
            GUIStyle vRecStyle4 = new GUIStyle(GUI.skin.button);
            vRecStyle4.normal.textColor = Color.white;         

            if (GUI.Button(new Rect(sideV, Screen.height * 0.4f + 145, buttonWidth*0.5f, buttonHeight * 0.5f), "End ", vRecStyle4))
            {
                StartSquat = false;
                Squat = false;
            }
        }

        //
        //Enable and disable joints fusion
        //
        if(!Fusion)
        {
            GUIStyle vRecStyle1 = new GUIStyle(GUI.skin.button);
            vRecStyle1.normal.textColor = Color.white;
            if (GUI.Button(new Rect(sideV + buttonWidth, Screen.height * 0.01f + buttonHeight, buttonWidth, buttonHeight), "Enable Fusion ", vRecStyle1))
            {
                Fusion = true;
            }
        }
        else
        {
            // Make Disable Fusion button red.
            GUIStyle vRecStyle2 = new GUIStyle(GUI.skin.button);
            vRecStyle2.normal.textColor = Color.red;

            if (GUI.Button(new Rect(sideV + buttonWidth, Screen.height * 0.01f + buttonHeight, buttonWidth, buttonHeight), "Disable Fusion ", vRecStyle2))
            {
                Fusion = false;

            }      
        }


        //
        // Data recording buttons.
        //
        if (mRecordData && !mIsRecording && GUI.Button(new Rect(sideV, Screen.height * 0.01f + buttonHeight, buttonWidth, buttonHeight), "Start Recording"))
		{
			for (int ndx = 0; ndx < mNodJoints.Length; ndx++)
			{
				mNodJoints[ndx].StartRecording();
			}

			mIsRecording = true;
		}

		if (mRecordData && mIsRecording)
		{
			// Make "Stop Recording" button red.
			GUIStyle vRecStyle = new GUIStyle(GUI.skin.button);
			vRecStyle.normal.textColor = Color.red;

			if (GUI.Button(new Rect(sideV, Screen.height * 0.01f + buttonHeight, buttonWidth, buttonHeight), "Stop Recording", vRecStyle))
			{
				for (int ndx = 0; ndx < mNodJoints.Length; ndx++)
				{
					mNodJoints[ndx].StopRecording();
				}
				
				mIsRecording = false;
			}

			// Prepare labels for each sensor.
			GUIStyle vLabelStyle = new GUIStyle(GUI.skin.box);
			vLabelStyle.normal.textColor = Color.black;
			vLabelStyle.alignment = TextAnchor.MiddleLeft;
			vLabelStyle.normal.background = new Texture2D(300, 25);

			// IMU label.
			int vNumIMUs = mIMUController.getNumDevices();
			GUI.Label (new Rect (sideV, Screen.height * 0.01f + 2 * buttonHeight, buttonWidth*2f, buttonHeight*0.5f), "# of IMUs connected: " + vNumIMUs, vLabelStyle);

			// Fabric sensor label.
			string vFabricSensorsLabel = "";
			if (!mUsingFabricSensors) {
				vFabricSensorsLabel = "Not in use.";
			} else if (!mFabricSensorsPortStream.IsOpen) {
				vFabricSensorsLabel = "Connecting...";
			} else {
				vFabricSensorsLabel = "Connected.";
			}

			GUI.Label (new Rect (sideV, Screen.height * 0.01f + buttonHeight * 2.5f, buttonWidth * 2f, buttonHeight * 0.5f), "Fabric sensors: " + vFabricSensorsLabel, vLabelStyle);

			// Encoder label.
			GUI.Label (new Rect (sideV, Screen.height * 0.01f + buttonHeight * 3f, buttonWidth * 2f, buttonHeight * 0.5f), "Encoder angle: " + smEncoderData, vLabelStyle);
		}

		//
		// Joint buttons.
		//
		
		if (GUI.Button (new Rect (Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f, buttonWidth, buttonHeight * 0.5f), "Thoracolumbar"))
		{			
			vKey = 1;        
		}

		if (GUI.Button (new Rect (Screen.width - sideV - 4 * buttonWidth, Screen.height * 0.9f, buttonWidth, buttonHeight * 0.5f), "Right Arm"))
		{			
			vKey = 2;        
		}


		if (GUI.Button (new Rect (Screen.width - sideV - 3 * buttonWidth, Screen.height * 0.9f, buttonWidth, buttonHeight * 0.5f), "Left Arm"))
		{			
			vKey = 3;         
		}


		if (GUI.Button (new Rect (Screen.width - sideV - 2 * buttonWidth, Screen.height * 0.9f, buttonWidth, buttonHeight * 0.5f), "Right Leg"))
		{			
			vKey = 4;        
		}

		if (GUI.Button (new Rect (Screen.width - sideV - buttonWidth, Screen.height * 0.9f, buttonWidth, buttonHeight * 0.5f), "Left Leg"))
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
	
	/// 
	/// Called once when the application quits.
	/// 
	public void OnApplicationQuit()
	{
		mFabricSensorsThread = null;

		// Close fabric sensors port stream.
		if (mFabricSensorsPortStream != null)
		{
			mFabricSensorsPortStream.Close();
		}

		// Close encoder port stream.
		if (mEncoderPortStream != null)
		{
			mEncoderPortStream.Close();
		}
	}
}
