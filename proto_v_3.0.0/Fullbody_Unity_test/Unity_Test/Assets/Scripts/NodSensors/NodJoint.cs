using UnityEngine;
using System;
using System.Collections;
using Nod;

public class NodJoint : MonoBehaviour  
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

		 

				Debug.Log("Reseting joint nod");
				Vector3 vNodRawEuler = mNodSensors[ndx].curRotationRawEuler;
				vNodRawEuler = new Vector3(vNodRawEuler.x , vNodRawEuler.y , vNodRawEuler.z );
				vNodIniEuler.Set(vNodRawEuler.x * eulerFactor.x, vNodRawEuler.y * eulerFactor.y, vNodRawEuler.z * eulerFactor.z);
				NodQuaternionOrientation vNodRawQuat = eulerToQuaternion(vNodRawEuler.x, vNodRawEuler.y, vNodRawEuler.z);
				Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
				inverseInitRotation = Quaternion.Inverse(vNodQuat * Quaternion.Inverse(Quaternion.Euler(quaternionFactor)));

		}

	}




	/////////////////////////////Strech Sensor/// Filtering- Angle extraction
	//*********************************************************************************
	public float SSFiltering (float vStrechSenseData, float vStrechSenseDataOld )
	{
		
		vStrechSenseData=vStrechSenseDataOld*0.75f+vStrechSenseData*0.25f;
		return vStrechSenseData;
	}
	float ssMax = 0;
	float ssmin = 100000;

	public float SSAngleMap (float vStrechSenseData,float TetaMax, float Tetamin)
	{
		
		float ssAngleMap;

		if (vStrechSenseData<ssmin)
		{
			ssmin = vStrechSenseData;
		}
		if (vStrechSenseData>ssMax)
		{
			ssMax = vStrechSenseData;
		}
		ssAngleMap= (vStrechSenseData-ssmin)*(TetaMax-Tetamin)/(ssMax-ssmin)+Tetamin;
		//print ("ssAngleMap  "+ ssAngleMap+" ssmin " +ssmin +" ssMax "+ssMax);
		return ssAngleMap;
	}
	//*********************************************************************************





	//////////////////////////// These are the reference functions for matrix calculation and transformation ///////////////////////////////

	/**
	//	* RotationGlobal()
	//	* @ This Performs transformation From Global Coordinate System To local Nod coordinates
	//	* @params yaw pitch and roll of each nod
	//	* @return 3*3 orientation matrix in nod local coordinate system
	//	*/// 
	public float[,] RotationGlobal(float yaw, float pitch, float roll)
	{
		float [,] a = new float[3,3];
		a[0,0] = Mathf.Cos(pitch) * Mathf.Cos(yaw);
		a[1,0] = (Mathf.Sin(roll) * Mathf.Cos(yaw) * Mathf.Sin(pitch) - Mathf.Sin(yaw) * Mathf.Cos(roll));
		a[2,0] = (Mathf.Sin(roll) *Mathf.Sin(yaw) + Mathf.Cos(yaw) *Mathf.Sin(pitch)*Mathf.Cos(roll));
		a[0,1] = Mathf.Cos(pitch)*Mathf.Sin(yaw);
		a[1,1] = Mathf.Sin(roll)*Mathf.Sin(yaw)*Mathf.Sin(pitch) + Mathf.Cos(yaw) * Mathf.Cos(roll);
		a[2,1] = (Mathf.Cos(roll)*Mathf.Sin(yaw)*Mathf.Sin(pitch) - Mathf.Cos(yaw) * Mathf.Sin(roll));
		a[0,2] = -Mathf.Sin(pitch);
		a[1,2] = Mathf.Cos(pitch)*Mathf.Sin(roll);
		a[2,2] = (Mathf.Cos(pitch)*Mathf.Cos(roll));
		return a;
	}



	/**
	//	* RotationLocal()
	//	* @ This Performs transformation From Nods Local Coordinate System To global coordinates
	//	* @params yaw pitch and roll of each nod
	//	* @return 3*3 orientation matrix in global coordinate system
	//	*/// 
	public float[,] RotationLocal( float yaw, float pitch, float roll)
	{
		float [,] a = new float[3,3];
		a[0,0] = Mathf.Cos(pitch) * Mathf.Cos(yaw);
		a[1,0] = Mathf.Cos(pitch) * Mathf.Sin(yaw);
		a[2,0] = -Mathf.Sin(pitch);
		a[0,1] = -Mathf.Cos(roll) *Mathf.Sin(yaw) + Mathf.Cos(yaw) *Mathf.Sin(pitch)*Mathf.Sin(roll);
		a[1,1] = Mathf.Sin(roll)*Mathf.Sin(yaw)*Mathf.Sin(pitch) + Mathf.Cos(yaw) * Mathf.Cos(roll);
		a[2,1] = Mathf.Cos(pitch)*Mathf.Sin(roll);
		a[0,2] = (Mathf.Sin(roll) *Mathf.Sin(yaw) + Mathf.Cos(yaw) *Mathf.Sin(pitch)*Mathf.Cos(roll));
		a[1,2] = (Mathf.Cos(roll)*Mathf.Sin(yaw)*Mathf.Sin(pitch) - Mathf.Cos(yaw) * Mathf.Sin(roll));
		a[2,2] = (Mathf.Cos(pitch)*Mathf.Cos(roll));
		return a;
	}




	//	/**
	//* MatToQuat
	//* @It converts a 3*3 orientation Matrix to a Quatrenion
	//* @param float m[][3] is the original 3*3 matrix
	//* @return NodQuaternionOrientation, the orientation in quatrenion
	//* reference: @http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche52.html
	//*
	public static NodQuaternionOrientation MatToQuat(float [,] m)
	{
		NodQuaternionOrientation q;
		q.w = (m[0,0] + m[1,1] + m[2,2] + 1.0f) / 4.0f;
		q.x = (m[0,0] - m[1,1] - m[2,2] + 1.0f) / 4.0f;
		q.y = (-m[0,0] + m[1,1] - m[2,2] + 1.0f) / 4.0f;
		q.z = (-m[0,0] - m[1,1] + m[2,2] + 1.0f) / 4.0f;
		if (q.w < 0.0f) q.w = 0.0f;
		if (q.x < 0.0f) q.x = 0.0f;
		if (q.y < 0.0f) q.y = 0.0f;
		if (q.z < 0.0f) q.z = 0.0f;
		q.w = (float)Math.Sqrt(q.w);
		q.x = (float)Math.Sqrt(q.x);
		q.y = (float)Math.Sqrt(q.y);
		q.z = (float)Math.Sqrt(q.z);
		if (q.w >= q.x && q.w >= q.y && q.w >= q.z) {
			q.w *= +1.0f;
			q.x *= Mathf.Sign(m[2,1] - m[1,2]);
			q.y *= Mathf.Sign(m[0,2] - m[2,0]);
			q.z *= Mathf.Sign(m[1,0] - m[0,1]);
		}
		else if (q.x >= q.w && q.x >= q.y && q.x >= q.z) {
			q.w *= Mathf.Sign(m[2,1] - m[1,2]);
			q.x *= +1.0f;
			q.y *= Mathf.Sign(m[1,0] + m[0,1]);
			q.z *= Mathf.Sign(m[0,2] + m[2,0]);
		}
		else if (q.y >= q.w && q.y >= q.x && q.y >= q.z) {
			q.w *= Mathf.Sign(m[0,2] - m[2,0]);
			q.x *= Mathf.Sign(m[1,0] + m[0,1]);
			q.y *= +1.0f;
			q.z *= Mathf.Sign(m[2,1] + m[1,2]);
		}
		else if (q.z >= q.w && q.z >= q.x && q.z >= q.y) {
			q.w *= Mathf.Sign(m[1,0] - m[0,1]);
			q.x *= Mathf.Sign(m[2,0] + m[0,2]);
			q.y *= Mathf.Sign(m[2,1] + m[1,2]);
			q.z *= +1.0f;
		}

		float r = (float)Math.Sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
		q.w /= r;
		q.x /= r;
		q.y /= r;
		q.z /= r;
		return q;
	}




	//
	//	/**
	//* RotationVector()
	//* @It producess a rotation matrix around an arbitary vector with desired angles
	//* @param vec u, arbitary unit vector
	//* @param flaot t, desired angle of rotation
	//* @return float a[][3], The output rotation matrix
	//*/
	public float[,] RVector(Vector3 u, float t)
	{
		float [,] a = new float[3,3];
		a[0,0] = Mathf.Cos(t) + u.x*u.x* (1 - Mathf.Cos(t));
		a[1,0] = u.x*u.y* (1 - Mathf.Cos(t)) + u.z * Mathf.Sin(t);
		a[2,0] = u.x*u.z* (1 - Mathf.Cos(t)) - u.y * Mathf.Sin(t);
		a[0,1] = u.x*u.y* (1 - Mathf.Cos(t)) - u.z * Mathf.Sin(t);
		a[1,1] = Mathf.Cos(t) + u.y*u.y* (1 - Mathf.Cos(t));;
		a[2,1] = u.z*u.y* (1 - Mathf.Cos(t)) + u.x * Mathf.Sin(t);
		a[0,2] = u.x*u.z* (1 - Mathf.Cos(t)) + u.y * Mathf.Sin(t);
		a[1,2] = u.z*u.y* (1 - Mathf.Cos(t)) - u.x * Mathf.Sin(t);
		a[2,2] = Mathf.Cos(t) + u.z*u.z* (1 - Mathf.Cos(t));
		return a;
	}



	//		/**
	//	* multi()
	//	*	@This Fuction do multiplication between two 3*3 matrices
	//	*	@param matrix a and b
	//	*	@return c = a * b,
	//	*/
	public float[,] multi(float[,] a, float[,] b)
	{
		float [,] c = new float[3,3];
		int i, j, k;
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				c[i,j] = a[i,0] * b[0,j] + a[i,1] * b[1,j] + a[i,2] * b[2,j];

			}
		}
		return c;
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
