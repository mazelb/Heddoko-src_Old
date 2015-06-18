using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class ss_joint_torso : ss_joint
{
	//
	// Updates joint position and values.
	//
	public override void update_joint()
	{
		// Don't update anything if joint is paused.
		if (pause_animation) {
			return;
		}
		
		// Update individual sensors.
		for (int ndx = 0; ndx < sensors.Length; ndx++) {
			sensors[ndx].update_sensor();
		}
		
		// Update torso orientation.
		// ...
	}
}

