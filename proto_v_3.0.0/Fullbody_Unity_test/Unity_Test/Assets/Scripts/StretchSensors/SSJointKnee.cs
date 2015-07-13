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

public class SSJointKnee : SSJoint
{
    /**
     * @brief                   Converts StretchSense data into an knee angle in degrees.
     * @param StretchSensor     StrechSensor attached to knee.
     * @return float            Knee angle in degrees.
     */
	private float GetKneeAngle(StretchSensor vKnee)
	{
		int vMapTo = 1000;
		float vValue = vKnee.GetMappedReading(vMapTo);
		float vAngle = (vValue / vMapTo) * (180 - 39);
		vAngle = float.IsNaN(vAngle) ? 0.0f : vAngle;

		if (vShowDebug)
        {
		    print("Knee angle: "+ vAngle);
		}
		
		return vAngle;
	}

    /**
     * @brief           Updates joint position and values.
     * @return void
     */
    public override void UpdateJoint()
    {
        // Don't update anything if joint is paused.
        if (vPauseAnimation)
        {
            return;
        }

        // Update individual sensors.
        for (int i = 0; i < maSensors.Length; i++)
        {
            maSensors[i].UpdateSensor();
        }

        // Update knee angle.
        StretchSensor vKnee = maSensors[0];
        if (vKnee != null)
        {
            mOrientationEuler = GetKneeAngle(vKnee) * vRotationalDirections;
            vJointObject.localRotation = Quaternion.Euler(mOrientationEuler);
        }
    }
}

