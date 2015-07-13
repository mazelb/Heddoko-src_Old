/**
 * @file StretchJoint.cs
 * @brief Describe what this script does.
 * @note Add notes.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJoint : MonoBehaviour 
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
   * GetSensorByName(string name)
   * @brief Update() is called once per frame.
   * @param progName
   * @return StretchSense sensor, or null if none found.
   */
	protected StretchSensor GetSensorByName(string name)
	{
		for (int i = 0; i < maSensors.Length; i++)
        {
			if (maSensors[i].name == name)
            {
				return maSensors[i];
			}
		}

		return null;
    }
  
  /**
   * GetSensorByPosition(StretchSensor.PositionName position)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	protected StretchSensor GetSensorByPosition(StretchSensor.PositionName position)
	{
		for (int i = 0; i < maSensors.Length; i++)
        {
			if (maSensors[i].position == position)
            {
				return maSensors[i];
			}
		}

		return null;
    }
  
  /**
   * StartJoint(string vDataSet)
   * @brief ...
   * @param vDataSet Name of CSV data set.
   * @return void
   */
	public void StartJoint(string vDataSet)
	{
        // Don't update anything if joint is paused.
        if (vPauseAnimation)
        {
            return;
        }

		for (int i = 0; i < maSensors.Length; i++)
        {
			// "vDataSet" allows us to set specific data sets to all sensors at once.
			maSensors[i].csv_data_set = vDataSet;
			maSensors[i].start_reading();
		}
    }
  
  /**
   * UpdateJoint()
   * @brief This method is called once per frame, and is meant to be overriden by child scripts.
   * @return void
   */
	public virtual void UpdateJoint() {}
  
  /**
   * ResetJoint()
   * @brief Resets joint.
   * @return void
   */
	public void ResetJoint()
	{
		for (int i = 0; i < maSensors.Length; i++)
        {
			maSensors[i].reset();
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
     * Awake()
     * @brief This method is called by Unity when the program is launched.
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
     * Start()
     * @brief This method is called by Unity when the program is started.
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
     * Update()
     * @brief This method is called by Unity once per frame.
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

