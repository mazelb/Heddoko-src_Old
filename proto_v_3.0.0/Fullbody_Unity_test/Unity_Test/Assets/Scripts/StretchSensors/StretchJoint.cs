using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJoint : MonoBehaviour 
{
	//represent the different degrees of freedom ?
	public Transform jointTransform = null;
	public Vector3 rotatesXYZ = Vector3.zero;

	//TODO: See if translation is necessary
	public Vector3 translatesXYZ = Vector3.zero;

	//Resulting transforms
	private Quaternion mCurJointRotation;
	private Vector3 mCurJointRotationEuler;
	private Vector3 mCurJointPosition; 

	//When True the stretch joint updates independantly of the rest
	//otherwise the joint only updates when the full body is updated
	public Boolean independantUpdate = false; 

	//each joint can be composed of one or multiple sensors simultaneously
	private StretchSensor[] mStretchSensors;

	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoint () 
	{
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) 
		{
			mStretchSensors[ndx].StartReading();
		}
	}

	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public void UpdateJoint () 
	{
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) 
		{
			mStretchSensors[ndx].UpdateSensor();
		}

		//TODO: This part is dependant on the type of joint we are tracking
		//update the joint angles
		if (mStretchSensors.Length == 1) //Single Sensor for joint
		{
			//update angle of the joint
			mCurJointRotationEuler = mStretchSensors[0].getCurAngleReading() * rotatesXYZ;

			jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);

			//TODO: add translation detection
			//mCurJointPosition; 
		}
		else if (mStretchSensors.Length > 1) //Multiple sensors for joint
		{
			//TODO: add multi-sensors to Angle transformations
		}
	}

	/// <summary>
	/// Reset the stretch joint sensors.
	/// </summary>
	public void ResetJoint ()
	{
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) 
		{
			mStretchSensors[ndx].Reset();
		}

		//reset the transforms
		mCurJointRotation = Quaternion.identity;
		mCurJointPosition = Vector3.zero;
		mCurJointRotationEuler  = Vector3.zero;
		if(jointTransform != null)
		{
			//jointTransform.localPosition = mCurJointPosition;
			jointTransform.localRotation = mCurJointRotation;
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
		mStretchSensors = GetComponentsInChildren<StretchSensor>();

		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) 
		{
			mStretchSensors[ndx].stretchID = ndx;
		}
	}

	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start() 
	{
		if (independantUpdate) 
		{
			ResetJoint();
			StartJoint();
		}
	}

	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update() 
	{
		if (independantUpdate) 
		{
			UpdateJoint();
		}
	}

	/// <summary>
	/// GUI Update.
	/// </summary>
	void OnGUI()
	{
		//Show the name of the ring over the joint the NodBone is attached to
		Camera cam = Camera.current;
		if (null == cam)
			return;

		foreach (StretchSensor sensor in mStretchSensors) {
			string msg = sensor.stretchName;

			Vector3 sensorWorldPos = sensor.transform.position;
			Vector3 pos = cam.WorldToScreenPoint(sensorWorldPos);
			GUI.Label(new Rect(pos.x, Screen.height - pos.y, 150, 150), msg);
		}
	}
}

