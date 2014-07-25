using UnityEngine;
using System.Collections;

public class Elbow : MonoBehaviour
{
	public float radialVelocity;
	public float linearVelocity;
	
	
	void Start ()
	{
		
	}
	
	void FixedUpdate ()
	{
		
		// Collect data from sensors
		// ...
		this.radialVelocity = 50f;
		this.linearVelocity = 10f;
		
		// Move according to data from sensors
		this.Move();
	}
	
	// Temporary method, testing motion stuff...
	void Move()
	{
		if(Input.GetKey(KeyCode.W))
			transform.Rotate (Vector3.forward, -radialVelocity * Time.deltaTime);
		
		if(Input.GetKey(KeyCode.S))
			transform.Rotate (Vector3.forward, radialVelocity * Time.deltaTime);
		
		if(Input.GetKey(KeyCode.A))
			transform.Rotate(Vector3.up, radialVelocity * Time.deltaTime);
		
		if (Input.GetKey(KeyCode.D))
			transform.Rotate (Vector3.up, -radialVelocity * Time.deltaTime);
	}
}
