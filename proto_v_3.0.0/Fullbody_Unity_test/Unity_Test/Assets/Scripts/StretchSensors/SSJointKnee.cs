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
     * @brief                   Gets the bending angle of the knee in degrees.
     * @param StretchSensor     StrechSensor attached to knee.
     * @return float            Bending angle of the knee in degrees.
     */
	private float GetExtAngle(StretchSensor vKnee)
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
     * @brief                   Gets the rotational angle of the knee in degrees.
     * @param StretchSensor     StrechSensor attached to knee.
     * @return float            Rotational angle of the knee in degrees.
     */
    private float GetSupAngle(StretchSensor vKnee)
    {
        float vAngle = 0.0f;
        
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
            mOrientationEuler.x = GetExtAngle(vKnee) * vRotationalDirections.x;
            mOrientationEuler.y = GetSupAngle(vKnee) * vRotationalDirections.y;
            vJointObject.localRotation = Quaternion.Euler(mOrientationEuler);
        }
    }
}

