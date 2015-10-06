/**
 * @file	JigTest.cs
 * @brief	This script is used to collect data during tests using the jigs.
 * @note	This should be ported to an independent C# program.
 * @author	Francis Amankrah (frank@heddoko.com)
 * @date 	October 2015
 */
using UnityEngine;
using System;
using System.IO;
using System.IO.Ports;
using System.Diagnostics;
using System.Threading;
using Nod;

//using System.Collections;
//using System.Collections.Generic;
//using System.Runtime.InteropServices;

public class JigTest : MonoBehaviour 
{
	/**
     * Interval at which to collect data, in miliseconds (set to zero to ignore).
     */
	public int mFrameInterval = 100;
	
	/**
     * COM ports.
     */
	public string mEncoderCOMPort;
	public string mFabricSensorsCOMPort;

	/**
	 * IMU IDs.
	 */
	public int mFirstImuId = 0;
	public int mSecondImuId = 1;

	/**
	 * Information about the file name.
	 */
	public string mFileNameAppend = "jig_test_data";
	public string mFileNameExtension = "csv";
	
	/**
     * Boolean indicating if debugging comments should be verbose or not.
     */
	public bool mVerboseDebug = false;
	
	/**
     * Port streams
     */
	private SerialPort mEncoderPortStream = null;
	private SerialPort mFabricSensorsPortStream = null;
	
	/**
     * IMU-related objects.
     */
	private NodController mIMUController;
	private NodRing mFirstImu;
	private NodRing mSecondImu;
	
	/**
     * Collected data.
     */
	private long mTimeStamp = 0;
	private double mEncoderData = 0.0;
	private string mFirstImuData = "";
	private string mSecondImuData = "";
	private int[] maFabricSensorsData = new int[6];
	
	/**
     * Timer used to create timestamps.
     */
	private Stopwatch mStopwatch = null;
	
	/**
     * Filename to write to.
     */
	private string mFileName = "";

	/**
	 * File stream to write to.
	 */
	private TextWriter mFileStream;
	
	/**
     * Boolean indicating if we should keep reading data or not.
     */
	private bool mReading = true;
	
	/**
     * Main.
     */
	public void StartProgram()
	{
		print ("Starting program... 1");

		// Open the encoder COM port.
		if (mEncoderCOMPort.Length > 0)
		{
			print("... Connecting to encoder on port " + mEncoderCOMPort);
			mEncoderPortStream = new SerialPort(mEncoderCOMPort, 115200);
			mEncoderPortStream.ReadTimeout = 200;
			OpenCOMPort(mEncoderPortStream, mEncoderCOMPort);
		}
		
		print ("Starting program... 2");

		// Open Fabric sensors COM port.
		if (mFabricSensorsCOMPort.Length > 0)
		{
			print("... Connecting to fabric sensors on port " + mFabricSensorsCOMPort);
			mFabricSensorsPortStream = new SerialPort(mFabricSensorsCOMPort, 115200, Parity.None, 8, StopBits.One);
			mFabricSensorsPortStream.ReadTimeout = 500;
			OpenCOMPort(mFabricSensorsPortStream, mFabricSensorsCOMPort);
			
			// Send start command to StretchSense Bluetooth module.
			if (mFabricSensorsPortStream.IsOpen)
			{
				try
				{
					mFabricSensorsPortStream.Write("#s\r\n");
				}
				catch (Exception)
				{
					print("Could not send start command to fabric sensors module.");
				}
			}
		}
		
		print ("Starting program... 3");

		// Connect to the IMUs.
		mIMUController = NodController.GetNodInterface();
		int vNumIMUsPaired = mIMUController.getNumDevices();
		if (vNumIMUsPaired > 0)
		{
			// Connect first IMU.
			if (vNumIMUsPaired > mFirstImuId)
			{
				mFirstImu = ConnectIMU(mFirstImuId);
			}

			// Connect second IMU.
			if (vNumIMUsPaired > mSecondImuId)
			{
				mSecondImu = ConnectIMU(mSecondImuId);
			}
		}
		
		print ("Starting program... 4");

		// Make sure we have a directory to write to.
		string vDataPath = "../../JigTestData";
		if (!Directory.Exists(vDataPath))
		{
			Directory.CreateDirectory(vDataPath);
		}
		
		print ("Starting program... 5");

		// Set the file name to write to.
		int vFileNameIncrement = 1;
		mFileName = string.Format("{0}/{1}_{2}.{3}", vDataPath, mFileNameAppend, vFileNameIncrement, mFileNameExtension);
		while (File.Exists(mFileName))
		{
			mFileName = string.Format("{0}/{1}_{2}.{3}", vDataPath, mFileNameAppend, (++vFileNameIncrement), mFileNameExtension);
		}

		print ("Filename: " + mFileName);
		
		// Collect data and write the heading.
		mFileStream = new StreamWriter(mFileName, true);
		mFileStream.WriteLine(
			"Timestamp," +
			"Encoder," +
			"Imu1RawX,Imu1RawY,Imu1RawZ," +
			"Imu2RawX,Imu2RawY,Imu2RawZ," +
			"FabricSensor1,FabricSensor2,FabricSensor3,FabricSensor4,FabricSensor5"
		);

        print("Starting program... 6");

    }

