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

public class ss_joint : MonoBehaviour 
{
  /**
   * Child in StretchSense Character game object representing this joint.
   */
  public Transform joint_object = null;
  
  /**
   * Property representing the positive rotational directions for the current joint.
   */
	public Vector3 directions = Vector3.zero;

  /**
   * Pauses the animation of the joint, regardless of all settings.
   */
	public bool pause_animation = false;

	/**
   * Show debug data.
   */
	public bool show_debug = false;
	
	/**
   * Update independently of ss_container.cs
   */
  public bool independent_update = false;

  /**
   * Resulting transforms.
   */
  protected Vector3 orientation_euler;
  protected Quaternion joint_rotation;
	protected Vector3 joint_position;

	/**
   * Array of StretchSense sensors on this joint.
   */
  protected ss_sensor[] sensors;
  
  /**
   * get_sensor_by_name(string vName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return StretchSense sensor, or null if none found.
   */
	protected ss_sensor get_sensor_by_name(string vName)
	{
		for (int i = 0; i < sensors.Length; i++) {
			if (sensors[i].name == vName) {
				return sensors[i];
			}
		}

		return null;
  }
  
  /**
   * get_sensor_by_position(ss_sensor.positionName position)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	protected ss_sensor get_sensor_by_position(ss_sensor.positionName position)
	{
		for (int i = 0; i < sensors.Length; i++) {
			if (sensors[i].position == position) {
				return sensors[i];
			}
		}

		return null;
  }
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	public void start_joint (string data_set)
	{
    // Don't update anything if joint is paused.
    if (pause_animation) {
        return;
    }

		for (int i = 0; i < sensors.Length; i++) {

			// The CSV data set allows us to set specific files (by folder) to all sensors.
			sensors[i].CSV_data_set = data_set;
			sensors[i].StartReading();
		}
  }
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	public virtual void update_joint () {}
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	public void reset_joint ()
	{
		for (int i = 0; i < sensors.Length; i++) {
			sensors[i].reset();
		}

		//reset the transforms
		mCurJointRotation = Quaternion.identity;
		mCurJointPosition = Vector3.zero;
		orientation_euler  = Vector3.zero;

		if(joint_object != null)
		{
			joint_object.localRotation = mCurJointRotation;
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
		sensors = GetComponentsInChildren<StretchSensor>();

		for (int i = 0; i < sensors.Length; i++) 
		{
			sensors[i].stretchID = i;
		}
	}

	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start() 
	{
		if (independent_update) 
		{
			reset_joint();
			StartJoint("");
		}
	}

	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update() 
	{
		if (independent_update) 
		{
			update_joint();
		}
	}

	/// <summary>
	/// GUI Update.
	/// </summary>
	void OnGUI()
	{

	}
}

