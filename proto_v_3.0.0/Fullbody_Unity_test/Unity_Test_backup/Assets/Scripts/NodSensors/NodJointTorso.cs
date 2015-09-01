using UnityEngine;
using System;
using System.Collections;
using Nod;

public class NodJointTorso : NodJoint 
{
	///// joint code for the Torso

	public float [,] vaTorsoOrientation = new float[3,3];

	// Torso Extracted Angles, Angular velocities  and Angular accelerations
	public float vAngleTorsoFlection = 0;
	public float vAngularVelocityTorsoFlection = 0;
	public float vAngularAccelerationTorsoFlection = 0;

	public float vAngleTorsoLateral = 0;
	public float vAngularVelocityTorsoLateral = 0;
	public float vAngularAccelerationTorsoLateral = 0;

	public float vAngleTorsoRotation = 0;
	public float vAngularVelocityTorsoRotation = 0;
	public float vAngularAccelerationTorsoRotation = 0;

	public float vTime= 0 ;



//	/**
	//	* ReturnTorsoOrientation()
//	* @ This returns the current orientation of the Torso
//	* @param float a[][3], Outputted Rotation Matrix
//	* @param float yaw, float pitch, float roll, Euler Angles
//	* @return void
//	*/// 
	public override float[,] ReturnTorsoOrientation()
	{

		return vaTorsoOrientation;
	}



//	/**
//	* RotationGlobal()
//	* @ This Performs Rotation From Global Coordinate System To local
//	* @param float a[][3], Outputted Rotation Matrix
//	* @param float yaw, float pitch, float roll, Euler Angles
//	* @return void
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
	* RotationLocal()
	* @ This Performs Rotation From Local Coordinate System To Global
	* @param float a[][3], Outputted Rotation Matrix
	* @param float yaw, float pitch, float roll, Euler Angles
	* @return void
	*/// 

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




	/**
* SIGN()
* @It provides sign of an input
* @param float x is the input
* @return float sign of the x
*/
	public static float SIGN(float x) { return (x >= 0.0f) ? +1.0f : -1.0f; }




