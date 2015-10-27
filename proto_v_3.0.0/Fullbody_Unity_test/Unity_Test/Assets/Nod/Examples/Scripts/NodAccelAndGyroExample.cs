/*
 * Copyright 2014 Nod Labs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Nod;

public class NodAccelAndGyroExample : NodExampleBase
{
	private Rigidbody body;
	private Vector3 initialPosition;

	bool firstValueRead = false;

	public Transform red;
	public Transform green;
	public Transform blue;

	private Vector3 forceFromNodDevice = Vector3.zero;
	private Quaternion nodDeviceOrientation = Quaternion.identity;
	private Vector3 forceOnGameObject = Vector3.zero;

	private List<Vector3> storedForces = new List<Vector3>();
	private Vector3 previousRawForce = Vector3.zero;
	private Quaternion inverseInitialRotation = Quaternion.identity;

	private void recenter()
	{
		inverseInitialRotation = Quaternion.Inverse(nodDevice.rotation);
	}

	public void OnEnable()
	{
		initialPosition = transform.position;
		body = GetComponent<Rigidbody>();

		nodSubscribtionList = new NodSubscriptionType []
		{
			NodSubscriptionType.EulerMode,
			NodSubscriptionType.AccelMode,
			NodSubscriptionType.GyroMode
		};

		nod = NodController.GetNodInterface();
	}

	public void Update()
	{
		if (!NodDeviceConnectedAndInitialized())
			return;

		if (Input.GetKeyDown(KeyCode.Space)) {
			//reset everything
			recenter();
			transform.position = initialPosition;
			body.velocity = Vector3.zero;
		}

		if (!firstValueRead &&
		    nodDevice.acceleration.accelX != 0 &&
		    nodDevice.acceleration.accelY != 0 &&
		    nodDevice.acceleration.accelZ != 0)
		{
			firstValueRead = true;
			recenter();
		}

		bool rawForceChanged = false;
		if (nodDevice.acceleration.accelX != previousRawForce.x ||
		    nodDevice.acceleration.accelY != previousRawForce.y ||
		    nodDevice.acceleration.accelZ != previousRawForce.z)
		{
			rawForceChanged = true;
			previousRawForce = new Vector3(nodDevice.acceleration.accelX,
			                               nodDevice.acceleration.accelY,
			                               nodDevice.acceleration.accelZ);
		}

		//The acceleration is defined in terms of units of gravity
		forceFromNodDevice = new Vector3(nodDevice.acceleration.accelX, nodDevice.acceleration.accelY, nodDevice.acceleration.accelZ);
		nodDeviceOrientation = nodDevice.rotation;

		forceFromNodDevice = nodDeviceOrientation * forceFromNodDevice;
		if (firstValueRead) {
			//Once transformed we can back out gravity by subtracting 1.0 from the z axis
			forceFromNodDevice.z -= 1.0f;
		}
		forceFromNodDevice = inverseInitialRotation * forceFromNodDevice;

		//Get the force in terms of meters per second per second to conform to Unity's physics system
		forceFromNodDevice *= 9.8f;

		float scaleFactor = 0.2f;

		Vector3 scale = red.localScale;
		scale.y =  forceFromNodDevice.x * scaleFactor;
		red.localScale = scale;

		scale = green.localScale;
		scale.y =  forceFromNodDevice.y * scaleFactor;
		green.localScale = scale;

		scale = blue.localScale;
		scale.y =  forceFromNodDevice.z * scaleFactor;
		blue.localScale = scale;


		//The force reported by the nod device is the force imparted on the nod device so you need to reverse all the components of the force
		//that the nod device reported to have the nod device move in a similar direction.
		forceOnGameObject = new Vector3(-forceFromNodDevice.x, -forceFromNodDevice.y, -forceFromNodDevice.z);

		if (rawForceChanged) {
			storedForces.Add(forceOnGameObject);
		}
	}

	public void FixedUpdate()
	{
		foreach (Vector3 vec in storedForces)
			body.AddForce(vec);
		storedForces = new List<Vector3>();

		body.rotation = inverseInitialRotation * nodDeviceOrientation;
	}

	public void OnGUI()
	{
		BaseNodOnGUI();
	}
}