    /**
     * Writes the collected data to file.
     */
    public void RecordDataToFile()
	{
		// Performance check.
		if (mFileStream == null)
		{
			return;
		}

		string vDataLine = "";
		
		// Start with a timestamp.
		vDataLine += mTimeStamp + ",";
		
		// Append the encoder value.
		if (mEncoderPortStream != null && mEncoderPortStream.IsOpen)
		{
			vDataLine += mEncoderData;
		}
		vDataLine += ",";

		// Append first IMU data.
		if (mFirstImu != null)
		{
			vDataLine += mFirstImuData;
		}
		else
		{
			vDataLine += ",,";
		}
		vDataLine += ",";
		
		// Append second IMU data.
		if (mFirstImu != null)
		{
			vDataLine += mSecondImuData;
		}
		else
		{
			vDataLine += ",,";
		}
		vDataLine += ",";
		
		// Append fabric sensor data.
		if (mFabricSensorsPortStream != null && mFabricSensorsPortStream.IsOpen)
		{
			vDataLine +=
				maFabricSensorsData[1] + "," +
					maFabricSensorsData[2] + "," +
					maFabricSensorsData[3] + "," +
					maFabricSensorsData[4] + "," +
					maFabricSensorsData[5];
		}
		else
		{
			vDataLine += ",,,,";
		}
		
		mFileStream.WriteLine(vDataLine);
	}
	
	/**
     * Tries to open a COM port so we can read from it.
     */
	public void OpenCOMPort(SerialPort vPortStream, string vPortName)
	{
		// If port is already open, close it.
		if (vPortStream.IsOpen)
		{
			if (mVerboseDebug)
			{
				print("Closing port: " + vPortName);
			}
			
			vPortStream.Close();
		}
		
		// Try to open COM port.
		try
		{
			if (mVerboseDebug)
			{
				print("Opening port: " + vPortName);
			}

			vPortStream.Open();
		}
		catch (Exception e)
		{
			print("Couldn't open port " + vPortName + " (exception: " + e.Message + ")");
		}
	}
	
	public NodRing ConnectIMU(int vImuId)
	{
		print("... Connecting IMU with ID: " + vImuId);
		
		// Find requested IMU.
		NodRing vIMU = mIMUController.getRing(vImuId);
		
		if (vIMU == null)
		{
			print("Could not find IMU with ID: " + vImuId);
			return vIMU;
		}
		
		// Subscribe the IMU to the relevant services.
		if (vIMU.Subscribe(NodSubscriptionType.Orientation) && vIMU.Subscribe(NodSubscriptionType.Button))
		{
			if (mVerboseDebug)
			{
				print("Successfully connected IMU wth ID: " + vImuId);
			}
		}

		return vIMU;
	}
	
	/**
     * Retrieve data from the encoder.
     *
     * See: Heddoko-src/Testing/stretchSense_encoder/dual_csv.py
     */
	public void ReadEncoder()
	{
		// Performance check.
		if (mEncoderCOMPort.Length == 0 || mEncoderPortStream == null || !mEncoderPortStream.IsOpen)
		{
			return;
		}
		
		// Retrieve data.
		string vRawData = "";
		try
		{
			if (mVerboseDebug)
			{
				print("Reading from encoder...");
			}
			
			vRawData = mEncoderPortStream.ReadLine();
		}
		catch (Exception e)
		{
			if (mVerboseDebug)
			{
				print("Could not read from encoder: " + e.Message);
			}
		}
		
		// Update encoder values.
		if (vRawData.Length > 0)
		{
			mEncoderData = Convert.ToDouble(vRawData);
		}
		
		if (mVerboseDebug)
		{
			print("Encoder raw data: " + vRawData);
		}
	}
	
	/**
     * Retrieves data from the IMUs.
     *
     * See: Heddoko-src/proto_v_3.0.0/Fullbody_Unity_test/Unity_Test/Assets/Scripts/NodSensors/NodSensor.cs
     */
	public void ReadIMUs()
	{
		// Update data for first IMU.
		if (mFirstImu != null)
		{
			mFirstImuData = GetImuEulerAngles(mFirstImu);
		}
		
		// Update data for second IMU.
		if (mSecondImu != null)
		{
			mSecondImuData = GetImuEulerAngles(mSecondImu);
		}
	}