	/**
* MatToQuat
* @It converts a Matrix to a Quatrenion
* @param q is the transformed quatrenion
* @param float m[][3] is the original 3*3 matrix
* @return void
* @http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche52.html
*/
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
			q.x *= SIGN(m[2,1] - m[1,2]);
			q.y *= SIGN(m[0,2] - m[2,0]);
			q.z *= SIGN(m[1,0] - m[0,1]);
		}
		else if (q.x >= q.w && q.x >= q.y && q.x >= q.z) {
			q.w *= SIGN(m[2,1] - m[1,2]);
			q.x *= +1.0f;
			q.y *= SIGN(m[1,0] + m[0,1]);
			q.z *= SIGN(m[0,2] + m[2,0]);
		}
		else if (q.y >= q.w && q.y >= q.x && q.y >= q.z) {
			q.w *= SIGN(m[0,2] - m[2,0]);
			q.x *= SIGN(m[1,0] + m[0,1]);
			q.y *= +1.0f;
			q.z *= SIGN(m[2,1] + m[1,2]);
		}
		else if (q.z >= q.w && q.z >= q.x && q.z >= q.y) {
			q.w *= SIGN(m[1,0] - m[0,1]);
			q.x *= SIGN(m[2,0] + m[0,2]);
			q.y *= SIGN(m[2,1] + m[1,2]);
			q.z *= +1.0f;
		}

		float r = (float)Math.Sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
		q.w /= r;
		q.x /= r;
		q.y /= r;
		q.z /= r;
		return q;
	}








	/**
* TorsoOrientation()
*	@This Fuction Provides The Final compensated Update for the Torso Orientation
*	@param Vector3 NodTorso, Torso Euler Angles Inputs
*	@paramVector3  InitNodTorso, this is the information of the initial frame for Torso joint
*	@return void
*/
	public void TorsoOrientation(Vector3 NodTorso , Vector3 InitNodTorso)
	{
		//Intermediate arrays until achive final orienation for hip and knee, they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

		float[,] TorsoF1 = new float[3, 3];
		float[,] TorsoF2 = new float[3, 3];
		float[,] TorsoF3 = new float[3, 3];
		float[,] TorsoF4 = new float[3, 3];
		float[,] TorsoFi = new float[3, 3];

		float[,] TorsoB1 = new float[3, 3];
		float[,] TorsoB2 = new float[3, 3];
		float[,] TorsoB3 = new float[3, 3];
		float[,] TorsoB4 = new float[3, 3];
		float[,] TorsoBi = new float[3, 3];



		float[,] CompensationRotationKnee = new float[3, 3];
		float[,] CompensationRotationHip = new float[3, 3];
		float[,] CurrentKneeOrientation = new float[3, 3];

		float OrientationError = 0;
		float CompensationAngle = 0;


		/////////// Converting to rotation matrix ///////////////////

		TorsoB1 = RotationLocal (NodTorso.z, NodTorso.x, NodTorso.y);
		TorsoF1 = RotationGlobal (NodTorso.z, NodTorso.x, NodTorso.y);

		TorsoBi = RotationLocal (InitNodTorso.z, InitNodTorso.x, InitNodTorso.y);
		TorsoFi = RotationGlobal (InitNodTorso.z, InitNodTorso.x, InitNodTorso.y);

		/////////// Initial Frame Adjustments ///////////////////

		TorsoF2 = multi (TorsoF1, TorsoBi);
		vaTorsoOrientation = multi (TorsoFi, TorsoB1);

	}










	/************
* TorsoAngles()
*	@This Fuction Calculates the important angles of the Torso
*	@param float [,] vaTorsoOrientation, Final orientation of the Torso
*	@return void
*/

	public void TorsoAngles(float [,] vaTorsoOrientation)

	{
		///// calculate the time difference since last call
		float vTimeDifference = Time.time - vTime;
		if (vTimeDifference == 0)
		{
			return;
		}

		vTime = Time.time;

		//////////////// calculate the Torso Flection angle ////////////////////////////////////////

		// Axis 1 and 2 are two axes to calculate each angle 

		Vector3 vAxis1 = new Vector3(vaTorsoOrientation[0,1],vaTorsoOrientation[1,1], vaTorsoOrientation[2,1]);
		Vector3 vAxis2 = new Vector3(vaTorsoOrientation[0,0],vaTorsoOrientation[1,0] , vaTorsoOrientation [2,0]);
		Vector3 vAxis3 = new Vector3(0,1,0);
		Vector3 vAxis4 =Vector3.Cross (vAxis2 , vAxis3);
		vAxis4.Normalize ();
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis4)) * vAxis4;
		vAxis3.Normalize();
		//Debug.Log (vaTorsoOrientation[0,2]  + ", and, " + vaTorsoOrientation [2,2]);

		vAxis2.Set (0, 1, 0);

		float vAngleTorsoFlectionNew = Vector3.Angle (vAxis3, vAxis2);
		vAxis1.Set (vaTorsoOrientation [0, 0], 0, vaTorsoOrientation [2, 0]);
		float vAngularVelocityTorsoFlectionNew = (vAngleTorsoFlectionNew - Mathf.Abs(vAngleTorsoFlection)) / vTimeDifference;

		//Debug.Log (vaTorsoOrientation [0, 0] + " and " + vaTorsoOrientation [2, 0] +  " and " +  vAxis3.x + " and " +  vAxis3.y + " and " +  vAxis3.z );
		if ( ( Vector3.Dot(vAxis3, vAxis1) * Vector3.Dot(vAxis3, vAxis2) ) > 0)
		{
			vAngleTorsoFlectionNew = -vAngleTorsoFlectionNew;
			vAngularVelocityTorsoFlectionNew = -vAngularVelocityTorsoFlectionNew;
		}

		vAngularAccelerationTorsoFlection = (vAngularVelocityTorsoFlectionNew - vAngularVelocityTorsoFlection) / vTimeDifference;
		vAngularVelocityTorsoFlection = vAngularVelocityTorsoFlectionNew;
		vAngleTorsoFlection = vAngleTorsoFlectionNew;


		//Debug.Log ("Torso Angles" + vAngleTorsoFlection + ", and, " + vAngularVelocityTorsoFlection + ", and, " + vAngularAccelerationTorsoFlection);

	
		/////////////////// calculate the Torso lateral angle ////////////////////////////////////////

		vAxis1.Set(vaTorsoOrientation[0,1],vaTorsoOrientation[1,1], vaTorsoOrientation[2,1]);
		vAxis2.Set(vaTorsoOrientation[0,2],vaTorsoOrientation[1,2] , vaTorsoOrientation [2,2]);
		vAxis3.Set(0,1,0);
		vAxis4 =Vector3.Cross (vAxis2 , vAxis3);
		vAxis4.Normalize ();
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis4)) * vAxis4;
		vAxis3.Normalize();
		vAxis2.Set (0, 1, 0);
		float vAngleTorsoLateralNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityTorsoLateralNew = (vAngleTorsoLateralNew - Mathf.Abs(vAngleTorsoLateral)) / vTimeDifference;
		vAxis1.Set (vaTorsoOrientation [0, 2], 0, vaTorsoOrientation [2, 2]);
		if ( ( Vector3.Dot(vAxis3, vAxis1) * Vector3.Dot(vAxis3, vAxis2) ) < 0)
		{
			vAngleTorsoLateralNew = -vAngleTorsoLateralNew;
			vAngularVelocityTorsoLateralNew = -vAngularVelocityTorsoLateralNew;
		}

		vAngularAccelerationTorsoLateral = (vAngularVelocityTorsoLateralNew - vAngularVelocityTorsoLateral) / vTimeDifference;
		vAngularVelocityTorsoLateral = vAngularVelocityTorsoLateralNew;
		vAngleTorsoLateral = vAngleTorsoLateralNew;

		//Debug.Log ("Torso Angles" + vAngleTorsoLateral + ", and, " + vAngularVelocityTorsoLateral + ", and, " + vAngularAccelerationTorsoLateral);



		/////////////////// calculate the Torso Rotational angle ////////////////////////////////////////

		vAxis1.Set(vaTorsoOrientation[0,2], 0, vaTorsoOrientation[2,2]);
		vAxis2.Set(0,0,1);
		float vAngleTorsoRotationNew = Vector3.Angle (vAxis1, vAxis2);

		float vAngularVelocityTorsoRotationNew = (vAngleTorsoRotationNew - Mathf.Abs(vAngleTorsoRotation)) / vTimeDifference;

		if (vaTorsoOrientation[0,2] < 0 )
		{
			vAngleTorsoRotationNew = -vAngleTorsoRotationNew;
			vAngularVelocityTorsoRotationNew = -vAngularVelocityTorsoRotationNew;
		}

		vAngularAccelerationTorsoRotation = (vAngularVelocityTorsoRotationNew - vAngularVelocityTorsoRotation) / vTimeDifference;
		vAngularVelocityTorsoRotation = vAngularVelocityTorsoRotationNew;
		vAngleTorsoRotation = vAngleTorsoRotationNew;

		//Debug.Log ("Torso Angles" + vAngleTorsoRotation + ", and, " + vAngularVelocityTorsoRotation + ", and, " + vAngularAccelerationTorsoRotation);

	}




