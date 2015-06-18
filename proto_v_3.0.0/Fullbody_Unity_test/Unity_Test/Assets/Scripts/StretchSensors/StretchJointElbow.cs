/**
 * @file StretchSensor.cs
 * @brief Describe what this script does.
 * @note Add notes.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class ss_jointElbow : ss_joint
{
	//
	// Converts StretchSense data into an elbow angle in degrees (starting from a T-pose).
  //
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_elbow_angle(StretchSensor elbow)
	{
		int mapTo = 1000;
		float ssValue = elbow.getMappedReading(mapTo);
		float angle = (ssValue / mapTo) * (180 - 39);
		
		if (show_debug) {
			print ("Elbow: " + angle);
		}
		
		return angle;
	}
	
	//
	// Converts StretchSense data into forearm orientation in degrees (starting from a T-pose).
  //
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_forearm_angle(StretchSensor forearm)
	{
		int mapTo = 1000;
		float ssValue = forearm.getMappedReading(mapTo);
		float angle = (ssValue / mapTo) * 210 - 140;

		if (show_debug) {
			print ("Forearm: " + angle);
		}
		
		return angle;
	}

	//
    // Updates joint position and values.
  //
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
    public override void update_joint()
    {
	    // Don't update anything if joint is paused.
	    if (pause_animation) {
	        return;
	    }

        // Update individual sensor data.
		for (int ndx = 0; ndx < sensors.Length; ndx++) {
			sensors[ndx].UpdateSensor();
		}

        // Retrieve sensor objects.
        StretchSensor elbow = getSensorByPosition(StretchSensor.positionName.Elbow);
        StretchSensor forearm = getSensorByPosition(StretchSensor.positionName.Forearm);

        // Update angles
        if (elbow != null && forearm != null)
        {
            float elbowAngle = get_elbow_angle(elbow);
			float forearmAngle = get_forearm_angle(forearm);

            orientation_euler.y = elbowAngle * orientation.y;
            orientation_euler.x = forearmAngle * orientation.x;

            joint_object.localRotation = Quaternion.Euler(orientation_euler);
        }
	}
}

