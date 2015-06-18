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
	public Vector3 rotational_directions = Vector3.zero;

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

	/**
   * Array of StretchSense sensors on this joint.
   */
  protected ss_sensor[] sensors;
  
  /**
   * get_sensor_by_name(string name)
   * @brief Update() is called once per frame.
   * @param progName
   * @return StretchSense sensor, or null if none found.
   */
	protected ss_sensor get_sensor_by_name(string name)
	{
		for (int i = 0; i < sensors.Length; i++) {
			if (sensors[i].name == name) {
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
   * start_joints(string data_set)
   * @brief ...
   * @param data_set Name of CSV data set.
   * @return void
   */
	public void start_joint(string data_set)
	{
    // Don't update anything if joint is paused.
    if (pause_animation) {
        return;
    }

		for (int i = 0; i < sensors.Length; i++) {

			// "data_set" allows us to set specific data sets to all sensors at once.
			sensors[i].csv_data_set = data_set;
			sensors[i].start_reading();
		}
  }
  
  /**
   * update_joint()
   * @brief This method is called once per frame, and is meant to be overriden by child scripts.
   * @return void
   */
	public virtual void update_joint () {}
  
  /**
   * reset_joint()
   * @brief Resets joint.
   * @return void
   */
	public void reset_joint ()
	{
		for (int i = 0; i < sensors.Length; i++) {
			sensors[i].reset();
		}

		// Reset the transforms
    joint_rotation = Quaternion.identity;
    orientation_euler = Vector3.zero;

		if (joint_object != null) {
			joint_object.localRotation = joint_rotation;
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
		sensors = GetComponentsInChildren<ss_sensor>();

		for (int i = 0; i < sensors.Length; i++) {
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
		if (independent_update) {
			reset_joint();
			start_joint("");
		}
	}
  
  /**
   * Update()
   * @brief This method is called by Unity once per frame.
   * @return void
   */
	void Update() 
	{
		if (independent_update) {
			update_joint();
		}
	}
}

