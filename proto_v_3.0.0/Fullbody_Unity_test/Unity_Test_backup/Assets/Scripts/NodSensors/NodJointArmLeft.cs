using UnityEngine;
using System;
using System.Collections;
using Nod;

public class NodJointArmLeft : NodJoint 
{
	// this file calculates the orientation for the right forearm
	public float [,] UpArOrientation = new float[3,3];
	public float [,] LoArOrientation = new float[3,3];


	public float vTime = 0;


	public float vAngleLeftElbowFlexion = 0;
	public float vAngularVelocityLeftElbowFlexion = 0;
	public float vAngularAccelerationLeftElbowFlexion = 0;

	public float vAngleLeftElbowPronation = 0;
	public float vAngularVelocityLeftElbowPronation = 0;
	public float vAngularAccelerationLeftElbowPronation = 0;


	public float vAngleLeftShoulderFlexion = 0;
	public float vAngularVelocityLeftShoulderFlexion = 0;
	public float vAngularAccelerationLeftShoulderFlexion = 0;

	public float vAngleLeftShoulderAbduction = 0;
	public float vAngularVelocityLeftShoulderAbduction = 0;
	public float vAngularAccelerationLeftShoulderAbduction = 0;


	public float vAngleLeftShoulderRotation = 0;
	public float vAngularVelocityLeftShoulderRotation = 0;
	public float vAngularAccelerationLeftShoulderRotation = 0;


	/**
	* RotationGlobal()
	* @ This Performs Rotation From Global Coordinate System To local
	* @param float a[][3], Outputted Rotation Matrix
	* @param float yaw, float pitch, float roll, Euler Angles
	* @return void
	*/// 


