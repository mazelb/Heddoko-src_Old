using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class SSJointTorso : SSJoint
{
	/**
     * Updates joint position and values.
     */
    public override void UpdateJoint()
    {
    	// Don't update anything if joint is paused.
    	if (vPauseAnimation) {
    		return;
    	}
    	
    	// Update individual sensors.
    	for (int ndx = 0; ndx < maSensors.Length; ndx++) {
    		maSensors[ndx].UpdateSensor();
    	}
    	
    	// Update torso orientation.
    	// ...
    }
}

