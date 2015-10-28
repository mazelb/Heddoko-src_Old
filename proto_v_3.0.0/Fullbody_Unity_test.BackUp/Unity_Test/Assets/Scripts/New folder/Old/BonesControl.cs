using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.IO;


[RequireComponent(typeof(Animator))] 

public class BonesControl : MonoBehaviour 
{
	public static nodController nc = new nodController();
	public Boolean isPlayRecording = false;
	private Boolean isInitialized = false;

	private float initXDisplacement = 0.0f;
	private short lastButtonState = 0;
	
	//Arms transforms
	public Transform lowerSpineTransform = null;
	public Transform rightUpperArmTransform = null;
	public Transform rightForeArmTransform = null;
	public Transform leftUpperArmTransform = null;
	public Transform leftForeArmTransform = null;
	public Transform upperSpineTransform = null;
	public Transform rightThighTransform = null;
	public Transform rightCalfTransform = null;
	public Transform leftCalfTransform = null;
	public Transform leftThighTransform = null;
	
	//Quaternion targets
	private Quaternion rightUpperArmTarget;
	private Quaternion rightForeArmTarget;
	private Quaternion leftUpperArmTarget;
	private Quaternion leftForeArmTarget;
	private Quaternion upperSpineTarget;
	private Quaternion rightThighTarget;
	private Quaternion rightCalfTarget;
	private Quaternion leftCalfTarget;
	private Quaternion leftThighTarget;
	
	//Vector Initial targets
	private Vector3 lowerSpineInitPos;
	private Vector3 rightUpperArmInitEulers;
	private Vector3 rightForeArmInitEulers;
	private Vector3 leftUpperArmInitEulers;
	private Vector3 leftForeArmInitEulers;
	private Vector3 upperSpineInitEulers;
	private Vector3 rightThighInitEulers;
	private Vector3 rightCalfInitEulers;
	private Vector3 leftThighInitEulers;
	private Vector3 leftCalfInitEulers;
	private Quaternion rightUpperArmInitQuat;
	private Quaternion rightForeArmInitQuat;
	private Quaternion leftUpperArmInitQuat;
	private Quaternion leftForeArmInitQuat;
	private Quaternion upperSpineInitQuat;
	private Quaternion rightThighInitQuat;
	private Quaternion rightCalfInitQuat;
	private Quaternion leftCalfInitQuat;
	private Quaternion leftThighInitQuat;
	private NodQuaternionOrientation rightUpperArmInitNodQuat;
	private NodQuaternionOrientation rightForeArmInitNodQuat;
	private NodQuaternionOrientation leftUpperArmInitNodQuat;
	private NodQuaternionOrientation leftForeArmInitNodQuat;
	private NodQuaternionOrientation upperSpineInitNodQuat;
	private NodQuaternionOrientation rightThighInitNodQuat;
	private NodQuaternionOrientation rightCalfInitNodQuat;
	private NodQuaternionOrientation leftCalfInitNodQuat;
	private NodQuaternionOrientation leftThighInitNodQuat;