	private string GetImuEulerAngles(NodRing vIMU)
	{
		vIMU.CheckForUpdate();
		
		//Quaternion vRingRotation = vIMU.ringRotation;
		//Vector3 vEulerAngles = vRingRotation.eulerAngles;
		Vector3 vRawEulerAngles = vIMU.ringEulerRotation;

		return vRawEulerAngles.x +","+ vRawEulerAngles.y +","+ vRawEulerAngles.z;
	}
	
	/**
     * Retrieves StretchSense data.
     *
     * See: Heddoko-src/proto_v_3.0.0/Fullbody_Unity_test/Unity_Test/Assets/Scripts/StretchSensors/SSContainer.cs
     */
	public void ReadFabricSensors()
	{
		// Performance check.
		if (mFabricSensorsCOMPort.Length == 0 || mFabricSensorsPortStream == null || !mFabricSensorsPortStream.IsOpen)
		{
			return;
		}
		
		// Retrieve data.
		string vRawData = "";
		try
		{
			if (mVerboseDebug)
			{
				print("Reading from fabric sensors module...");
			}
			
			vRawData = mFabricSensorsPortStream.ReadLine();
		}
		catch (Exception e)
		{
			if (mVerboseDebug)
			{
				print("Could not read from fabric sensors module: " + e.Message);
			}
		}
		
		// Read a data line.
		if (vRawData.Length >= 21 && vRawData[0] == '!')
		{
			if (mVerboseDebug)
			{
				print("Received fabric sensors data.");
			}
			
			maFabricSensorsData[1] = Convert.ToInt32(vRawData.Substring(1, 4));
			maFabricSensorsData[2] = Convert.ToInt32(vRawData.Substring(5, 4));
			maFabricSensorsData[3] = Convert.ToInt32(vRawData.Substring(9, 4));
			maFabricSensorsData[4] = Convert.ToInt32(vRawData.Substring(13, 4));
			maFabricSensorsData[5] = Convert.ToInt32(vRawData.Substring(17, 4));
		}
		
		// Read a comment line.
		else if (vRawData[0] == '@' && mVerboseDebug)
		{
			print("Fabric sensors module comment: " + vRawData);
		}
		
		// Anything else will be unusable.
		else { }
	}
	
	/**
     * Cleans up before exiting program.
     */
	public void Reset()
	{
		// Close the file stream.
		if (mFileStream != null)
		{
            print("Closing file");
			mFileStream.Close();
		}

		// Close COM ports.
		if (mEncoderCOMPort.Length > 0 && mEncoderPortStream.IsOpen)
		{
			mEncoderPortStream.Close();
		}
		if (mFabricSensorsCOMPort.Length > 0 && mFabricSensorsPortStream.IsOpen)
		{
			mFabricSensorsPortStream.Close();
		}
		
		// Unsubscribe from Nod services.
		if (mFirstImu != null)
		{
			mFirstImu.Unsubscribe(NodSubscriptionType.Button);
			mFirstImu.Unsubscribe(NodSubscriptionType.Orientation);
		}
		if (mSecondImu != null)
		{
			mSecondImu.Unsubscribe(NodSubscriptionType.Button);
			mSecondImu.Unsubscribe(NodSubscriptionType.Orientation);
		}

		// Reset the stopwatch.
		mStopwatch = Stopwatch.StartNew();
	}
	
	
	
	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
	//////////////////////////////////////////////////////////////////////////////////////
	
	
	
	/**
     * @brief	This method is called by Unity when the program is launched.
     * @return	void
     */
	void Awake ()
	{
		Application.targetFrameRate = 300;
	}
	
	/**
     * @brief	This method is called by Unity when the program is started.
     * @return 	void
     */
	void Start() 
	{
		Reset();
		StartProgram();
	}
	
	/**
     * @brief	This method is called by Unity once per frame.
     * @return 	void
     */
	void Update() 
	{
		// Respect the indicated frame rate, so that we're not overloaded with data.
		// We'll also try to avoid duplicate lines by checking the timestamp.
		if (mFrameInterval > 0 && Time.frameCount % mFrameInterval != 0)
		{
			return;
		}
		
		// Update the time stamp.
		mTimeStamp = mStopwatch.ElapsedMilliseconds;
		
		// Read data from our devices.
		ReadIMUs();
		ReadFabricSensors();
		ReadEncoder();
		
		// Write the data to file.
		RecordDataToFile();
	}
	
	/**
     * @brief	Sets up the GUI buttons.
     * @return	void
     */
	void OnGUI()
	{
	}
}