//	/**
//* RotationVector()
//* @It provides a rotation matrix around an arbitary vector with desired angles
//* @param float a[][3], The output rotation matrix
//* @param vec u, arbitary unit vector
//* @param flaot t, desired angle of rotation
//* @return void
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










	/**
* multi()
*	@This Fuction do multiplication between two 3*3 matrices
*	@param matrix a and b
*	@return c = a * b,
*/
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
	// To Show appropriate Angles in each joint
	/// <summary>
	public override void OnGUIAngles()
	{

		if (NodContainer.vKey == 1)
		{
			GUI.contentColor = Color.black;

			GUI.Label (new Rect (930, 450, 400 , 25), "Torso Flexion/Extension: " + vAngleTorsoFlection , "color");

			GUI.Label (new Rect (930, 480, 400 , 25), "Torso Lateral Flexion: " + vAngleTorsoLateral , "color");

			GUI.Label (new Rect (930, 510, 400 , 25), "Torso Rotation: " + vAngleTorsoRotation, "color");
		}

	}






	//TODO: for now only one nod sensor per joint ! 

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
		TorsoOrientation (vNodRawEuler1, NodIniEuler1);
		TorsoAngles(vaTorsoOrientation);
		NodQuaternionOrientation vNodRawQuat = MatToQuat(vaTorsoOrientation)	;
		Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
		Quaternion vJointQuat = inverseInitRotation * vNodQuat * Quaternion.Inverse(Quaternion.Euler(quaternionFactor));

		if(jointTransform != null)
		{
			jointTransform.rotation = vJointQuat;
			jointTransform2.rotation = vJointQuat;
		}


	}

}
