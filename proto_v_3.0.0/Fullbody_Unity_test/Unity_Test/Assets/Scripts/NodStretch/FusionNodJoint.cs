using UnityEngine;
using System;
using System.Collections;
using Nod;

public class FusionNodJoint : MonoBehaviour  
{

	public NodSensor [] mNodSensors;

	//represent the different factors of the rotations
	public Transform jointTransform = null;
	public Transform jointTransform2 = null;
	public Vector3 eulerFactor = Vector3.zero;
	public Vector3 quaternionFactor = Vector3.zero;
	public Vector3 vNodIniEuler = Vector3.zero;
	public float [,] NodCurrentOrientation = new float[3,3];
	public float vLegHeight;

	//Initial rotation
	public Quaternion inverseInitRotation = Quaternion.identity;

	//When True the stretch joint updates independantly of the rest
	//otherwise the joint only updates when the full body is updated
	public Boolean independantUpdate = false;


	/// <summary>
	/// Returns the torso orientation.
	/// </summary>
	/// <returns>The torso orientation.</returns>
	public virtual float[,] ReturnTorsoOrientation()
	{

		return NodCurrentOrientation;
	}


	/// <summary>
	// To Show appropriate Angles in each joint
	/// <summary>
	public virtual void OnGUIAngles()
	{
		// will be override in each child
	}



	/// <summary>
	/// Eulers to quaternion.
	/// </summary>
	/// <returns>The to quaternion.</returns>
	/// <param name="pitch">Pitch.</param>
	/// <param name="roll">Roll.</param>
	/// <param name="yaw">Yaw.</param>

	public static NodQuaternionOrientation eulerToQuaternion(float pitch, float roll, float yaw)
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
	}



	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoint () 
	{
		for (int ndx = 0; ndx < mNodSensors.Length; ndx++) 
		{
			mNodSensors[ndx].StartReading();
		}
	}
	
	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public virtual void UpdateJoint () 
	{
		for (int ndx = 0; ndx < mNodSensors.Length; ndx++) 
		{
			mNodSensors[ndx].UpdateSensor();

			//TODO: for now only one nod sensor per joint ! 
			if(ndx == 0)
			{
				Vector3 vNodRawEuler = mNodSensors[ndx].curRotationRawEuler;
				//Debug.Log (vNodRawEuler.z * eulerFactor.y );
				vNodRawEuler = new Vector3(vNodRawEuler.x * eulerFactor.x, vNodRawEuler.y * eulerFactor.y, vNodRawEuler.z * eulerFactor.z);
				NodQuaternionOrientation vNodRawQuat = eulerToQuaternion(vNodRawEuler.x, vNodRawEuler.y, vNodRawEuler.z);
				Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
				Quaternion vJointQuat = inverseInitRotation * vNodQuat * Quaternion.Inverse(Quaternion.Euler(quaternionFactor));

				if(jointTransform != null)
				{
					jointTransform.rotation = vJointQuat;
				}

			}
		}
	}
	
	/// <summary>
	/// Reset the stretch joint sensors.
	/// </summary>
	public virtual void ResetJoint ()
	{
		for (int ndx = 0; ndx < mNodSensors.Length; ndx++) 
		{
			mNodSensors[ndx].Reset();

		 

				//Debug.Log("Reseting joint nod");
				Vector3 vNodRawEuler = mNodSensors[ndx].curRotationRawEuler;
				vNodRawEuler = new Vector3(vNodRawEuler.x , vNodRawEuler.y , vNodRawEuler.z );
				vNodIniEuler.Set(vNodRawEuler.x * eulerFactor.x, vNodRawEuler.y * eulerFactor.y, vNodRawEuler.z * eulerFactor.z);
				NodQuaternionOrientation vNodRawQuat = eulerToQuaternion(vNodRawEuler.x, vNodRawEuler.y, vNodRawEuler.z);
				Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
				inverseInitRotation = Quaternion.Inverse(vNodQuat * Quaternion.Inverse(Quaternion.Euler(quaternionFactor)));

		}

	}

	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
	//////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Awake this instance.
	/// </summary>
	void Awake ()
	{
		mNodSensors = GetComponentsInChildren<NodSensor>();
		
		//for (int ndx = 0; ndx < mNodSensors.Length; ndx++) 
		//{
		//	mNodSensors[ndx].nodID = ndx;
		//}

	}
	
	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start() 
	{
		if (independantUpdate) 
		{
			ResetJoint();
			StartJoint();
		}
	}
	
	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update() 
	{
		if (independantUpdate) 
		{
			UpdateJoint();
		}
	}
	
	/// <summary>
	/// GUI Update.
	/// </summary>
	void OnGUI()
	{
		//Show the name of the ring over the joint the NodBone is attached to
		Camera cam = Camera.current;
		if (null == cam)
			return;


		OnGUIAngles ();
		foreach (NodSensor sensor in mNodSensors) 
		{
			string msg = sensor.nodName;
			Vector3 ringWorldPos = sensor.transform.position;
			Vector3 pos = cam.WorldToScreenPoint(ringWorldPos);
			//GUI.Label(new Rect(pos.x, Screen.height - pos.y, 150, 150), msg);
		}
	}
}
