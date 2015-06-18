using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class ss_joint_hip : ss_joint
{
  public override void update_joint()
  {
    // Don't update anything if joint is paused.
    if (pause_animation) {
      return;
    }

    // Update individual sensors.
    for (int i = 0; i < sensors.Length; i++) {
      sensors[i].update_sensor();
    }

    // Update hip orientation.
    // ...
  }
}

