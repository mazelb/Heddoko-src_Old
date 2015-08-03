/**
 * @file ss_joint.cs
 * @brief Describe what this script does.
 * @note Add notes.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

public class SSJoint : MonoBehaviour 
{
    /**
     * Child in StretchSense Character game object representing this joint.
     */
    public Transform vJointObject = null;
  
    /**
     * Property representing the positive rotational directions for the current joint.
     */
    public Vector3 vRotationalDirections = Vector3.zero;

    /**
     * Pauses the animation of the joint, regardless of all settings.
     */
    public bool vPauseAnimation = false;

    /**
     * Show debug data.
     */
    public bool vShowDebug = false;

    /**
     * Update independently of ss_container.cs
     */
    public bool vIndependentUpdate = false;

    /**
     * Resulting transforms.
     */
    protected Vector3 mOrientationEuler;
    protected Quaternion mJointRotation;

    /**
     * Array of StretchSense sensors on this joint.
     */
    protected StretchSensor[] maSensors;
  
    /**
     * @brief               Gets a sensor object by name.
     * @param string vName  Sensor name.
     * @return StretchSense sensor, or null if none found.
     */
    protected StretchSensor GetSensorByName(string vName)
    {
    	for (int i = 0; i < maSensors.Length; i++)
        {
    		if (maSensors[i].name == vName)
            {
    			return maSensors[i];
    		}
    	}

    	return null;
    }

    /**
     * @brief                                       Gets a sensor object by its position on the body.
     * @param StretchSensor.PositionName vPosition  Sensor's position on the body.
     * @return StretchSense                         sensor, or null if none found.
     */
    protected StretchSensor GetSensorByPosition(StretchSensor.PositionName vPosition)
    {
    	for (int i = 0; i < maSensors.Length; i++)
        {
    		if (maSensors[i].vPosition == vPosition)
            {
    			return maSensors[i];
    		}
    	}

    	return null;
    }

    /**
     * @brief                   ...
     * @param string vDataSet   Name of CSV data set.
     * @return void
     */
    public void StartJoint(string vDataSet)
    {
		// Set orientation of joint if specified.
		string vOrientationsFile = "../../Data/"+ vDataSet +"/"+ name +".orientations.csv";
		if (File.Exists(vOrientationsFile))
		{
			print("Loading orientation data for "+ name);

			string[] vOrientations = File.ReadAllLines(vOrientationsFile);
			int vx = Convert.ToInt32(vOrientations[0]);
			int vy = Convert.ToInt32(vOrientations[1]);
			int vz = Convert.ToInt32(vOrientations[2]);

			vJointObject.Rotate(vx, vy, vz);
		}
		
		// Don't animate if joint is paused.
		if (vPauseAnimation)
		{
			return;
		}

		// Initialize sensors.
    	for (int i = 0; i < maSensors.Length; i++)
        {
    		// "vDataSet" allows us to set specific data sets to all sensors at once.
    		maSensors[i].vDataSet = vDataSet;
    		maSensors[i].StartReading();
    	}
    }

    /**
     * @brief           This method is called once per frame, and is meant to be overriden by child scripts.
     * @return void
     */
    public virtual void UpdateJoint() {}
  
    /**
     * @brief           Resets joint.
     * @return void
     */
    public void ResetJoint()
    {
    	for (int i = 0; i < maSensors.Length; i++)
        {
    		maSensors[i].Reset();
    	}

    	// Reset the transforms
        mJointRotation = Quaternion.identity;
        mOrientationEuler = Vector3.zero;

    	if (vJointObject != null)
        {
    		vJointObject.localRotation = mJointRotation;
    	}
    }



	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
	//////////////////////////////////////////////////////////////////////////////////////


  
    /**
     * @brief           This method is called by Unity when the program is launched.
     * @return void
     */
    void Awake ()
    {
    	maSensors = GetComponentsInChildren<StretchSensor>();

    	for (int i = 0; i < maSensors.Length; i++)
        {
            // Initialize sensors here.
    	}
    }
  
    /**
     * @brief           This method is called by Unity when the program is started.
     * @return void
     */
    void Start() 
    {
    	if (vIndependentUpdate)
        {
    		ResetJoint();
    		StartJoint("");
    	}
    }
  
    /**
     * @brief           This method is called by Unity once per frame.
     * @return void
     */
    void Update() 
    {
    	if (vIndependentUpdate)
        {
    		UpdateJoint();
    	}
    }
}

