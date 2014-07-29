using UnityEngine;
using System.Collections;

abstract public class Move
{
	// Calculates the arc-tangent
	// and checks that we're not dividing by zero
	public float Atan(float a, float b)
	{
		// Make sure we're not dividing by zero
		if (b == 0.0f) {
			// Do something...
		}

		// Return arc-tangent
		return Mathf.Atan (a / b);
	}

	// Converts acceleration data into angles
	// NOTE: use g-scale for acceleration
	public void AccelToDegrees (float[] acc, ref float[] deg)
	{
		// To do: apply a low-pass filter
		float ax = acc [0];
		float ay = acc [1];
		float az = acc [2];

		// Pitch (about x-axis)
		deg[0]	= this.Atan ( ax, Mathf.Sqrt(ay*ay + az*az) ) * Mathf.Rad2Deg;

		// Roll (about y-axis)
		deg[1]	= this.Atan ( ay, Mathf.Sqrt(ax*ax + az*az) ) * Mathf.Rad2Deg;

		// Yaw (about z-axis)
		deg[2]	= this.Atan ( Mathf.Sqrt(ax*ax + ay*ay), az ) * Mathf.Rad2Deg;
	}
}
