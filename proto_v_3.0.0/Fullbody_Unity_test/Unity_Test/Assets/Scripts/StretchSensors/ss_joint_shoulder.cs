/**
 * @file ss_joint_shoulder.cs
 * @brief Describe what this script does.
 * @note Add notes.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class ss_joint_shoulder : ss_joint
{
  /**
   * 
   */
	public bool apply_correction = true;
	
	/**
   * Gets the axial angle in degrees (starting from a T-pose).
   */
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_axial_angle(ss_sensor axilla, ss_sensor clavicle, ss_sensor deltoid, ss_sensor trapezius)
	{
		int map_to = 1000;
		float axilla_value = axilla.get_mapped_reading(map_to);
		float clavicle_value = clavicle.get_mapped_reading(map_to);
		float deltoid_value = deltoid.get_mapped_reading(map_to);
		float trapezius_value = trapezius.get_mapped_reading(map_to);
		
		// Initial calculations.
    float anteriorAngle = (trapezius_value / map_to) * 90;
    float posteriorAngle = -1 * (clavicle_value / map_to) * 90;
    //float posteriorAngle = -1 * (1 / (float) Math.Pow(clavicle.max_value - clavicle.min_value, 2)) * (float) Math.Pow(clavicle.get_reading() - clavicle.min_value, 2) * 90;
		
		// Correction factors.
		if (apply_correction) {

			float axillaCorrection = 0.15f * axilla_value / map_to;
			float deltoidCorrection = 0.15f * deltoid_value / map_to;
			axillaCorrection = float.IsNaN(axillaCorrection) ? 0.0f : axillaCorrection;
			deltoidCorrection = float.IsNaN(deltoidCorrection) ? 0.0f : deltoidCorrection;
			
			anteriorAngle = anteriorAngle - axillaCorrection - deltoidCorrection;
			posteriorAngle = posteriorAngle + axillaCorrection + deltoidCorrection;
		}
		
		// Make sure we return floating point numbers.
		anteriorAngle = float.IsNaN(anteriorAngle) ? 0.0f : anteriorAngle;
		posteriorAngle = float.IsNaN(posteriorAngle) ? 0.0f : posteriorAngle;
		
		if (show_debug) {
			print ("Axial angle (anterior + posterior): " + anteriorAngle + " + " + posteriorAngle);
		}
		
		return anteriorAngle + posteriorAngle;
	}

  /**
   * usage(const char *progName)
   * @brief Gets the frontal angle in degrees (starting from a T-pose).
   * @param progName
   * @return -1 for failure
   */
	private float get_frontal_angle(ss_sensor axilla, ss_sensor clavicle, ss_sensor deltoid, ss_sensor trapezius)
	{
		int map_to = 1000;
		float axilla_value = axilla.get_mapped_reading(map_to);
		float clavicle_value = clavicle.get_mapped_reading(map_to);
		float deltoid_value = deltoid.get_mapped_reading(map_to);
		float trapezius_value = trapezius.get_mapped_reading(map_to);
		
		// Initial calculations.
		float superior_angle = (axilla_value / map_to) * 90;
		float inferior_angle = -1 * (deltoid_value / map_to) * 90;
		
		// Correction factors.
		if (apply_correction) {

      // TODO: describe these factors.
			float clavicle_correction = 0.15f * clavicle_value / map_to;
			float trapezius_correction = 0.15f * trapezius_value / map_to;
			clavicle_correction = float.IsNaN (clavicle_correction) ? 0.0f : clavicle_correction;
			trapezius_correction = float.IsNaN (trapezius_correction) ? 0.0f : trapezius_correction;
			
			superior_angle = superior_angle - clavicle_correction - trapezius_correction;
			inferior_angle = inferior_angle + clavicle_correction + trapezius_correction;
		}
		
		// Make sure we return floating point numbers.
		superior_angle = float.IsNaN(superior_angle) ? 0.0f : superior_angle;
		inferior_angle = float.IsNaN(inferior_angle) ? 0.0f : inferior_angle;
		
		if (show_debug) {
			print ("Frontal angle (superior + inferior): " + superior_angle + " + " + inferior_angle);
		}
		
		return superior_angle + inferior_angle;
	}
	
  /**
   * Gets the rotation angle in degrees (starting from a T-pose).
   * 
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_rot_angle(ss_sensor axilla, ss_sensor clavicle, ss_sensor deltoid, ss_sensor trapezius)
	{
		float angle = 0.0f;
		
		return angle;
	}
	
  /**
   * Updates joint position and values.
   * 
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
		for (int i = 0; i < sensors.Length; i++) {
			sensors[i].update_sensor();
		}
		
		// Retrieve sensor objects.
		ss_sensor axilla = get_sensor_by_position(ss_sensor.positionName.Axilla);
		ss_sensor clavicle = get_sensor_by_position(ss_sensor.positionName.Clavicle);
		ss_sensor deltoid = get_sensor_by_position(ss_sensor.positionName.Deltoid);
		ss_sensor trapezius = get_sensor_by_position(ss_sensor.positionName.Trapezius);
		
		// Update shoulder orientation.
		if (axilla != null && clavicle != null && deltoid != null && trapezius != null) {

      // ...
			float axial_angle = get_axial_angle(axilla, clavicle, deltoid, trapezius);
			float frontal_angle = get_frontal_angle(axilla, clavicle, deltoid, trapezius);
			float rot_angle = get_rot_angle(axilla, clavicle, deltoid, trapezius);
			
      // ...
			orientation_euler.y = axial_angle * rotational_directions.y;
			orientation_euler.z = frontal_angle * rotational_directions.z;
			orientation_euler.x = rot_angle * rotational_directions.x;
			
			if (show_debug) {
				print("Euler angles (x / y / z): "+ orientation_euler.x +" / "+ orientation_euler.y +" / "+ orientation_euler.y);
			}
			
			joint_object.localRotation = Quaternion.Euler(orientation_euler);
		}
		
		else {
			print ("Missing shoulder sensors...");
		}
	}
}

