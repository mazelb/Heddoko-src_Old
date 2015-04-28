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
	private NodControler mNodController;
	private NodRing mNodSensor = null;
	private bool mIsNodConnected = false;
	private bool mIsStartConnection = false;

	//Nod data
	public Quaternion initRotation = Quaternion.identity;
	public Quaternion curRotation = Quaternion.identity;
	public Vector3 initRotationEuler = Vector3.zero;
	public Vector3 curRotationEuler = Vector3.zero;
	public Vector3 curRotationRawEuler = Vector3.zero;

	/// <summary>
	/// Reset the sensors data to init status.
	/// </summary>
	public void Reset() 
	{
		if (null == mNodSensor)
			return;

		initRotation = mNodSensor.ringRotation;
		initRotationEuler = mNodSensor.ringEulerRotation;
		curRotation = Quaternion.identity;
		curRotationEuler = Vector3.zero;
		curRotationRawEuler = Vector3.zero;
	}
	
	/// <summary>
	/// Starts reading from designated source.
	/// </summary>
	public void StartReading()
	{
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
			Debug.Log("Detecting nod : " + nodID);

			mIsNodConnected = false;

			int vNumRingsPaired = mNodController.getNumDevices();

			Debug.Log("Num nods paired : " + vNumRingsPaired);

			if (vNumRingsPaired > nodID) 
			{
				mNodSensor = mNodController.getRing(nodID);

				Debug.Log("Got nod ring object : " + nodID);

				if (null == mNodSensor)
				{	
					Debug.Log("Could not find ring object : " + nodID);
					mIsNodConnected = false;
				}
				else
				{
					if(mNodSensor.SubscribeToPose6D() && mNodSensor.SubscribeToButton())
					{
						Debug.Log("Ring Success !! : " + nodID);
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
		
		mNodSensor.UnsubscribeToPose6D();
		mNodSensor.UnsubscribeToButton();
	}

	/// <summary>
	/// Call this function to update current sensor values.
	/// </summary>
	public void UpdateSensor () 
	{
		if (!VerifyNodConnection())
			return;

		//Call this once per update to check for updated ring values.
		mNodSensor.CheckForUpdate();
		
		//Example of applying the rings orientation to the local transform.
		curRotation = mNodSensor.ringRotation;
		curRotationEuler = curRotation.eulerAngles;
		curRotationRawEuler = mNodSensor.ringEulerRotation;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
	//////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start () 
	{
		mNodController = NodControler.GetNodInterface();

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
