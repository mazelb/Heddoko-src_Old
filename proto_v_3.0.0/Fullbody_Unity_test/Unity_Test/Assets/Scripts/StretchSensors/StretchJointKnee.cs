using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJointKnee : StretchJoint
{
	//
	// Converts StretchSense data into a knee angle (starting from a T-pose).
	//
	private float getElbowAngle(StretchSensor knee)
	{
		int mapTo = 1000;
		float ssValue = knee.getMappedReading(mapTo);
		float angle = (ssValue / mapTo) * (180 - 39);
		angle = float.IsNaN (angle) ? 0.0f : angle;

		if (showDebug) {
		    print("Knee angle: "+ angle);
		}
		
		return angle;
	}

	//
    // Updates joint position and values.
	//
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
}