	public float[,] RotationGlobal(float yaw, float pitch, float roll)
	{
		float [,] a = new float[3,3];
		a[0,0] = Mathf.Cos(pitch)*Mathf.Cos(yaw);
		a[1,0] = (Mathf.Sin(roll)*Mathf.Cos(yaw)*Mathf.Sin(pitch) - Mathf.Sin(yaw) * Mathf.Cos(roll));
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
* HipOrientation()
*	@This Fuction Provides The Final compensated Update for the Hip Orientation
*	@param CurrentHipOrientation[][3]: The final hip orientation
*	@param float IntitialRotationLocalHip[][3], this is the information of the initial frame for Hip joint
*	@param float IntitialRotationGloballHip[][3],  this is the information of the initial frame for Hip joint
*	@param float IntitialRotationLocalKnee[][3], this is the information of the initial frame for Knee joint
*	@param float IntitialRotationGlobalKnee[][3], this is the information of the initial frame for Knee joint
*	@param float NodHipYaw , .......... Hip and Knee Nods Inputs
*  @param float StretchSensorKnee    Stretch Sensor data for Knee
*	@return void
*/
	public void ArmOrientation(Vector3 NodUpAr , Vector3 InitNodUpAr , Vector3 NodLoAr , Vector3 InitNodLoAr)

	{
		//Intermediate arrays until achive final orienation for hip and knee, they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

		float [,] UpArF1 = new float[3,3];
		float [,] UpArF2 = new float[3,3];
		float [,] UpArF3 = new float[3,3];
		float [,] UpArF4 = new float[3,3];
		float [,] UpArF5 = new float[3,3];
		float [,] UpArF6 = new float[3,3];
		float [,] UpArF7 = new float[3,3];
		float [,] UpArFi = new float[3,3];


		float [,] UpArB1 = new float[3,3];
		float [,] UpArB2 = new float[3,3];
		float [,] UpArB3 = new float[3,3];
		float [,] UpArB4 = new float[3,3];
		float [,] UpArB5 = new float[3,3];
		float [,] UpArB6 = new float[3,3];
		float [,] UpArB7 = new float[3,3];
		float [,] UpArBi = new float[3,3];

		float [,] LoArF1 = new float[3,3];
		float [,] LoArF2 = new float[3,3];
		float [,] LoArF3 = new float[3,3];
		float [,] LoArF4 = new float[3,3];
		float [,] LoArF5 = new float[3,3];
		float [,] LoArF6 = new float[3,3];
		float [,] LoArF7 = new float[3,3];
		float [,] LoArF8 = new float[3,3];
		float [,] LoArFi = new float[3,3];


		float [,] LoArB1 = new float[3,3];
		float [,] LoArB2 = new float[3,3];
		float [,] LoArB3 = new float[3,3];
		float [,] LoArB4 = new float[3,3];
		float [,] LoArB5 = new float[3,3];
		float [,] LoArB6 = new float[3,3];
		float [,] LoArB7 = new float[3,3];
		float [,] LoArBi = new float[3,3];



		float [,] CompensationRotationLoAr = new float[3,3];
		float [,] CompensationRotationUpAr = new float[3,3];
		float [,] CurrentLoArOrientation = new float[3,3];

		float OrientationError = 0;
		float CompensationAngle = 0;


		Vector3 pitchUpAr = new Vector3(0,0,0);
		Vector3  pitchLoAr = new Vector3(0,0,0);

	









		/////////// Converting to rotation matrix ///////////////////

		UpArB1 = RotationLocal (NodUpAr.z, NodUpAr.x, NodUpAr.y);
		UpArF1 = RotationGlobal (NodUpAr.z,NodUpAr.x, NodUpAr.y);

		LoArB1 = RotationLocal (NodLoAr.z, NodLoAr.x, NodLoAr.y);
		LoArF1 = RotationGlobal (NodLoAr.z,NodLoAr.x, NodLoAr.y);

		UpArBi = RotationLocal (InitNodUpAr.z, InitNodUpAr.x, InitNodUpAr.y);
		UpArFi = RotationGlobal (InitNodUpAr.z,InitNodUpAr.x, InitNodUpAr.y);

		LoArBi = RotationLocal (InitNodLoAr.z, InitNodLoAr.x, InitNodLoAr.y);
		LoArFi = RotationGlobal (InitNodLoAr.z, InitNodLoAr.x, InitNodLoAr.y);



		/////////// Initial Frame Adjustments ///////////////////
		UpArF2 = multi(UpArF1,UpArBi);
		UpArB2 = multi(UpArFi, UpArB1);
		LoArF2 = multi(LoArF1, LoArBi);
		LoArB2 = multi(LoArFi, LoArB1);




		///////////// Yaw Compensation Step ///////////////////


		// finding compensated yaw in one plane with upperarm
		float temp1, temp2, temp3, temp4;
		temp1 = LoArB2[0,1] - (UpArB2[0,2] * LoArB2[0,1] + UpArB2[1,2] * LoArB2[1,1] + UpArB2[2,2] * LoArB2[2,1]) * UpArB2[0,2];
		temp2 = LoArB2[1,1] - (UpArB2[0,2] * LoArB2[0,1] + UpArB2[1,2] * LoArB2[1,1] + UpArB2[2,2] * LoArB2[2,1]) * UpArB2[1,2];
		temp3 = LoArB2[2,1] - (UpArB2[0,2] * LoArB2[0,1] + UpArB2[1,2] * LoArB2[1,1] + UpArB2[2,2] * LoArB2[2,1]) * UpArB2[2,2];
		temp4 = (float)Math.Sqrt((float)(temp1 * temp1 + temp2 * temp2 + temp3*temp3));
		Vector3   yawLoAr = new Vector3(temp1/temp4,temp2/temp4,temp3/temp4);

		// Finding yaw compensation Angle
		OrientationError = yawLoAr.x * LoArB2[0,1] + yawLoAr.y * LoArB2[1,1] + yawLoAr.z* LoArB2[2,1];
		CompensationAngle = (float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError);

		// Finding yaw compensation axis
		Vector3  yawLoAr2 = new Vector3(LoArB2[0,1],LoArB2[1,1],LoArB2[2,1]);

		Vector3 NcrossUpArLoAr = new Vector3(0,0,0);
		NcrossUpArLoAr = Vector3.Cross(yawLoAr2, yawLoAr).normalized;

		CompensationRotationUpAr = RVector(NcrossUpArLoAr, CompensationAngle);
		CompensationRotationLoAr = RVector(NcrossUpArLoAr, -CompensationAngle);

		// Applying Pitch Compensation 
		LoArF3 = multi(LoArF2, CompensationRotationLoAr);

		LoArB3 = multi(CompensationRotationUpAr, LoArB2);






	

		///////////// LoAr 180 degree Constriant ///////////////////
		Vector3 RollUpAr = new Vector3(UpArB2[0,0],UpArB2[1,0],UpArB2[2,0]);
			
		yawLoAr = new Vector3(LoArB3[0,1],LoArB3[1,1],LoArB3[2,1]);
		yawLoAr2 = new Vector3(UpArB2[0,1],UpArB2[1,1],UpArB2[2,1]);

		Vector3  NcrossUpArLoArRoll = new Vector3(0,0,0);
		NcrossUpArLoArRoll = Vector3.Cross(yawLoAr2, yawLoAr).normalized;
		if (Vector3.Dot(RollUpAr, yawLoAr) < 0) /// this case when not obey 180 degree constraint
		{

			OrientationError = UpArB2[0,1] * LoArB3[0,1] + UpArB2[1,1] * LoArB3[1,1] + UpArB2[2,1] * LoArB3[2,1];


			// Finding yaw compensation Angle
			if ((float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError) > (3.1415f / 2))
			{
				CompensationAngle = (float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError)  - 3.1415f;
			}
			else
			{
				//Finding yaw compensation Angle
				CompensationAngle = (float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError) ;
			}

			// Building yaw compensation rotation matrices
			CompensationRotationUpAr = RVector(NcrossUpArLoArRoll, +0.5f*CompensationAngle);
			CompensationRotationLoAr = RVector(NcrossUpArLoArRoll, -0.5f*CompensationAngle);

	

			// Applying yaw Compensation 
			LoArF4 = multi(LoArF3, CompensationRotationUpAr);
			UpArF3 = multi(UpArF2, CompensationRotationLoAr);
			LoArB4 = multi(CompensationRotationLoAr, LoArB3);
			UpArB3 = multi(CompensationRotationUpAr, UpArB2);

		}
		else  /// this case when obey 180 degree constraint just to improve LoAr angle estimation
		{

			OrientationError = UpArB2[0,1] * LoArB3[0,1] + UpArB2[1,1] * LoArB3[1,1] + UpArB2[2,1] * LoArB3[2,1];

			// Finding yaw compensation Angle
			CompensationAngle = 0;

			// Building yaw compensation rotation matrices
			CompensationRotationUpAr = RVector(NcrossUpArLoArRoll, +0.5f*CompensationAngle);
			CompensationRotationLoAr = RVector(NcrossUpArLoArRoll, -0.5f*CompensationAngle);



			// Applying yaw Compensation 
			LoArF4 = multi(LoArF3, CompensationRotationUpAr);
			UpArF3 = multi(UpArF2, CompensationRotationLoAr);
			LoArB4 = multi(CompensationRotationLoAr, LoArB3);
			UpArB3 = multi(CompensationRotationUpAr, UpArB2);

		}

		////////////////// setting to Final Body orientation lower arm///////////////////////////////
		Vector3 u = new Vector3(LoArB4[0,0] , LoArB4[1,0] , LoArB4[2,0]);
		CurrentLoArOrientation= RVector(u, -(float)3.1415 /2 );


		LoArF5 = multi(LoArF4, CurrentLoArOrientation);
		LoArB5= multi(CurrentLoArOrientation, LoArB4);



		u.Set( LoArB5[0,1] , LoArB5[1,1] , LoArB5[2,1]);
		CurrentLoArOrientation= RVector(u, (float)3.1415 );

		LoArF6 = multi(LoArF5, CurrentLoArOrientation);
		LoArB6 = multi(CurrentLoArOrientation, LoArB5);


		u.Set(1 , 0 , 0);
		CurrentLoArOrientation= RVector(u, -(float)3.1415 /2 );

		LoArF7 = multi(LoArF6, CurrentLoArOrientation);
		LoArB7 = multi(CurrentLoArOrientation, LoArB6);


		u.Set(0 , 0 , 1);
		CurrentLoArOrientation= RVector(u, (float)3.1415);

		LoArF8 = multi(LoArF7, CurrentLoArOrientation);
		LoArOrientation = multi(CurrentLoArOrientation, LoArB7);



		////////////////// setting to Final Body orientation upper arm///////////////////////////////
		Vector3 u2 = new Vector3(UpArB3[0,0] , UpArB3[1,0] , UpArB3[2,0]);
		CurrentLoArOrientation= RVector(u2, -(float)3.1415/2);

		UpArF4 = multi(UpArF3, CurrentLoArOrientation);
		UpArB4 = multi(CurrentLoArOrientation, UpArB3);

		u2.Set (UpArB4[0,1] , UpArB4[1,1] , UpArB4[2,1]);
		CurrentLoArOrientation= RVector(u2, (float)3.1415 );

		UpArF5 = multi(UpArF4, CurrentLoArOrientation);
		UpArB5 = multi(CurrentLoArOrientation, UpArB4);


		u2.Set(1 , 0 ,0);
		CurrentLoArOrientation= RVector(u2, -(float)3.1415/2 );

		UpArF6 = multi(UpArF5, CurrentLoArOrientation);
		UpArB6 = multi(CurrentLoArOrientation, UpArB5);



		u2.Set(0 , 0 ,1);
		CurrentLoArOrientation= RVector(u2, (float)3.1415 );

		UpArF7 = multi(UpArF6, CurrentLoArOrientation);
		UpArOrientation = multi(CurrentLoArOrientation, UpArB6);



	}


	public void LeftArmAngles(float [,] UpArOrientation , float [,] LoArOrientation, float[,] TorsoOrientation)

	{
		///// calculate the time difference since last call
		float vTimeDifference = Time.time - vTime;
		if (vTimeDifference == 0)
		{
			return;
		}

		vTime = Time.time;



		//////////////// calculate the Elbow Flection angle ////////////////////////////////////////

		// Axis 1 and 2 are two axes to calculate each angle 
		Vector3 vAxis1 = new Vector3(UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		Vector3 vAxis2 = new Vector3(LoArOrientation[0,2], LoArOrientation[1,2], LoArOrientation [2,2]);
		float vAngleLeftElbowFlexionNew = Vector3.Angle (vAxis1, vAxis2);

		float vAngularVelocityLeftElbowFlexionNew = (vAngleLeftElbowFlexionNew - vAngleLeftElbowFlexion) / vTimeDifference;

		vAngularAccelerationLeftElbowFlexion = (vAngularVelocityLeftElbowFlexionNew - vAngularVelocityLeftElbowFlexion) / vTimeDifference;
		vAngularVelocityLeftElbowFlexion = vAngularVelocityLeftElbowFlexionNew;
		vAngleLeftElbowFlexion = vAngleLeftElbowFlexionNew;


		//Debug.Log ("Elbow Flection Angles" + vAngleLeftElbowFlexion + ", and, " + vAngularVelocityLeftElbowFlexion + ", and, " + vAngularAccelerationLeftElbowFlexion);


		//////////////// calculate the Elbow Pronation angle ////////////////////////////////////////

		// Axis 1 and 2 are two axes to calculate each angle 
		vAxis1.Set(UpArOrientation[0,1], UpArOrientation[1,1], UpArOrientation [2,1]);
		vAxis2.Set(LoArOrientation[0,1], LoArOrientation[1,1], LoArOrientation [2,1]);
		Vector3 vAxis3 = new Vector3(UpArOrientation[0,0], UpArOrientation[1,0], UpArOrientation [2,0]);
		float vAngleLeftElbowPronationNew = Vector3.Angle (vAxis1, vAxis2);

		float vAngularVelocityLeftElbowPronationNew = (vAngleLeftElbowPronationNew - Mathf.Abs(vAngleLeftElbowPronation)) / vTimeDifference;


		if (Vector3.Dot (vAxis2 , vAxis3) > 0)
		{
			vAngleLeftElbowPronationNew = -vAngleLeftElbowPronationNew;
			vAngularVelocityLeftElbowPronationNew = -vAngularVelocityLeftElbowPronationNew;
		}

		vAngularAccelerationLeftElbowPronation = (vAngularVelocityLeftElbowPronationNew - vAngularVelocityLeftElbowPronation) / vTimeDifference;
		vAngularVelocityLeftElbowPronation = vAngularVelocityLeftElbowPronationNew;
		vAngleLeftElbowPronation = vAngleLeftElbowPronationNew;


		//Debug.Log ("Elbow Pronation Angles" + vAngleLeftElbowPronation + ", and, " + vAngularVelocityLeftElbowPronation + ", and, " + vAngularAccelerationLeftElbowPronation);


		//////////////// calculate the Left Shoulder Flection angle ////////////////////////////////////////

		vAxis1.Set (-UpArOrientation[0,2], -UpArOrientation[1,2], -UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);


		float vAngleLeftShoulderFlexionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftShoulderFlexionNew = (vAngleLeftShoulderFlexionNew - Mathf.Abs(vAngleLeftShoulderFlexion)) / vTimeDifference;

		if (Vector3.Dot (vAxis1 , vAxis3) > 0)
		{
			vAngleLeftShoulderFlexionNew = -vAngleLeftShoulderFlexionNew;
			vAngularVelocityLeftShoulderFlexionNew = -vAngularVelocityLeftShoulderFlexionNew;
		}

		vAngularAccelerationLeftShoulderFlexion = (vAngularVelocityLeftShoulderFlexionNew - vAngularVelocityLeftShoulderFlexion) / vTimeDifference;
		vAngularVelocityLeftShoulderFlexion = vAngularVelocityLeftShoulderFlexionNew;
		vAngleLeftShoulderFlexion = vAngleLeftShoulderFlexionNew;

		//Debug.Log ("Left Shoulder Flexion Angles" + vAngleLeftShoulderFlexionNew + ", and, " + vAngularVelocityLeftShoulderFlexion + ", and, " + vAngularAccelerationLeftShoulderFlexion);



		//////////////// calculate the Left Shoulder Abduction angle ////////////////////////////////////////

		vAxis1.Set (UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis1.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);


		float vAngleLeftShoulderAbductionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftShoulderAbductionNew = (vAngleLeftShoulderAbductionNew - Mathf.Abs(vAngleLeftShoulderAbduction)) / vTimeDifference;

		if (Vector3.Dot (vAxis1 , vAxis3) > 0)
		{
			vAngleLeftShoulderAbductionNew = -vAngleLeftShoulderAbductionNew;
			vAngularVelocityLeftShoulderAbductionNew = -vAngularVelocityLeftShoulderAbductionNew;
		}

		vAngularAccelerationLeftShoulderAbduction = (vAngularVelocityLeftShoulderAbductionNew - vAngularVelocityLeftShoulderAbduction) / vTimeDifference;
		vAngularVelocityLeftShoulderAbduction = vAngularVelocityLeftShoulderAbductionNew;
		vAngleLeftShoulderAbduction = vAngleLeftShoulderAbductionNew;

		//Debug.Log ("Left Shoulder Abduction Angles" + vAngleLeftShoulderAbduction + ", and, " + vAngularVelocityLeftShoulderAbduction + ", and, " + vAngularAccelerationLeftShoulderAbduction);

		//////////////// calculate the Left Shoulder Rotation angle ////////////////////////////////////////

		vAxis1.Set (UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis3 = Vector3.Cross (vAxis2, vAxis1); 
		vAxis3.Normalize();
		vAxis2.Set (UpArOrientation[0,0], UpArOrientation[1,0], UpArOrientation [2,0]);
		vAxis1.Set (UpArOrientation[0,1], UpArOrientation[1,1], UpArOrientation [2,1]);


		float vAngleLeftShoulderRotationNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftShoulderRotationNew = (vAngleLeftShoulderRotationNew - Mathf.Abs(vAngleLeftShoulderRotation)) / vTimeDifference;

		if (Vector3.Dot (vAxis1 , vAxis3) > 0)
		{
			vAngleLeftShoulderRotationNew = -vAngleLeftShoulderRotationNew;
			vAngularVelocityLeftShoulderRotationNew = -vAngularVelocityLeftShoulderRotationNew;
		}

		vAngularAccelerationLeftShoulderRotation = (vAngularVelocityLeftShoulderRotationNew - vAngularVelocityLeftShoulderRotation) / vTimeDifference;
		vAngularVelocityLeftShoulderRotation = vAngularVelocityLeftShoulderRotationNew;
		vAngleLeftShoulderRotation = vAngleLeftShoulderRotationNew;

		//Debug.Log ("Shoulder Rotation Angles" + vAngleLeftShoulderRotationNew + ", and, " + vAngularVelocityLeftShoulderRotation + ", and, " + vAngularAccelerationLeftShoulderRotation);


	}




	/**
* RotationVector()
* @It provides a rotation matrix around an arbitary vector with desired angles
* @param float a[][3], The output rotation matrix
* @param vec u, arbitary unit vector
* @param flaot t, desired angle of rotation
* @return void
*/
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

		if (NodContainer.vKey == 3)
		{
			GUI.contentColor = Color.black;

			GUI.Label (new Rect (930, 450, 400 , 25), "Left Shoulder Flexion/Extension: " + vAngleLeftShoulderFlexion , "color");

			GUI.Label (new Rect (930, 480, 400 , 25), "Left Shoulder Lateral Flexion: " + vAngleLeftShoulderAbduction , "color");

			GUI.Label (new Rect (930, 510, 400, 25), "Left Shoulder Rotation: " + vAngleLeftShoulderRotation, "color");

			GUI.Label (new Rect (1200, 450, 400, 25), "Elbow Flexion/Extension: " + vAngleLeftElbowFlexion, "color");

			GUI.Label (new Rect (1200, 480, 400 , 25), "Forearm Supination/Pronation: " + vAngleLeftElbowPronation, "color");
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
				Vector3 vNodRawEuler2 = mNodSensors[1].curRotationRawEuler;
				vNodRawEuler1 = new Vector3(vNodRawEuler1.x , vNodRawEuler1.y , vNodRawEuler1.z );
				vNodRawEuler2 = new Vector3(vNodRawEuler2.x , vNodRawEuler2.y , vNodRawEuler2.z );

				Vector3 NodIniEuler1 = mNodSensors[0].initRotationEuler;
				Vector3 NodIniEuler2 = mNodSensors[1].initRotationEuler;
				NodIniEuler1 = new Vector3(NodIniEuler1.x , NodIniEuler1.y , NodIniEuler1.z );
				NodIniEuler2 = new Vector3(NodIniEuler2.x , NodIniEuler2.y , NodIniEuler2.z );


				float[,] vaTorsoOrientation = new float[3, 3];
				vaTorsoOrientation =  NodContainer.GetTorsoOrientation();   

				ArmOrientation (vNodRawEuler1, NodIniEuler1 , vNodRawEuler2 ,  NodIniEuler2);
				LeftArmAngles(UpArOrientation ,LoArOrientation,vaTorsoOrientation);
				NodQuaternionOrientation vNodRawQuat = MatToQuat(UpArOrientation);
				Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
				Quaternion vJointQuat = inverseInitRotation * vNodQuat * Quaternion.Inverse(Quaternion.Euler(quaternionFactor));
				NodQuaternionOrientation vNodRawQuat2 = MatToQuat(LoArOrientation);
				Quaternion vNodQuat2 = new Quaternion(vNodRawQuat2.x, vNodRawQuat2.y, vNodRawQuat2.z, vNodRawQuat2.w);
				Quaternion vJointQuat2 = inverseInitRotation * vNodQuat2 * Quaternion.Inverse(Quaternion.Euler(quaternionFactor));

				if(jointTransform != null)
				{
				jointTransform.rotation = vJointQuat;
				jointTransform2.rotation = vJointQuat2;
				}
			

	}

}
