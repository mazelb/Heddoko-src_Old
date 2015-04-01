using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJoint : MonoBehaviour 
{
	//represent the different degrees of freedom ?
	public Transform jointTransform = null;
	public Vector3 rotatesXYZ = Vector3.zero;
	/*public Boolean rotatesX = false;
	public Boolean rotatesY = false;
	public Boolean rotatesZ = false;
	public Boolean inverseRotationX = false;
	public Boolean inverseRotationY = false;
	public Boolean inverseRotationZ = false;*/

	//TODO: See if translation is necessary
	public Vector3 translatesXYZ = Vector3.zero;
	/*public Boolean translatesX = false;
	public Boolean translatesY = false;
	public Boolean translatesZ = false;*/

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

		//update the joint angles
		if (mStretchSensors.Length == 1) //Single Sensor for joint
		{

			mStretchSensors[0].getCurAngleReading();

			mCurJointRotation;
			;
			mCurJointPosition; 
		}
		else if (mStretchSensors.Length > 1) //Multiple sensors for joint
		{


		}

		/*if (mCurDataIdx >= 0 && mCurDataIdx < mStretchValues.Count) 
		{
			if(mCurDataIdx >= (filteringAvgHistory-1) && useSmoothing) 
			{
				float vSum = 0.0f;
				
				for(int i=0; i < filteringAvgHistory; i++) 
				{
					vSum += mStretchValues[mCurDataIdx - i];
				}
				
				float vResult = vSum / filteringAvgHistory;
				mStretchAngles.Add(mapRange(mMinVal, mMaxVal, MinAngle, MaxAngle, vResult));
			}
			else 
			{
				mStretchAngles.Add(mapRange(mMinVal, mMaxVal, MinAngle, MaxAngle, mStretchValues[mCurDataIdx]));
			}
			
			rightForeArmCurrentEulers.x = rightForeArmCurrentEulers.z = 0;
			rightForeArmCurrentEulers.y = -mStretchAngles[mCurDataIdx];
			rightForeArmTransform.localRotation = Quaternion.Euler (rightForeArmCurrentEulers);
			mCurDataIdx++;
		} 
		else 
		{
			mCurDataIdx = 0;
		}*/
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
		jointTransform.localPosition = mCurJointPosition;
		jointTransform.localRotation = mCurJointRotation;
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
			mStretchSensors[ndx].mStretchID = ndx;
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

		foreach (Example4Ring ring in rings) {
			string msg = ring.RingName();

			Vector3 ringWorldPos = ring.transform.position;
			Vector3 pos = cam.WorldToScreenPoint(ringWorldPos);
			GUI.Label(new Rect(pos.x, Screen.height - pos.y, 150, 150), msg);
		}
	}
}

