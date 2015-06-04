using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJointKnee : StretchJoint
{
    // Updates joint position and values.
    public override void UpdateJoint()
    {
	    // Don't update anything if joint is paused.
	    if (pauseAnimation) {
	        return;
	    }

        // Update individual sensors.
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) {
			mStretchSensors[ndx].UpdateSensor();
		}

        // Update knee angle
        StretchSensor knee = mStretchSensors[0];
        if (knee != null)
		{
			mCurJointRotationEuler = getElbowAngle(knee) * rotatesXYZ;
			jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);
        }
	}
	
	//
	// Converts StretchSense data into a knee angle (starting from a T-pose).
	//
	private float getElbowAngle(StretchSensor knee)
	{
		int mapTo = 1000;
		float ssValue = knee.getMappedReading(mapTo);
		float angle = 180 - ( (knee.maxValue - ssValue) / mapTo ) * (180 - 35) + 35;
		
		return angle;
	}
}

