using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJoint : MonoBehaviour 
{
	// Joint degrees-of-freedom.
	public Transform jointTransform = null;
	public Vector3 rotatesXYZ = Vector3.zero;

    // Pauses the animation of the joint, regardless of all settings.
	public bool pauseAnimation = false;
	
	// When True the stretch joint updates independently of the rest,
	// otherwise the joint only updates when the full body is updated.
	public bool independentUpdate = false;
	
	// Resulting transforms
	protected Quaternion mCurJointRotation;
	protected Vector3 mCurJointRotationEuler;
	protected Vector3 mCurJointPosition;

	// Each joint can be composed of one or multiple sensors simultaneously
	protected StretchSensor[] mStretchSensors;

	/// <summary>
	/// Gets the sensor by name.
	/// </summary>
	/// <returns>The sensor by name.</returns>
	/// <param name="vName">V name.</param>
	protected StretchSensor getSensorByName(String vName)
	{
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) 
		{
			if (mStretchSensors[ndx].name == vName)
			{
				return mStretchSensors[ndx];
			}
		}

		return null;
	}

	/// <summary>
	/// Gets the sensor by position.
	/// </summary>
	/// <returns>The sensor by position.</returns>
	/// <param name="position">Position.</param>
	protected StretchSensor getSensorByPosition(StretchSensor.positionName position)
	{
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++)
		{
			if (mStretchSensors[ndx].bodyPosition == position)
			{
				return mStretchSensors[ndx];
			}
		}

		return null;
	}

	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	/// <param name="dataSet">Data set.</param>
	public void StartJoint (String dataSet)
	{
	    // Don't update anything if joint is paused.
	    if (pauseAnimation) {
	        return;
	    }

		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) 
		{
			// The CSV data set allows us to set specific files (by folder) to all sensors.
			mStretchSensors[ndx].CSVDataSet = dataSet;
			mStretchSensors[ndx].StartReading();
		}
	}

	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public virtual void UpdateJoint ()
	{
	    // Don't update anything if joint is paused.
	    if (pauseAnimation) {
	        return;
	    }

		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) {
			mStretchSensors[ndx].UpdateSensor();
		}
/*
		switch (jointType) 
		{
		case JointType.RightElbow:
		{
			StretchSensor vElbowSensor = getSensorByName("RightElbowStretchSensor");
			StretchSensor vForeArmSensor = getSensorByName("RightForeArmStretchSensor");

			if( vElbowSensor != null && vForeArmSensor != null )
			{
				float mAngleForeArm = vForeArmSensor.getCurAngleReading();
				float mAngleElbow = vElbowSensor.getCurAngleReading(); 

				mCurJointRotationEuler.x = mAngleForeArm * rotatesXYZ.x;
				mCurJointRotationEuler.y = mAngleElbow * rotatesXYZ.y;

				jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);
			}

			break;
		}
		case JointType.LeftElbow:
		{
			StretchSensor vElbowSensor = getSensorByName("LeftElbowStretchSensor");
			StretchSensor vForeArmSensor = getSensorByName("LeftForeArmStretchSensor");
			
			if( vElbowSensor != null && vForeArmSensor != null )
			{
				float mAngleForeArm = vForeArmSensor.getCurAngleReading();
				float mAngleElbow = vElbowSensor.getCurAngleReading(); 
				
				mCurJointRotationEuler.x = mAngleForeArm * rotatesXYZ.x;
				mCurJointRotationEuler.y = mAngleElbow * rotatesXYZ.y;
				
				jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);
			}
			
			break;
		}
		
		case JointType.RightShoulder:
		case JointType.LeftShoulder:
		{
			StretchSensor vShoulderSensor = getSensorByName("ShoulderStretchSensor");
			StretchSensor vBackSensor = getSensorByName("BackStretchSensor");
			StretchSensor vFrontSensor = getSensorByName("FrontStretchSensor");
			StretchSensor vBodySensor = getSensorByName("BodyStretchSensor");

			if( vShoulderSensor != null && 
			    vBackSensor != null && 
			    vFrontSensor != null && 
			    vBodySensor != null )
			{
				float mAngleI = 0.0f;
				float mAngleS = 0.0f; 
				float mAngleP = 0.0f; 
				float mAngleA = 0.0f;

				mAngleI = ( factorsABCi.x * (vShoulderSensor.getCurReading() * 90.0f / vShoulderSensor.maxStretchVal) ) +
						  ( factorsABCi.y * vBackSensor.getCurReading() / vBackSensor.maxStretchVal ) +
						  ( factorsABCi.z * vFrontSensor.getCurReading() / vFrontSensor.maxStretchVal );

				mAngleS = ( factorsABCs.x * (vBodySensor.getCurReading() * 90.0f / vBodySensor.maxStretchVal) ) +
						  ( factorsABCs.y * vBackSensor.getCurReading() / vBackSensor.maxStretchVal ) +
						  ( factorsABCs.z * vFrontSensor.getCurReading() / vFrontSensor.maxStretchVal );

				mAngleP = ( factorsABCp.x * (vFrontSensor.getCurReading() * 90.0f / vFrontSensor.maxStretchVal) ) +
						  ( factorsABCp.y * vBodySensor.getCurReading() / vBodySensor.maxStretchVal ) +
						  ( factorsABCp.z * vShoulderSensor.getCurReading() / vShoulderSensor.maxStretchVal );

				mAngleA = ( factorsABCa.x * (vBackSensor.getCurReading() * 90.0f / vShoulderSensor.maxStretchVal) ) +
						  ( factorsABCa.y * vBodySensor.getCurReading() / vBodySensor.maxStretchVal ) +
						  ( factorsABCa.z * vShoulderSensor.getCurReading() / vShoulderSensor.maxStretchVal );

				//TODO: apply angles to the joint transformation

			}

			break;
		}
		
		case JointType.Torso:
			//TODO: add multiple sensors combine
			break;
		
		case JointType.Hip:
			//TODO: add multiple sensors combine
			break;
		
		case JointType.RightKnee:
		case JointType.LeftKnee:
		{
			applySingleRotation();
			break;
		}
		
		default:
			break;
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

		if(jointTransform != null)
		{
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
		if (independentUpdate) 
		{
			ResetJoint();
			StartJoint("");
		}
	}

	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update() 
	{
		if (independentUpdate) 
		{
			UpdateJoint();
		}
	}

	/// <summary>
	/// GUI Update.
	/// </summary>
	void OnGUI()
	{

	}
}