	//Vector Current targets
	private Vector3 lowerSpineCurrentPos;
	private Vector3 rightUpperArmCurrentEulers;
	private Vector3 rightForeArmCurrentEulers;
	private Vector3 leftUpperArmCurrentEulers;
	private Vector3 leftForeArmCurrentEulers;
	private Vector3 upperSpineCurrentEulers;
	private Vector3 rightThighCurrentEulers;
	private Vector3 rightCalfCurrentEulers;
	private Vector3 leftThighCurrentEulers;
	private Vector3 leftCalfCurrentEulers;
	private Quaternion rightUpperArmCurrentQuat;
	private Quaternion rightForeArmCurrentQuat;
	private Quaternion leftUpperArmCurrentQuat;
	private Quaternion leftForeArmCurrentQuat;
	private Quaternion upperSpineCurrentQuat;
	private Quaternion rightThighCurrentQuat;
	private Quaternion rightCalfCurrentQuat;
	private Quaternion leftCalfCurrentQuat;
	private Quaternion leftThighCurrentQuat;
	private NodQuaternionOrientation rightUpperArmCurrentNodQuat;
	private NodQuaternionOrientation rightForeArmCurrentNodQuat;
	private NodQuaternionOrientation leftUpperArmCurrentNodQuat;
	private NodQuaternionOrientation leftForeArmCurrentNodQuat;
	private NodQuaternionOrientation upperSpineCurrentNodQuat;
	private NodQuaternionOrientation rightThighCurrentNodQuat;
	private NodQuaternionOrientation rightCalfCurrentNodQuat;
	private NodQuaternionOrientation leftCalfCurrentNodQuat;
	private NodQuaternionOrientation leftThighCurrentNodQuat;
	
	
	public struct NodQuaternionOrientation
	{
		public float x;
		public float y;
		public float z;
		public float w;
		
		public NodQuaternionOrientation(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
	};

	private float mapRange(float a1,float a2,float b1,float b2,float s)
	{
		return b1 + (s-a1)*(b2-b1)/(a2-a1);
	}
	
	private void ConnectSensor(int vIdx) 
	{
		Boolean vExists = nc.indexExist (vIdx);
		if (vExists) 
		{
			nc.connect6DSensor(vIdx);
		}
	}
	
	private void ShutdownSensor(int vIdx) 
	{
		Boolean vExists = nc.indexExist (vIdx);
		if (vExists) 
		{
			nc.shutDown6DSensor(vIdx);
		}
	}
	
	private Vector3 getEulerOrientation(int vIdx)
	{
		Vector3 vEulerVector = Vector3.zero;
		
		if (nc.indexExist (vIdx))
		{
			vEulerVector.x =nc.getSensorLatestPitch (vIdx);// * 360 / Mathf.PI;
			//vEulerVector.x = mapRange(-180, 180, 0, 360, vEulerVector.x);
			vEulerVector.y = nc.getSensorLatestRoll (vIdx);// * 360 / Mathf.PI;
			//vEulerVector.z = mapRange(-360, 360, 0, 360, vEulerVector.z);
			vEulerVector.z = nc.getSensorLatestYaw (vIdx);// * 360 / Mathf.PI;
			//vEulerVector.y = mapRange(-180, 180, 0, 360, vEulerVector.y);
		}
		
		return vEulerVector;
	}

	public void ReceiveRecordedMove ()
	{
		nc.sendRecordedMovement();
	}

