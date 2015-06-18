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

public class ss_jointShoulder : ss_joint
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
	private float get_axial_angle(StretchSensor axilla, StretchSensor clavicle, StretchSensor deltoid, StretchSensor trapezius)
	{
		int mapTo = 1000;
		float ssAxilla = axilla.getMappedReading(mapTo);
		float ssClavicle = clavicle.getMappedReading(mapTo);
		float ssDeltoid = deltoid.getMappedReading(mapTo);
		float ssTrapezius = trapezius.getMappedReading(mapTo);
		
		// Initial calculations.
		float anteriorAngle = (ssTrapezius / mapTo) * 90;
		float posteriorAngle = -0.5f * (ssClavicle / mapTo) * 90;
		
		// Correction factors.
		if (apply_correction) {
			float axillaCorrection = 0.15f * ssAxilla / mapTo;
			float deltoidCorrection = 0.15f * ssDeltoid / mapTo;
			axillaCorrection = float.IsNaN (axillaCorrection) ? 0.0f : axillaCorrection;
			deltoidCorrection = float.IsNaN (deltoidCorrection) ? 0.0f : deltoidCorrection;
			
			anteriorAngle = anteriorAngle - axillaCorrection - deltoidCorrection;
			posteriorAngle = posteriorAngle + axillaCorrection + deltoidCorrection;
		}
		
		// Make sure we return floating point numbers.
		anteriorAngle = float.IsNaN (anteriorAngle) ? 0.0f : anteriorAngle;
		posteriorAngle = float.IsNaN (posteriorAngle) ? 0.0f : posteriorAngle;
		
		if (show_debug) {
			print ("Axial angle (anterior + posterior): " + anteriorAngle + " + " + posteriorAngle);
		}
		
		return anteriorAngle + posteriorAngle;
	}
	
	//
	// Gets the frontal angle in degrees (starting from a T-pose).
  //
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_frontal_angle(StretchSensor axilla, StretchSensor clavicle, StretchSensor deltoid, StretchSensor trapezius)
	{
		int mapTo = 1000;
		float ssAxilla = axilla.getMappedReading(mapTo);
		float ssClavicle = clavicle.getMappedReading(mapTo);
		float ssDeltoid = deltoid.getMappedReading(mapTo);
		float ssTrapezius = trapezius.getMappedReading(mapTo);
		
		// Initial calculations.
		float superiorAngle = (ssAxilla / mapTo) * 90;
		float inferiorAngle = -1 * (ssDeltoid / mapTo) * 90;
		
		// Correction factors.
		if (apply_correction) {

      // TODO: describe these factors.
			float clavicleCorrection = 0.15f * ssClavicle / mapTo;
			float trapeziusCorrection = 0.15f * ssTrapezius / mapTo;
			clavicleCorrection = float.IsNaN (clavicleCorrection) ? 0.0f : clavicleCorrection;
			trapeziusCorrection = float.IsNaN (trapeziusCorrection) ? 0.0f : trapeziusCorrection;
			
			superiorAngle = superiorAngle - clavicleCorrection - trapeziusCorrection;
			inferiorAngle = inferiorAngle + clavicleCorrection + trapeziusCorrection;
		}
		
		// Make sure we return floating point numbers.
		superiorAngle = float.IsNaN(superiorAngle) ? 0.0f : superiorAngle;
		inferiorAngle = float.IsNaN(inferiorAngle) ? 0.0f : inferiorAngle;
		
		if (show_debug) {
			print ("Frontal angle (superior + inferior): " + superiorAngle + " + " + inferiorAngle);
		}
		
		return superiorAngle + inferiorAngle;
	}
	
	//
	// Gets the rotation angle in degrees (starting from a T-pose).
  //
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
	private float get_rot_angle(StretchSensor axilla, StretchSensor clavicle, StretchSensor deltoid, StretchSensor trapezius)
	{
		float angle = 0.0f;
		
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
		for (int i = 0; i < sensors.Length; i++) {
			sensors [i].UpdateSensor ();
		}
		
		// Retrieve sensor objects.
		StretchSensor axilla = getSensorByPosition (StretchSensor.positionName.Axilla);
		StretchSensor clavicle = getSensorByPosition (StretchSensor.positionName.Clavicle);
		StretchSensor deltoid = getSensorByPosition (StretchSensor.positionName.Deltoid);
		StretchSensor trapezius = getSensorByPosition (StretchSensor.positionName.Trapezius);
		
		// Update shoulder orientation.
		if (axilla != null && clavicle != null && deltoid != null && trapezius != null)
		{
			float axialAngle = get_axial_angle (axilla, clavicle, deltoid, trapezius);
			float frontalAngle = get_frontal_angle (axilla, clavicle, deltoid, trapezius);
			float rotAngle = get_rot_angle (axilla, clavicle, deltoid, trapezius);
			
			orientation_euler.y = axialAngle * orientation.y;
			orientation_euler.z = frontalAngle * orientation.z;
			orientation_euler.x = rotAngle * orientation.x;
			
			if (show_debug) {
				print("Euler angles (x / y / z): "+ orientation_euler.x +" / "+ orientation_euler.y +" / "+ orientation_euler.y);
			}
			
			joint_object.localRotation = Quaternion.Euler (orientation_euler);
		}
		
		else {
			print ("Missing shoulder sensors...");
		}
	}
}

