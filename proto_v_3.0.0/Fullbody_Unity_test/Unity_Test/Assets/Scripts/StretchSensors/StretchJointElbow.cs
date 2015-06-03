using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJointElbow : StretchJoint
{
    // Updates joint position and values.
    public override void UpdateJoint()
    {
	    // Don't update anything if joint is paused.
	    if (pauseAnimation) {
	        return;
	    }

        // Update individual sensor data.
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) {
			mStretchSensors[ndx].UpdateSensor();
		}

        // Retrieve sensor objects.
        StretchSensor elbow = getSensorByPosition(StretchSensor.ssPositionName.Elbow);
        StretchSensor forearm = getSensorByPosition(StretchSensor.ssPositionName.Forearm);

        // Update angles
        if (elbow != null && forearm != null)
        {
            float elbowAngle = elbow.getCurAngleReading();
            float forearmAngle = forearm.getCurAngleReading();

            mCurJointRotationEuler.y = elbowAngle * rotatesXYZ.y;
            mCurJointRotationEuler.x = forearmAngle * rotatesXYZ.x;

            jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);
        }
    }
}

