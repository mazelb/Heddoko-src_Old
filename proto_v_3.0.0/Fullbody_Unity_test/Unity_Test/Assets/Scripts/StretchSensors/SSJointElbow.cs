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

public class SSJointElbow : SSJoint
{
    /**
     * @brief               Converts StretchSense data into an elbow angle in degrees.
     * @param StretchSensor StretchSensor attached to elbow.
     * @return float        Elbow angle in degrees.
     */
    private float GetElbowAngle(StretchSensor vElbow)
    {
        int vMapTo = 1000;
        float vValue = vElbow.GetMappedReading(vMapTo);
        float vAngle = (vValue / vMapTo) * (180 - 39);

        if (vShowDebug)
        {
            print ("Elbow: " + vAngle);
        }
        
        // Make sure we return floating point numbers.
        vAngle = float.IsNaN(vAngle) ? 0.0f : vAngle;

        return vAngle;
    }
	
  /**
   * @brief                 Converts StretchSense data into forearm orientation in degrees.
   * @param StretchSensor   StretchSensor attached to forearm.
   * @return float          Forearm orientation in degrees.
   */
	private float GetForearmAngle(StretchSensor vForearm)
	{
		int vMapTo = 1000;
		float vValue = vForearm.GetMappedReading(vMapTo);
        float vAngle = (vValue / vMapTo) * 210 - 140;

		if (vShowDebug)
        {
			print ("Forearm: " + vAngle);
        }
        
        // Make sure we return floating point numbers.
        vAngle = float.IsNaN(vAngle) ? 0.0f : vAngle;
		
		return vAngle;
	}

    /**
     * @brief          Updates joint position and values.
     * @return void
     */
    public override void UpdateJoint()
    {
        // Don't update anything if joint is paused.
        if (vPauseAnimation)
        {
            return;
        }

        // Update individual sensor data.
        for (int ndx = 0; ndx < maSensors.Length; ndx++)
        {
            maSensors[ndx].UpdateSensor();
        }

        // Retrieve sensor objects.
        StretchSensor vElbow = GetSensorByPosition(StretchSensor.PositionName.Elbow);
        StretchSensor vForearm = GetSensorByPosition(StretchSensor.PositionName.Forearm);

        // Update angles
        if (vElbow != null && vForearm != null)
        {
            float vElbowAngle = GetElbowAngle(vElbow);
            float vForearmAngle = GetForearmAngle(vForearm);

            mOrientationEuler.y = vElbowAngle * vRotationalDirections.y;
            mOrientationEuler.x = vForearmAngle * vRotationalDirections.x;

            vJointObject.localRotation = Quaternion.Euler(mOrientationEuler);
        }
    }
}

