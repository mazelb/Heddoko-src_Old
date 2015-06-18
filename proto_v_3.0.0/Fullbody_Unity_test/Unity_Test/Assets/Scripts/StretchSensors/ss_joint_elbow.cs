/**
 * @file ss_joint_elbow.cs
 * @brief Describe what this script does.
 * @note Add notes.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class ss_joint_elbow : ss_joint
{
  /**
   * usage(const char *progName)
   * @brief Converts StretchSense data into an elbow angle in degrees (starting from a T-pose)
   * @param progName
   * @return -1 for failure
   */
	private float get_elbow_angle(ss_sensor elbow)
	{
		int map_to = 1000;
		float val = elbow.get_mapped_reading(map_to);
		float angle = (val / map_to) * (180 - 39);
		
		if (show_debug) {
			print ("Elbow: " + angle);
		}
		
		return angle;
	}
	
  /**
   * get_forearm_angle(ss_sensor forearm)
   * @brief Converts StretchSense data into forearm orientation in degrees (starting from a T-pose).
   * @param progName
   * @return -1 for failure
   */
	private float get_forearm_angle(ss_sensor forearm)
	{
		int map_to = 1000;
		float val = forearm.get_mapped_reading(map_to);
		float angle = (val / map_to) * 210 - 140;

		if (show_debug) {
			print ("Forearm: " + angle);
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

    // Update individual sensor data.
    for (int ndx = 0; ndx < sensors.Length; ndx++) {
      sensors[ndx].update_sensor();
    }

    // Retrieve sensor objects.
    ss_sensor elbow = get_sensor_by_position(ss_sensor.positionName.Elbow);
    ss_sensor forearm = get_sensor_by_position(ss_sensor.positionName.Forearm);

    // Update angles
    if (elbow != null && forearm != null) {

      float elbowAngle = get_elbow_angle(elbow);
      float forearmAngle = get_forearm_angle(forearm);

      orientation_euler.y = elbowAngle * rotational_directions.y;
      orientation_euler.x = forearmAngle * rotational_directions.x;

      joint_object.localRotation = Quaternion.Euler(orientation_euler);
    }
  }
}

