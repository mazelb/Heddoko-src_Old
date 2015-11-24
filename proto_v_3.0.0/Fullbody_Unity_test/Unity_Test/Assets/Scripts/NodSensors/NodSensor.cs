using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;
using Nod;
//using System.Diagnostics;
using System.IO;

public class NodSensor : MonoBehaviour 
{
	public int nodID = 0;
	public string nodName = "nodsensor9999";

	//When True the nod sensor updates independantly
	//otherwise the sensor only updates when the nodJoint asks
	public Boolean independantUpdate = false; 
	
	//Nod connection control
	private NodController mNodController;
    private NodDevice mNodSensor = null;
	private bool mIsNodConnected = false;
	private bool mIsStartConnection = false;

	//Nod data
	public Quaternion initRotation = Quaternion.identity;
	public Quaternion curRotation = Quaternion.identity;
	public Vector3 initRotationEuler = Vector3.zero;
	public Vector3 curRotationEuler = Vector3.zero;
	public Vector3 curRotationRawEuler = Vector3.zero;
    public Vector3 initAcceleration = Vector3.zero;
    public Vector3 currentAcceleration = Vector3.zero;
    public Vector3 initGyro = Vector3.zero; 
    public Vector3 currentGyro = Vector3.zero;

	/// <summary>
	/// Reset the sensors data to init status.
	/// </summary>
	public void Reset() 
	{
		if (null == mNodSensor)
			return;

		//Debug.Log("Reseting nod");
		initRotation = mNodSensor.rotation;
		initRotationEuler = mNodSensor.eulerRotation;

        NodGyro vNodGyro = mNodSensor.gyro;
        initGyro = new Vector3(vNodGyro.gyroX, vNodGyro.gyroY, vNodGyro.gyroZ);
        NodAccel vNodAccel = mNodSensor.acceleration;
        initAcceleration = new Vector3(vNodAccel.accelX, vNodAccel.accelY, vNodAccel.accelZ);

        curRotation = Quaternion.identity;
		curRotationEuler = Vector3.zero;
		curRotationRawEuler = Vector3.zero;
        currentAcceleration = Vector3.zero;
        currentGyro = Vector3.zero;
    }

    /// <summary>
    /// Starts reading from designated source.
    /// </summary>
    public void StartReading()
	{
		//Debug.Log("StartReading nod");
		mIsStartConnection = true;
	}

	/// <summary>
	/// Stops reading from designated source.
	/// </summary>
	public void StopReading()
	{
		mIsStartConnection = false;
	}

	/// <summary>
	/// Verify sensor connection. 
	/// Sensors connections happen asynchronously on mobile devices, 
	/// check each frame for a connected ring
	/// </summary>
	public Boolean VerifyNodConnection()
	{
		if (!mIsNodConnected && mIsStartConnection) 
		{
			//Debug.Log("Detecting nod : " + nodID);

			mIsNodConnected = false;

			int vNumRingsPaired = mNodController.getNumDevices();

			Debug.Log("Num nods paired : " + vNumRingsPaired);

			if (vNumRingsPaired > nodID) 
			{
				mNodSensor = mNodController.getNodDevice(nodID);

				//Debug.Log("Got nod ring object : " + nodID);

				if (null == mNodSensor)
				{	
					//Debug.Log("Could not find ring object : " + nodID);
					mIsNodConnected = false;
				}
				else
				{
                    if (mNodSensor.Subscribe(NodSubscriptionType.EulerMode) &&
                       mNodSensor.Subscribe(NodSubscriptionType.ButtonMode) &&
                                        mNodSensor.Subscribe(NodSubscriptionType.AccelMode) &&
                                       mNodSensor.Subscribe(NodSubscriptionType.GyroMode))
                    //                   if (mNodSensor.Subscribe(NodSubscriptionType.EulerMode))
                    {
                        //Debug.Log("Ring Success !! : " + nodID);
                        Reset();
						mIsNodConnected = true;
					}
				}
			} 
			else
			{
				mIsNodConnected = false;
			}
		}

		return mIsNodConnected;
	}

	/// <summary>
	/// Stops the nod connection.
	/// </summary>
	public void StopNodConnection()
	{
		if (null == mNodSensor)
			return;

		mNodSensor.Unsubscribe(NodSubscriptionType.EulerMode);
        mNodSensor.Unsubscribe(NodSubscriptionType.ButtonMode);
        mNodSensor.Unsubscribe(NodSubscriptionType.AccelMode);
        mNodSensor.Unsubscribe(NodSubscriptionType.GyroMode);
    }

    /// <summary>
    /// Call this function to update current sensor values.
    /// </summary>
    public void UpdateSensor () 
	{
		//Debug.Log("Updating Ring !! : " + nodID);

		if (!VerifyNodConnection())
			return;

		//Debug.Log("Connection verified !! : " + nodID);

		//Call this once per update to check for updated ring values.
		mNodSensor.CheckForUpdate();
		
		//Example of applying the rings orientation to the local transform.
		curRotation = mNodSensor.rotation;
		curRotationEuler = curRotation.eulerAngles;

        NodGyro vNodGyro= mNodSensor.gyro;
        currentGyro = new Vector3(vNodGyro.gyroX, vNodGyro.gyroY, vNodGyro.gyroZ);
        NodAccel vNodAccel = mNodSensor.acceleration;
        currentAcceleration = new Vector3(vNodAccel.accelX, vNodAccel.accelY, vNodAccel.accelZ);


		//Debug.Log("Current rotation: " + curRotationEuler.x + " , " + curRotationEuler.y + " , " + curRotationEuler.z);

		curRotationRawEuler = mNodSensor.eulerRotation;

		//Debug.Log("Current rotation euler: " + curRotationRawEuler.x + " , " + curRotationRawEuler.y + " , " + curRotationRawEuler.z);

	}

	/**
	 * Returns true if IMU is connected. Meant to be accessed from other classes.
	 */
	public bool IsConnected()
	{
		return mIsNodConnected;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
	//////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start () 
	{
		mNodController = NodController.GetNodInterface();

		if(independantUpdate)
		{
			StartReading();
		}
	}

	/// <summary>
	/// disable.
	/// </summary>
	void OnDisable()
	{
		if(independantUpdate)
		{
			StopNodConnection();
		}
	}

	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update () 
	{	
		if(independantUpdate)
		{
			UpdateSensor();
		}
	}
}