	public void ResetOrientations()
	{
		//if (nc.getNumberConnectedDevices () >= 1 && !isPlayRecording)
		if (nc.getNumberConnectedDevices () >= 1)
		{
			lowerSpineCurrentPos 		= Vector3.zero;
			upperSpineCurrentEulers 	= Vector3.zero;
			rightUpperArmCurrentEulers 	= Vector3.zero;
			rightForeArmCurrentEulers 	= Vector3.zero;
			leftUpperArmCurrentEulers 	= Vector3.zero;
			leftForeArmCurrentEulers 	= Vector3.zero;
			rightThighCurrentEulers 	= Vector3.zero;
			rightCalfCurrentEulers 		= Vector3.zero;
			leftThighCurrentEulers 		= Vector3.zero;
			leftCalfCurrentEulers 		= Vector3.zero;
			
			rightUpperArmTarget = Quaternion.identity;
			rightForeArmTarget 	= Quaternion.identity;
			upperSpineTarget 	= Quaternion.identity;
			leftUpperArmTarget 	= Quaternion.identity;
			leftForeArmTarget 	= Quaternion.identity;
			rightThighTarget 	= Quaternion.identity;
			rightCalfTarget 	= Quaternion.identity;
			leftThighTarget 	= Quaternion.identity;
			leftCalfTarget 		= Quaternion.identity;			
			
			lowerSpineInitPos			  		 = new Vector3(0.0f, 1.0f, 0.0f);
			lowerSpineTransform.localPosition 	 = lowerSpineInitPos;
			upperSpineTransform.localRotation 	 = upperSpineTarget;
			rightUpperArmTransform.localRotation = rightUpperArmTarget;
			rightForeArmTransform.localRotation  = rightForeArmTarget;			
			leftUpperArmTransform.localRotation  = leftUpperArmTarget;
			leftForeArmTransform.localRotation   = leftForeArmTarget;			
			rightThighTransform.localRotation 	 = rightThighTarget;
			rightCalfTransform.localRotation 	 = rightCalfTarget;			
			leftThighTransform.localRotation 	 = leftThighTarget;
			leftCalfTransform.localRotation 	 = leftCalfTarget;
			
			rightUpperArmInitEulers = getEulerOrientation(0);
			rightForeArmInitEulers = getEulerOrientation(1);
			upperSpineInitEulers = getEulerOrientation(2);
			leftUpperArmInitEulers = getEulerOrientation(4);
			leftForeArmInitEulers = getEulerOrientation(5);
			rightThighInitEulers = getEulerOrientation(6);
			rightCalfInitEulers = getEulerOrientation(7);
			leftThighInitEulers = getEulerOrientation(8);
			leftCalfInitEulers = getEulerOrientation(9);
			
			rightUpperArmInitNodQuat = eulerToQuaternion(rightUpperArmInitEulers.x, -rightUpperArmInitEulers.y, -rightUpperArmInitEulers.z);
			rightForeArmInitNodQuat = eulerToQuaternion(rightForeArmInitEulers.x, -rightForeArmInitEulers.y, -rightForeArmInitEulers.z);
			leftUpperArmInitNodQuat = eulerToQuaternion(-leftUpperArmInitEulers.x, -leftUpperArmInitEulers.y, leftUpperArmInitEulers.z);
			leftForeArmInitNodQuat = eulerToQuaternion(-leftForeArmInitEulers.x, -leftForeArmInitEulers.y, leftForeArmInitEulers.z);
			upperSpineInitNodQuat = eulerToQuaternion(upperSpineInitEulers.x, upperSpineInitEulers.y, upperSpineInitEulers.z);
			rightThighInitNodQuat = eulerToQuaternion(rightThighInitEulers.x, -rightThighInitEulers.y, -rightThighInitEulers.z);
			rightCalfInitNodQuat = eulerToQuaternion(rightCalfInitEulers.x, -rightCalfInitEulers.y, -rightCalfInitEulers.z);
			leftThighInitNodQuat = eulerToQuaternion(leftThighInitEulers.x, -leftThighInitEulers.y, -leftThighInitEulers.z);
			leftCalfInitNodQuat = eulerToQuaternion(leftCalfInitEulers.x, -leftCalfInitEulers.y, -leftCalfInitEulers.z);
			
			rightUpperArmInitQuat = new Quaternion(rightUpperArmInitNodQuat.x, rightUpperArmInitNodQuat.y, rightUpperArmInitNodQuat.z, rightUpperArmInitNodQuat.w);
			rightForeArmInitQuat = new Quaternion(rightForeArmInitNodQuat.x, rightForeArmInitNodQuat.y, rightForeArmInitNodQuat.z, rightForeArmInitNodQuat.w);
			leftUpperArmInitQuat = new Quaternion(leftUpperArmInitNodQuat.x, leftUpperArmInitNodQuat.y, leftUpperArmInitNodQuat.z, leftUpperArmInitNodQuat.w);
			leftForeArmInitQuat = new Quaternion(leftForeArmInitNodQuat.x, leftForeArmInitNodQuat.y, leftForeArmInitNodQuat.z, leftForeArmInitNodQuat.w);
			upperSpineInitQuat = new Quaternion(upperSpineInitNodQuat.x, upperSpineInitNodQuat.y, upperSpineInitNodQuat.z, upperSpineInitNodQuat.w);
			rightThighInitQuat = new Quaternion(rightThighInitNodQuat.x, rightThighInitNodQuat.y, rightThighInitNodQuat.z, rightThighInitNodQuat.w);
			rightCalfInitQuat = new Quaternion(rightCalfInitNodQuat.x, rightCalfInitNodQuat.y, rightCalfInitNodQuat.z, rightCalfInitNodQuat.w);
			leftThighInitQuat = new Quaternion(leftThighInitNodQuat.x, leftThighInitNodQuat.y, leftThighInitNodQuat.z, leftThighInitNodQuat.w);
			leftCalfInitQuat = new Quaternion(leftCalfInitNodQuat.x, leftCalfInitNodQuat.y, leftCalfInitNodQuat.z, leftCalfInitNodQuat.w);
			
			rightUpperArmInitQuat = Quaternion.Inverse(rightUpperArmInitQuat * Quaternion.Inverse(Quaternion.Euler(90,90,0)));
			rightForeArmInitQuat = Quaternion.Inverse(rightForeArmInitQuat * Quaternion.Inverse(Quaternion.Euler(90,90,0)));
			leftUpperArmInitQuat = Quaternion.Inverse(leftUpperArmInitQuat * Quaternion.Inverse(Quaternion.Euler(90,90,0)));
			leftForeArmInitQuat = Quaternion.Inverse(leftForeArmInitQuat * Quaternion.Inverse(Quaternion.Euler(90,90,0)));
			upperSpineInitQuat = Quaternion.Inverse(upperSpineInitQuat * Quaternion.Inverse(Quaternion.Euler(0,90,0)));
			rightThighInitQuat = Quaternion.Inverse(rightThighInitQuat * Quaternion.Inverse(Quaternion.Euler(0,90,180)));
			rightCalfInitQuat = Quaternion.Inverse(rightCalfInitQuat * Quaternion.Inverse(Quaternion.Euler(0,90,180)));
			leftThighInitQuat = Quaternion.Inverse(leftThighInitQuat * Quaternion.Inverse(Quaternion.Euler(0,90,180)));
			leftCalfInitQuat = Quaternion.Inverse(leftCalfInitQuat * Quaternion.Inverse(Quaternion.Euler(0,90,180)));
		}
	}
	
