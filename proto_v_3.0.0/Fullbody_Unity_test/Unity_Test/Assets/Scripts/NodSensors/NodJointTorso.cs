using UnityEngine;
using System;
using System.Collections;
using Nod;


///// joint code for the Torso Orientation
public class NodJointTorso : NodJoint 
{


	// current Torso orientation for will be stored in this variable 
	public float [,] vaTorsoOrientation = new float[3,3];

	// Torso Extracted Angles, Angular velocities  and Angular accelerations, The names are choosed based on the human body angles document
	public float vAngleTorsoFlection = 0;
	public float vAngularVelocityTorsoFlection = 0;
	public float vAngularAccelerationTorsoFlection = 0;

	public float vAngleTorsoLateral = 0;
	public float vAngularVelocityTorsoLateral = 0;
	public float vAngularAccelerationTorsoLateral = 0;

	public float vAngleTorsoRotation = 0;
	public float vAngularVelocityTorsoRotation = 0;
	public float vAngularAccelerationTorsoRotation = 0;

	public float vAngleTorsoVertical = 0;
	public float vAngularVelocityTorsoVertical = 0;
	public float vAngularAccelerationTorsoVertical = 0;

	// This variable stores the time of curretn frame. It is used for angular velocity and acceleration extraction
	public float vTime= 0 ;



	//	
	//	 ReturnTorsoOrientation()
	//	* @ This returns the current orientation of the Torso, The curretn Torso Orientation is necessary for angle extraction of shoulders and hips
	//	* @return void float[,] vaTorsoOrientation
	//	*/// 
	public override float[,] ReturnTorsoOrientation()
	{

		return vaTorsoOrientation;
	}


		/**
	* TorsoOrientation()
	*	@This Fuction Provides The compensated Update for the Torso Orientation
	*	@param Vector3 NodTorso, Torso Euler Angles Inputs
	*	@paramVector3  InitNodTorso, this is the information of the initial frame for Torso joint
	*	@return void
	*/
	public void TorsoOrientation(Vector3 NodTorso , Vector3 InitNodTorso)
	{
		//Intermediate arrays until achive final orienation for Torso, they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

		float[,] TorsoF1 = new float[3, 3];
		float[,] TorsoF2 = new float[3, 3];
		float[,] TorsoFi = new float[3, 3];

		float[,] TorsoB1 = new float[3, 3];
		float[,] TorsoBi = new float[3, 3];



		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////


		/////////// Converting to rotation matrix ///////////////////

		TorsoB1 = RotationLocal (NodTorso.z, NodTorso.x, NodTorso.y);
		TorsoF1 = RotationGlobal (NodTorso.z, NodTorso.x, NodTorso.y);

		TorsoBi = RotationLocal (InitNodTorso.z, InitNodTorso.x, InitNodTorso.y);
		TorsoFi = RotationGlobal (InitNodTorso.z, InitNodTorso.x, InitNodTorso.y);

		/////////// Initial Frame Adjustments ///////////////////

		TorsoF2 = multi (TorsoF1, TorsoBi);
		vaTorsoOrientation = multi (TorsoFi, TorsoB1);

	}



	//	/************
	//* TorsoAngles()
	//*	@This Fuction Calculates the important angles of the Torso and updates them
	//*	@param float [,] vaTorsoOrientation, current orientation of the Torso
	//*	@return void
	//*/

