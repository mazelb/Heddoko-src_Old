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
        StretchSensor elbow = getSensorByPosition(StretchSensor.positionName.Elbow);
        StretchSensor forearm = getSensorByPosition(StretchSensor.positionName.Forearm);

        // Update angles
        if (elbow != null && forearm != null)
        {
            float elbowAngle = getElbowAngle(elbow);
			float forearmAngle = getForearmAngle(forearm);

            mCurJointRotationEuler.y = elbowAngle * rotatesXYZ.y;
            mCurJointRotationEuler.x = forearmAngle * rotatesXYZ.x;

            jointTransform.localRotation = Quaternion.Euler(mCurJointRotationEuler);
        }
	}
	
	//
	// Converts StretchSense data into an elbow angle (starting from a T-pose).
	//
	private float getElbowAngle(StretchSensor elbow)
	{
		int mapTo = 1000;
		float ssValue = elbow.getMappedReading(mapTo);
		float angle = 180 - ( (elbow.maxValue - ssValue) / mapTo ) * (180 - 15) + 15;
		
		return angle;
	}
	
	//
	// Converts StretchSense data into forearm orientation (starting from a T-pose).
	//
	private float getForearmAngle(StretchSensor forearm)
	{
		float angle = 0.0f;
		
		return angle;
	}
}