	private static NodQuaternionOrientation eulerToQuaternion(float pitch, float roll, float yaw)
	{
		float sinHalfYaw = Mathf.Sin(yaw / 2.0f);
		float cosHalfYaw = Mathf.Cos(yaw / 2.0f);
		float sinHalfPitch = Mathf.Sin(pitch / 2.0f);
		float cosHalfPitch = Mathf.Cos(pitch / 2.0f);
		float sinHalfRoll = Mathf.Sin(roll / 2.0f);
		float cosHalfRoll = Mathf.Cos(roll / 2.0f);
		
		NodQuaternionOrientation result;
		result.x = -cosHalfRoll * sinHalfPitch * sinHalfYaw
			+ cosHalfPitch * cosHalfYaw * sinHalfRoll;
		result.y = cosHalfRoll * cosHalfYaw * sinHalfPitch
			+ sinHalfRoll * cosHalfPitch * sinHalfYaw;
		result.z = cosHalfRoll * cosHalfPitch * sinHalfYaw
			- sinHalfRoll * cosHalfYaw * sinHalfPitch;
		result.w = cosHalfRoll * cosHalfPitch * cosHalfYaw
			+ sinHalfRoll * sinHalfPitch * sinHalfYaw;
		
		return result;
	}//*/
	
	// Use this for initialization
	void Start () 
	{
		//DontDestroyOnLoad(transform.gameObject);

		nc.Idx = "bones"; 

		if (isPlayRecording) 
		{
			Debug.Log("!!!!! Start Bone Recording !!!!! " + nc.movement.Count);
			//nc.sendRecordedMovement();
			nc.realTimeTracking = true;
			nc.playRecording = false;
			nc.stopRecording();
			nc.dummyMode = true;
			isInitialized=true;
			nc.prepareOrientations();
			ResetOrientations();
			StartCoroutine(nc.readFromAllSensors(.05f));
		} 
		else 
		{
			Debug.Log("!!!!! Start Bone RealTime !!!!!");

			//FIXME.  fill in the following order
			/*   
	    	rightUpperArmInitEulers = getEulerOrientation(0);
	        rightForeArmInitEulers = getEulerOrientation(1);        
	        upperSpineInitEulers = getEulerOrientation(2);
	        lowerSpineInitEulers = getEulerOrientation(3);
	        leftUpperArmInitEulers = getEulerOrientation(4);
	        leftForeArmInitEulers = getEulerOrientation(5);
	        rightThighInitEulers = getEulerOrientation(6);
	        rightCalfInitEulers = getEulerOrientation(7);
	        leftThighInitEulers = getEulerOrientation(8);
	        leftCalfInitEulers = getEulerOrientation(9);
			*/
			
			//Full body
			//nc.setSensorNames("nod-3b8","nod-47a","nod-425","nod-553","nod-55d","nod-3fc","nod-387","nod-427","nod-428","nod-380");
			//nc.setSensorNames("nod-272","nod-5ac","nod-2cd","nod-55c","nod-1f2","nod-421","nod-274","nod-25d","nod-2cb","nod-2a5");
			//nc.setSensorNames("nod-272","nod-25d","nod-2cd","nod-55c","nod-1f2","nod-2a5","nod-274","nod-27c","nod-2cb","nod-421");
			//nc.setSensorNames("nod-272","nod-25d","nod-2cd","nod-55c","nod-1f2","nod-2e8","nod-274","nod-27c","nod-2cb","nod-421");
			
			nc.realTimeTracking = true;
			nc.playRecording = false;
			nc.dummyMode = false;

			nc.setSensorNames("nod-25d","nod-45a","nod-2cd","nod-55c","nod-1f2","nod-2e8","nod-274","nod-27c","nod-2cb","nod-421");
			
			StartCoroutine(nc.readFromAllSensors(.05f));	
		}
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (!isInitialized) 
		{
			isInitialized = true;
			ResetOrientations ();
		}
	}
	
