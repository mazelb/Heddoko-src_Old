/**
 * @file ss_joint_knee.cs
 * @brief Describe what this script does.
 * @note Add notes.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class ss_joint_knee : ss_joint
{
  /**
   * Converts StretchSense data into a knee angle (starting from a T-pose).
   * 
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_elbow_angle(ss_sensor knee)
	{
		int map_to = 1000;
		float val = knee.get_mapped_reading(map_to);
		float angle = (val / map_to) * (180 - 39);
		angle = float.IsNaN(angle) ? 0.0f : angle;

		if (show_debug) {
		    print("Knee angle: "+ angle);
		}
		
		return angle;
	}

  /**
  * usage(const char *progName)
  * @brief Updates joint position and values.
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
    for (int i = 0; i < sensors.Length; i++) {
      sensors[i].update_sensor();
    }

    // Update knee angle
    ss_sensor knee = sensors[0];
    if (knee != null) {
      orientation_euler = get_elbow_angle(knee) * rotational_directions;
      joint_object.localRotation = Quaternion.Euler(orientation_euler);
    }
	}
}

