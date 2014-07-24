using UnityEngine;
using System.Collections;

public class Shoulder : MonoBehaviour
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