	void OnApplicationQuit() 
	{
		for (int i = 0; i < 10; i++) 
		{
			ShutdownSensor(i);
		}
	}
	
	void LateUpdate()
	{
		//Debug.Log("Connected devices " + nc.getNumberConnectedDevices () );
		//if (nc.getNumberConnectedDevices () >= 1 && isInitialized && !isPlayRecording)
		if (nc.getNumberConnectedDevices () >= 1 && isInitialized)
		{
			//Debug.Log("Connected devices " + nc.getNumberConnectedDevices () );

			/////////////////////////////////////////////
			/// reset orientations on button press:
			///////////////////////////////////////////// 
			string btnState = nc.getSensorLatestButtonState(3);
			if(btnState != null)
			{
				byte[] Values = System.Convert.FromBase64String(btnState);// GetBytes(parts[2]);
				short button =(short) ( (short)Values[0] | (short)(Values[1] << 8) );
				short touch0 =(short) ( button & 0x3 );
				
				if(lastButtonState != touch0) 
				{
					//Ready();
					ResetOrientations();
				}
				
				lastButtonState = touch0;
				//short touch1 =(short) ( (button >> 2) & 0x3 ) ;
				//short touch2 =(short) ( (button >> 4) & 0x3 );
				//short tact0 =(short) ( (button >> 6) & 0x3 );
				//short tact1 =(short) ( (button >> 8) & 0x3 );
				//Debug.Log("( " + touch0 + " , " + touch1 + " , " + touch2 + " , " + tact0 + " , " + tact1 + " )" );
			}
			
			/////////////////////////////////////////////
			/// Get raw orientations:
			///////////////////////////////////////////// 
			rightUpperArmCurrentEulers 	= getEulerOrientation(0);
			rightForeArmCurrentEulers 	= getEulerOrientation(1);
			upperSpineCurrentEulers 	= getEulerOrientation(2);
			leftUpperArmCurrentEulers 	= getEulerOrientation(4);
			leftForeArmCurrentEulers 	= getEulerOrientation(5);
			//Debug.Log("( " + leftForeArmCurrentEulers.x + " , " + leftForeArmCurrentEulers.y + " , " + leftForeArmCurrentEulers.z + " )" );
			
			rightThighCurrentEulers 	= getEulerOrientation(6);
			rightCalfCurrentEulers 		= getEulerOrientation(7);
			leftThighCurrentEulers 		= getEulerOrientation(8);
			leftCalfCurrentEulers 		= getEulerOrientation(9);
			
			/////////////////////////////////////////////
			/// Adjust  orientations directions 
			/// and get Quat output:
			///////////////////////////////////////////// 
			
			rightUpperArmCurrentNodQuat = eulerToQuaternion(rightUpperArmCurrentEulers.x, -rightUpperArmCurrentEulers.y, -rightUpperArmCurrentEulers.z);
			rightForeArmCurrentNodQuat = eulerToQuaternion(rightForeArmCurrentEulers.x, -rightForeArmCurrentEulers.y, -rightForeArmCurrentEulers.z);
			leftUpperArmCurrentNodQuat = eulerToQuaternion(-leftUpperArmCurrentEulers.x, -leftUpperArmCurrentEulers.y, leftUpperArmCurrentEulers.z);
			leftForeArmCurrentNodQuat = eulerToQuaternion(-leftForeArmCurrentEulers.x, -leftForeArmCurrentEulers.y, leftForeArmCurrentEulers.z);
			upperSpineCurrentNodQuat = eulerToQuaternion(upperSpineCurrentEulers.x, upperSpineCurrentEulers.y, upperSpineCurrentEulers.z);
			rightThighCurrentNodQuat = eulerToQuaternion(rightThighCurrentEulers.x, -rightThighCurrentEulers.y, -rightThighCurrentEulers.z);
			rightCalfCurrentNodQuat = eulerToQuaternion(rightCalfCurrentEulers.x, -rightCalfCurrentEulers.y, -rightCalfCurrentEulers.z);
			leftThighCurrentNodQuat = eulerToQuaternion(leftThighCurrentEulers.x, -leftThighCurrentEulers.y, -leftThighCurrentEulers.z);
			leftCalfCurrentNodQuat = eulerToQuaternion(leftCalfCurrentEulers.x, -leftCalfCurrentEulers.y, -leftCalfCurrentEulers.z);
			
			rightUpperArmCurrentQuat = new Quaternion(rightUpperArmCurrentNodQuat.x, rightUpperArmCurrentNodQuat.y, rightUpperArmCurrentNodQuat.z, rightUpperArmCurrentNodQuat.w);
			rightForeArmCurrentQuat = new Quaternion(rightForeArmCurrentNodQuat.x, rightForeArmCurrentNodQuat.y, rightForeArmCurrentNodQuat.z, rightForeArmCurrentNodQuat.w);
			leftUpperArmCurrentQuat = new Quaternion(leftUpperArmCurrentNodQuat.x, leftUpperArmCurrentNodQuat.y, leftUpperArmCurrentNodQuat.z, leftUpperArmCurrentNodQuat.w);
			leftForeArmCurrentQuat = new Quaternion(leftForeArmCurrentNodQuat.x, leftForeArmCurrentNodQuat.y, leftForeArmCurrentNodQuat.z, leftForeArmCurrentNodQuat.w);
			upperSpineCurrentQuat = new Quaternion(upperSpineCurrentNodQuat.x, upperSpineCurrentNodQuat.y, upperSpineCurrentNodQuat.z, upperSpineCurrentNodQuat.w);
			rightThighCurrentQuat = new Quaternion(rightThighCurrentNodQuat.x, rightThighCurrentNodQuat.y, rightThighCurrentNodQuat.z, rightThighCurrentNodQuat.w);
			rightCalfCurrentQuat = new Quaternion(rightCalfCurrentNodQuat.x, rightCalfCurrentNodQuat.y, rightCalfCurrentNodQuat.z, rightCalfCurrentNodQuat.w);
			leftThighCurrentQuat = new Quaternion(leftThighCurrentNodQuat.x, leftThighCurrentNodQuat.y, leftThighCurrentNodQuat.z, leftThighCurrentNodQuat.w);
			leftCalfCurrentQuat = new Quaternion(leftCalfCurrentNodQuat.x, leftCalfCurrentNodQuat.y, leftCalfCurrentNodQuat.z, leftCalfCurrentNodQuat.w);
			
			//TODO: Trim rotations to Human scales
			
			/////////////////////////////////////////////
			/// Adjust Nods orientations and hierarchy:
			///////////////////////////////////////////// 
			
			upperSpineTarget 	= upperSpineInitQuat
				* upperSpineCurrentQuat
					* Quaternion.Inverse(Quaternion.Euler(0,90,0));
			
			rightUpperArmTarget = rightUpperArmInitQuat 
				* rightUpperArmCurrentQuat   
					* Quaternion.Inverse(Quaternion.Euler(90,90,0));
			
			rightForeArmTarget 	= rightForeArmInitQuat 
				* rightForeArmCurrentQuat 
					* Quaternion.Inverse(Quaternion.Euler(90,90,0));
			
			leftUpperArmTarget 	= leftUpperArmInitQuat 
				* leftUpperArmCurrentQuat 
					* Quaternion.Inverse(Quaternion.Euler(90,90,0));
			
			leftForeArmTarget 	= leftForeArmInitQuat 
				* leftForeArmCurrentQuat 
					* Quaternion.Inverse(Quaternion.Euler(90,90,0));
			
			rightThighTarget 	= rightThighInitQuat 
				* rightThighCurrentQuat
					* Quaternion.Inverse(Quaternion.Euler(0,90,180));
			
			rightCalfTarget 	= rightCalfInitQuat 
				* rightCalfCurrentQuat
					* Quaternion.Inverse(Quaternion.Euler(0,90,180));
			
			leftThighTarget 	= leftThighInitQuat 
				* leftThighCurrentQuat
					* Quaternion.Inverse(Quaternion.Euler(0,90,180));
			
			leftCalfTarget 		= leftCalfInitQuat 
				* leftCalfCurrentQuat
					* Quaternion.Inverse(Quaternion.Euler(0,90,180));
			
			/////////////////////////////////////////////
			/// Apply final rotations:
			///////////////////////////////////////////// 
			leftUpperArmTransform.rotation  = leftUpperArmTarget;
			rightUpperArmTransform.rotation = rightUpperArmTarget;
			leftForeArmTransform.rotation   = leftForeArmTarget;
			rightForeArmTransform.rotation  = rightForeArmTarget;
			rightThighTransform.rotation  	= rightThighTarget;
			leftThighTransform.rotation 	= leftThighTarget;
			rightCalfTransform.rotation 	= rightCalfTarget;
			leftCalfTransform.rotation 		= leftCalfTarget;
			upperSpineTransform.rotation 	= upperSpineTarget;
			
			/////////////////////////////////////////////
			/// Move body when legs move:
			///////////////////////////////////////////// 
			
			
			///////////////////////////////////////////// 
			/// Vertically Based on thighs
			///////////////////////////////////////////// 
			float vRThighVerticalAngle = Mathf.Abs(Quaternion.Angle(rightThighTransform.localRotation, Quaternion.identity));
			float vLThighVerticalAngle = Mathf.Abs(Quaternion.Angle(leftThighTransform.localRotation, Quaternion.identity));
			float vThighYAngle = 0.0f;
			
			//Move body only by the smallest one of the angles
			if(vRThighVerticalAngle < vLThighVerticalAngle) 
			{
				vThighYAngle = vRThighVerticalAngle;
			}
			else 
			{
				vThighYAngle = vLThighVerticalAngle;
			}
			
			float vYDisplacement = mapRange(0.0f, 180.0f, 0.0f, -0.5f, vThighYAngle);

			//Appy the transitions
			if(lowerSpineTransform != null)
			{
				
				lowerSpineCurrentPos = new Vector3(0.0f, 
				                                   vYDisplacement, 
				                                   0.0f)
					+ lowerSpineInitPos;
				
				/*lowerSpineCurrentPos = new Vector3((vLXDisplacement - vRXDisplacement)*vXFactor, 
				                                   vYDisplacement, 
				                                   0.0f)
											+ lowerSpineInitPos;*/
				
				lowerSpineTransform.localPosition = lowerSpineCurrentPos;
			}

			//nc.sendRecordedMovement();
		}
	}
	
