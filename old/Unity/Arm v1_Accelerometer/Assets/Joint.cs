using UnityEngine;
using System.Collections;

abstract public class Joint : MonoBehaviour
{
	public float radialVelocity;
	public float linearVelocity;

	abstract public void Start ();
	abstract public void FixedUpdate ();

	// Read serial data
	public void ReadSerial()
	{


		// Todo...
	}
	
	// Convert accelerometer value to angle
	public float AccelToGyro(float accel)
	{
		float angle = 0f;
		
		
		
		return angle;
	}

	public void Move()
	{
		// Collect data from sensors...
		this.radialVelocity = 50f;
		this.linearVelocity = 10f;

		// Todo
		if(Input.GetKey(KeyCode.UpArrow))
			transform.Rotate (Vector3.forward, -radialVelocity * Time.deltaTime);
		
		if(Input.GetKey(KeyCode.DownArrow))
			transform.Rotate (Vector3.forward, radialVelocity * Time.deltaTime);
		
		if(Input.GetKey(KeyCode.LeftArrow))
			transform.Rotate(Vector3.up, radialVelocity * Time.deltaTime);
		
		if (Input.GetKey(KeyCode.RightArrow))
			transform.Rotate (Vector3.up, -radialVelocity * Time.deltaTime);
	}
}