	public void TorsoAngles(float [,] vaTorsoOrientation)
	{
		///// calculate the time difference since last call
		float vTimeDifference = Time.time - vTime;
		if (vTimeDifference == 0) {
			return;
		}

		vTime = Time.time;


		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////

		//////////////// calculate the Torso Flection angle ////////////////////////////////////////

		// Axes 1 to 4 are intermediate variables used to calculate angles. 
		//	with appropriate matrix calculations each angle and angular velocities are calculated in the first step
		// In the second step the sign of these angles will be determined and the angles will be updated


		/// step 1///
		Vector3 vAxis1 = new Vector3 (vaTorsoOrientation [0, 1], vaTorsoOrientation [1, 1], vaTorsoOrientation [2, 1]);
		Vector3 vAxis2 = new Vector3 (vaTorsoOrientation [0, 0], vaTorsoOrientation [1, 0], vaTorsoOrientation [2, 0]);
		Vector3 vAxis3 = new Vector3 (0, 1, 0);
		Vector3 vAxis4 = Vector3.Cross (vAxis2, vAxis3);
		vAxis4.Normalize ();
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis4)) * vAxis4;
		vAxis3.Normalize ();
		vAxis2.Set (0, 1, 0);
		float vAngleTorsoFlectionNew = Vector3.Angle (vAxis3, vAxis2);
		vAxis1.Set (vaTorsoOrientation [0, 0], 0, vaTorsoOrientation [2, 0]);
		float vAngularVelocityTorsoFlectionNew = (vAngleTorsoFlectionNew - Mathf.Abs (vAngleTorsoFlection)) / vTimeDifference;

		///step 2///
		if ((Vector3.Dot (vAxis3, vAxis1) * Vector3.Dot (vAxis3, vAxis2)) > 0) {
			vAngleTorsoFlectionNew = -vAngleTorsoFlectionNew;
			vAngularVelocityTorsoFlectionNew = -vAngularVelocityTorsoFlectionNew;
		}

		vAngularAccelerationTorsoFlection = (vAngularVelocityTorsoFlectionNew - vAngularVelocityTorsoFlection) / vTimeDifference;
		vAngularVelocityTorsoFlection = vAngularVelocityTorsoFlectionNew;
		vAngleTorsoFlection = vAngleTorsoFlectionNew;


		//Debug.Log ("Torso Angles" + vAngleTorsoFlection + ", and, " + vAngularVelocityTorsoFlection + ", and, " + vAngularAccelerationTorsoFlection);

	
		/////////////////// calculate the Torso lateral angle ////////////////////////////////////////

		// step 1///
		vAxis1.Set (vaTorsoOrientation [0, 1], vaTorsoOrientation [1, 1], vaTorsoOrientation [2, 1]);
		vAxis2.Set (vaTorsoOrientation [0, 2], vaTorsoOrientation [1, 2], vaTorsoOrientation [2, 2]);
		vAxis3.Set (0, 1, 0);
		vAxis4 = Vector3.Cross (vAxis2, vAxis3);
		vAxis4.Normalize ();
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis4)) * vAxis4;
		vAxis3.Normalize ();
		vAxis2.Set (0, 1, 0);
		float vAngleTorsoLateralNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityTorsoLateralNew = (vAngleTorsoLateralNew - Mathf.Abs (vAngleTorsoLateral)) / vTimeDifference;
		vAxis1.Set (vaTorsoOrientation [0, 2], 0, vaTorsoOrientation [2, 2]);


		///step 2///
		if ((Vector3.Dot (vAxis3, vAxis1) * Vector3.Dot (vAxis3, vAxis2)) < 0) {
			vAngleTorsoLateralNew = -vAngleTorsoLateralNew;
			vAngularVelocityTorsoLateralNew = -vAngularVelocityTorsoLateralNew;
		}

		vAngularAccelerationTorsoLateral = (vAngularVelocityTorsoLateralNew - vAngularVelocityTorsoLateral) / vTimeDifference;
		vAngularVelocityTorsoLateral = vAngularVelocityTorsoLateralNew;
		vAngleTorsoLateral = vAngleTorsoLateralNew;

		//Debug.Log ("Torso Angles" + vAngleTorsoLateral + ", and, " + vAngularVelocityTorsoLateral + ", and, " + vAngularAccelerationTorsoLateral);



		/////////////////// calculate the Torso Rotational angle ////////////////////////////////////////

		///step 1///
		vAxis1.Set (vaTorsoOrientation [0, 2], 0, vaTorsoOrientation [2, 2]);
		vAxis2.Set (0, 0, 1);
		float vAngleTorsoRotationNew = Vector3.Angle (vAxis1, vAxis2);

		float vAngularVelocityTorsoRotationNew = (vAngleTorsoRotationNew - Mathf.Abs (vAngleTorsoRotation)) / vTimeDifference;


		///step 2///
		if (vaTorsoOrientation [0, 2] < 0) {
			vAngleTorsoRotationNew = -vAngleTorsoRotationNew;
			vAngularVelocityTorsoRotationNew = -vAngularVelocityTorsoRotationNew;
		}

		vAngularAccelerationTorsoRotation = (vAngularVelocityTorsoRotationNew - vAngularVelocityTorsoRotation) / vTimeDifference;
		vAngularVelocityTorsoRotation = vAngularVelocityTorsoRotationNew;
		vAngleTorsoRotation = vAngleTorsoRotationNew;

		//Debug.Log ("Torso Angles" + vAngleTorsoRotation + ", and, " + vAngularVelocityTorsoRotation + ", and, " + vAngularAccelerationTorsoRotation);



		/////////////////// calculate the Torso Vertical angle ////////////////////////////////////////

		///step 1///
		vAxis1.Set(vaTorsoOrientation[0,1], vaTorsoOrientation[1,1], vaTorsoOrientation[2,1]);
		vAxis2.Set(0,1,0);
		float vAngleTorsoVerticalNew = Vector3.Angle (vAxis1, vAxis2);
		float vAngularVelocityTorsoVerticalNew = (vAngleTorsoVerticalNew - (vAngleTorsoVertical)) / vTimeDifference;

		///step 2///
		vAngularAccelerationTorsoVertical = (vAngularVelocityTorsoVerticalNew - vAngularVelocityTorsoVertical) / vTimeDifference;
		vAngularVelocityTorsoVertical = vAngularVelocityTorsoVerticalNew;
		vAngleTorsoVertical = vAngleTorsoVerticalNew;

		//Debug.Log ("Torso Angles" + vAngleTorsoVertical + ", and, " + vAngularVelocityTorsoVertical + ", and, " + vAngularAccelerationTorsoVertical);

	}




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


