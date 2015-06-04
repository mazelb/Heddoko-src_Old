using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJoint : MonoBehaviour 
{
	//The type of joint (affects the calculations of angles)
	public enum JointType {RightForeArm, LeftForeArm, RightElbow, LeftElbow, RightShoulder, LeftShoulder, Torso, Hip, RightKnee, LeftKnee};
	public JointType jointType = JointType.RightElbow;

	//represent the different degrees of freedom ?
	public Transform jointTransform = null;
	public Vector3 rotatesXYZ = Vector3.zero;

    // Keeps joint from updating with container temporarily.
	public Boolean pauseAnimation = false;
	
	//Resulting transforms
	protected Quaternion mCurJointRotation;
	protected Vector3 mCurJointRotationEuler;
	protected Vector3 mCurJointPosition; 

	//When True the stretch joint updates independently of the rest
	//otherwise the joint only updates when the full body is updated
	public Boolean independentUpdate = false; 

	//StretchSensors Combination factors Data 
	//public enum DataCombinationType {LinearComb};
	//public DataCombinationType combType;
	public Vector3 factorsABCi = Vector3.zero; 
	public Vector3 factorsABCs = Vector3.zero;
	public Vector3 factorsABCp = Vector3.zero;
	public Vector3 factorsABCa = Vector3.zero;

	//each joint can be composed of one or multiple sensors simultaneously
	protected StretchSensor[] mStretchSensors;

	/// <summary>
	/// Gets the sensor by name.
	/// </summary>
	/// <returns>The sensor by name.</returns>
	/// <param name="vName">V name.</param>
	protected virtual StretchSensor getSensorByName(String vName)
	{
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) 
		{
			if(mStretchSensors[ndx].name == vName)
			{
				return mStretchSensors[ndx];
			}
		}

		return null;
	}

	protected StretchSensor getSensorByPosition(StretchSensor.ssPositionName position)
	{
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++)
		{
			if(mStretchSensors[ndx].bodyPosition == position)
			{
				return mStretchSensors[ndx];
			}
		}

		return null;
	}

	/// <summary>
	/// Applies the single rotation to the joint.
	/// </summary>
	protected void applySingleRotation()
	{
		if (mStretchSensors.Length == 1) //Single Sensor for joint
		{
			//update angle of the joint
			mCurJointRotationEuler = mStretchSensors[0].getCurAngleReading() * rotatesXYZ;
			jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);
		}
	}


	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
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

