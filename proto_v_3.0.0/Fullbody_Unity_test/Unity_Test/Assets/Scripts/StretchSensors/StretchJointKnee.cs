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
			mCurJointRotationEuler = knee.getCurAngleReading() * rotatesXYZ;
			jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);
        }
    }
}

