using UnityEngine;
using System.Collections;
using Nod;

public class NodJoystickAndTriggerExample : NodExampleBase
{
	public Transform playerRoot;
	public Transform triggerRoot;

	private Rigidbody playerRigidBody;

	public void Awake()
	{
		nodSubscribtionList = new NodSubscriptionType []
		{
			NodSubscriptionType.GameControlMode
		};

		playerRigidBody = playerRoot.GetComponent<Rigidbody>();

		//This will create a GameObject in your Hierarchy called "NodController" which will manage
		//interactions with all connected nod devices.  It will presist between scene loads.  Only
		//one instance will be created if you request a nod interface from multiple locations
		//in your code.
		nod = NodController.GetNodInterface();
	}

	public void Update()
	{
		if (!NodDeviceConnectedAndInitialized())
			return;

		//Move the player using the joystick
		const float speed = 2.0f;
		Vector3 movementRight = Vector3.right * speed * Time.deltaTime * nodDevice.joyStickPosition.x;
		Vector3 movementForward = Vector3.forward * speed * Time.deltaTime * nodDevice.joyStickPosition.y;
		Vector3 totalOffset = movementRight + movementForward;
		playerRigidBody.MovePosition(playerRoot.position + totalOffset);

		//Move the pillers on the side for the trigger
		const float pulledAngle = 90.0f;
		const float neutralAngle = 0.0f;
		float interpolatedAngle = (1.0f - nodDevice.triggerPressure) * neutralAngle +
								  (nodDevice.triggerPressure) * pulledAngle;

		triggerRoot.localEulerAngles = new Vector3(interpolatedAngle, 0.0f, 0.0f);
	}

	public void OnGUI()
	{
		BaseNodOnGUI();
    }
}