/// <summary>
// To Show appropriate Angles for each joint in GUI
/// <summary>
	public override void OnGUIAngles()
	{

		if (NodContainer.vKey == 1)
		{
			GUI.contentColor = Color.black;

			GUI.Label (new Rect (880, 450, 400 , 25), "Thoracolumbar Flexion/Extension: " + vAngleTorsoFlection , "color");

			GUI.Label (new Rect (880, 480, 400 , 25), "Thoracolumbar Lateral Flexion: " + vAngleTorsoLateral , "color");

			GUI.Label (new Rect (880, 510, 400 , 25), "Thoracolumbar Rotation: " + vAngleTorsoRotation, "color");

			GUI.Label (new Rect (1180, 450, 400, 25), "Thoracolumbar Vertical: " + vAngleTorsoVertical , "color");
		}

	}







	public override void UpdateJoint () 
	{
		for (int ndx = 0; ndx < mNodSensors.Length; ndx++)
		{
			mNodSensors [ndx].UpdateSensor ();
		}

		Vector3 vNodRawEuler1 = mNodSensors[0].curRotationRawEuler;
		vNodRawEuler1 = new Vector3(vNodRawEuler1.x , vNodRawEuler1.y , vNodRawEuler1.z );
		Vector3 NodIniEuler1 = mNodSensors[0].initRotationEuler;
		NodIniEuler1 = new Vector3(NodIniEuler1.x , NodIniEuler1.y , NodIniEuler1.z );

		// call TorsoOrientation function to calculate current orientation of the Torso joint
		TorsoOrientation (vNodRawEuler1, NodIniEuler1);

		// call TorsoAngles function to calculate current angles of Torso
		TorsoAngles(vaTorsoOrientation);

		// convert the orientation matrices to quatrenion to be used in unity model
		NodQuaternionOrientation vNodRawQuat = MatToQuat(vaTorsoOrientation);

		Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
		Quaternion vJointQuat = inverseInitRotation * vNodQuat * Quaternion.Inverse(Quaternion.Euler(quaternionFactor));

		if(jointTransform != null)
		{
			jointTransform.rotation = vJointQuat;
			jointTransform2.rotation = vJointQuat;
		}


	}

}