	public void Ready()
	{
		Debug.Log("!!!!! Reset and Start Recording !!!!!");
		ResetOrientations();
		nc.startRecording();
	}
	
	public void Analyze()
	{
		/*Debug.Log("!!!!! Stop Recording And Analyze !!!!!");

		//stop recording first
		// then compare
		nc.dummyMode = true;
		nc.realTimeTracking = false;
		nc.playRecording = true;

		nc.stopRecording();
		nc.sendRecordedMovement ();
		//nc.DeInitialize();

		//check length
		//add BlankFrames for the short one
		//play both

		DummyBoneControl refModel = GameObject.Find("ReferenceModel").GetComponent<DummyBoneControl>();
		
		//bring it to the scene
		Vector3 temp = new Vector3(0,0.5f,0);
		refModel.transform.position = temp;		
		
		int frameDiff = nc.getTotalFrames() - refModel.nc.getTotalFrames();
		if(frameDiff>0)
		{
			//recorded is longer than reference
			//add blank frames to reference
			//refModel.nc.addBlankFrames(frameDiff);
		}
		else
		{
			//reference is longer than recorded
			//add blank frames to recorded
			nc.addBlankFrames(-1*frameDiff);
		}

		//Debug.Log("!!!!! Play Reference Recorded !!!!!");

		//refModel.nc.playRecorded(false);

		Debug.Log("!!!!! Play Current Recorded !!!!!");

		nc.playRecorded(false);*/
	}
	/*void OnGUI()
	{
		if (Application.platform == RuntimePlatform.IPhonePlayer) {
			// De-comment this to hide the buttons for the real iPhone app:
			return;
		}
		
		if (GUI.Button (new Rect (20, 20, 300, 100), "Record"))
		{
			Ready();
		}
		
		if (GUI.Button (new Rect (320, 20, 300, 100), "Reset Orientations "))
		{			
			ResetOrientations();        
		}
		
		if (GUI.Button (new Rect (320, 120, 300, 100), "Stop"))
		{
			nc.stopRecording();
			nc.sendRecordedMovement();
		}

		if (GUI.Button (new Rect (20, 120, 300, 100), "Replay"))
		{			
			nc.prepareOrientations();
			ResetOrientations();
			nc.prepareReferenceMovement();
			nc.playRecorded(true);			
		}
		
		if (GUI.Button (new Rect (20, 720, 300, 100), "Write Reference to Console"))
		{
			//stop recording first
			//then compare
			
			nc.DeInitialize();
			nc.writeRecordedReferenceMovement(0.03f);
		}

		if (GUI.Button (new Rect (320, 820, 300, 100), "Disconnect Sensors"))
		{
			nc.DeInitialize();
			
		}

		if (GUI.Button (new Rect (20, 820, 300, 100), "Analyse"))
		{
			Analyze();
		}
	}*/
}
