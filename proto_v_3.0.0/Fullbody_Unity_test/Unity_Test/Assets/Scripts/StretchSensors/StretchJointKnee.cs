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

public class ss_jointKnee : ss_joint
{
	//
	// Converts StretchSense data into a knee angle (starting from a T-pose).
  //
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_elbow_angle(StretchSensor knee)
	{
		int mapTo = 1000;
		float ssValue = knee.getMappedReading(mapTo);
		float angle = (ssValue / mapTo) * (180 - 39);
		angle = float.IsNaN (angle) ? 0.0f : angle;

		if (show_debug) {
		    print("Knee angle: "+ angle);
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

        // Update individual sensors.
		for (int ndx = 0; ndx < sensors.Length; ndx++) {
			sensors[ndx].UpdateSensor();
		}

        // Update knee angle
        StretchSensor knee = sensors[0];
        if (knee != null)
		{
			orientation_euler = get_elbow_angle(knee) * orientation;
			joint_object.localRotation = Quaternion.Euler(orientation_euler);
        }
	}
}

