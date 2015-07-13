using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class SSJointHip : SSJoint
{
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

        // Update hip orientation.
        // ...
    }
}

