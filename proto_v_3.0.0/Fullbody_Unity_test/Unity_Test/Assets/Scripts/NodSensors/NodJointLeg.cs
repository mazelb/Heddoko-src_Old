using UnityEngine;
using System;
using System.Collections;
using Nod;

public class NodJointLeg : NodJoint 
{
	///// joint code for the hips

	public float [,] HipOrientation = new float[3,3];
	public float [,] KneeOrientation = new float[3,3];


	public float vTime = 0;

	public float vAngleKneeFlexion = 0;
	public float vAngularVelocityKneeFlexion = 0;
	public float vAngularAccelerationKneeFlexion = 0;

	public float vAngleKneeRotation = 0;
	public float vAngularVelocityKneeRotation = 0;
	public float vAngularAccelerationKneeRotation = 0;

	public float vAngleRightHipFlexion = 0;
	public float vAngularVelocityRightHipFlexion = 0;
	public float vAngularAccelerationRightHipFlexion = 0;


	public float vAngleRightHipAbduction = 0;
	public float vAngularVelocityRightHipAbduction = 0;
	public float vAngularAccelerationRightHipAbduction = 0;


	public float vAngleRightHipRotation = 0;
	public float vAngularVelocityRightHipRotation = 0;
	public float vAngularAccelerationRightHipRotation = 0;


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
		a[0,0] = Mathf.Cos(pitch) * Mathf.Cos(yaw);
		a[1,0] = (Mathf.Sin(roll) * Mathf.Cos(yaw) * Mathf.Sin(pitch) - Mathf.Sin(yaw) * Mathf.Cos(roll));
		a[2,0] = (Mathf.Sin(roll) * Mathf.Sin(yaw) + Mathf.Cos(yaw) *Mathf.Sin(pitch)*Mathf.Cos(roll));
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
	public void LegOrientation(Vector3 NodHip , Vector3 InitNodHip , Vector3 NodKnee , Vector3 InitNodKnee)

	{
		//Intermediate arrays until achive final orienation for hip and knee, they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

		float [,] HipF1 = new float[3,3];
		float [,] HipF2 = new float[3,3];
		float [,] HipF3 = new float[3,3];
		float [,] HipF4 = new float[3,3];
		float [,] HipF5 = new float[3,3];
		float [,] HipF6 = new float[3,3];
		float [,] HipF7 = new float[3,3];
		float [,] HipF8 = new float[3,3];
		float [,] HipFi = new float[3,3];


		float [,] HipB1 = new float[3,3];
		float [,] HipB2 = new float[3,3];
		float [,] HipB3 = new float[3,3];
		float [,] HipB4 = new float[3,3];
		float [,] HipB5 = new float[3,3];
		float [,] HipB6 = new float[3,3];
		float [,] HipB7 = new float[3,3];
		float [,] HipB8 = new float[3,3];
		float [,] HipBi = new float[3,3];

		float [,] KneeF1 = new float[3,3];
		float [,] KneeF2 = new float[3,3];
		float [,] KneeF3 = new float[3,3];
		float [,] KneeF4 = new float[3,3];
		float [,] KneeF5 = new float[3,3];
		float [,] KneeF6 = new float[3,3];
		float [,] KneeF7 = new float[3,3];
		float [,] KneeF8 = new float[3,3];
		float [,] KneeFi = new float[3,3];


		float [,] KneeB1 = new float[3,3];
		float [,] KneeB2 = new float[3,3];
		float [,] KneeB3 = new float[3,3];
		float [,] KneeB4 = new float[3,3];
		float [,] KneeB5 = new float[3,3];
		float [,] KneeB6 = new float[3,3];
		float [,] KneeB7 = new float[3,3];
		float [,] KneeB8 = new float[3,3];
		float [,] KneeBi = new float[3,3];




		float [,] CompensationRotationKnee = new float[3,3];
		float [,] CompensationRotationHip = new float[3,3];
		float [,] CurrentKneeOrientation = new float[3,3];

		float OrientationError = 0;
		float CompensationAngle = 0;




		/////////// Converting to rotation matrix ///////////////////

		HipB1 = RotationLocal (NodHip.z, NodHip.x, NodHip.y);
		HipF1 = RotationGlobal (NodHip.z,NodHip.x, NodHip.y);

		KneeB1 = RotationLocal (NodKnee.z, NodKnee.x, NodKnee.y);
		KneeF1 = RotationGlobal (NodKnee.z, NodKnee.x, NodKnee.y);

		HipBi = RotationLocal (InitNodHip.z, InitNodHip.x, InitNodHip.y);
		HipFi = RotationGlobal (InitNodHip.z,InitNodHip.x, InitNodHip.y);

		KneeBi = RotationLocal (InitNodKnee.z, InitNodKnee.x, InitNodKnee.y);
		KneeFi = RotationGlobal (InitNodKnee.z, InitNodKnee.x, InitNodKnee.y);



		/////////// Initial Frame Adjustments ///////////////////

		HipF2 = multi(HipF1, HipBi);
		HipB2 = multi(HipFi, HipB1);
		KneeF2 = multi(KneeF1, KneeBi);
		KneeB2 = multi(KneeFi, KneeB1);



		///////////// Pitch Compensation Step ///////////////////
		Vector3   pitchHip = new Vector3(HipB2[0,2], HipB2[1,2], HipB2[2,2]);
		Vector3   pitchKnee = new Vector3(KneeB2[0,2], KneeB2[1,2], KneeB2[2,2]);
		Vector3   NcrossHipKnee = new Vector3(0,0,0);


		// rotation axix for pitch compensation
		NcrossHipKnee = Vector3.Cross(pitchHip, pitchKnee).normalized;
		OrientationError = HipB2[0,2] * KneeB2[0,2] + HipB2[1,2] * KneeB2[1,2] + HipB2[2,2] * KneeB2[2,2];

		// Finding Pitch compensation Angle
		CompensationAngle = (float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError);

		// Building Pitch compensation rotation matrices
		CompensationRotationHip = RVector(NcrossHipKnee, -0.5f* CompensationAngle);
		CompensationRotationKnee = RVector(NcrossHipKnee, +0.5f* CompensationAngle);

		// Applying Pitch Compensation 
		KneeF3 = multi(KneeF2, CompensationRotationKnee);
		HipF3 = multi(HipF2, CompensationRotationHip);
		KneeB3 = multi(CompensationRotationHip, KneeB2);
		HipB3 = multi(CompensationRotationKnee, HipB2);




		///////////// Knee 180 degree Constriant ///////////////////

		Vector3   RollHip = new Vector3(HipB3[0,0], HipB3[1,0], HipB3[2,0]);
		Vector3   YawKnee = new Vector3(KneeB3[0,1], KneeB3[1,1], KneeB3[2,1]);
		Vector3   YawHip = new Vector3(HipB3[0,1], HipB3[1,1], HipB3[2,1]);

		RollHip.x = HipB3[0,0];
		RollHip.y = HipB3[1,0];
		RollHip.z = HipB3[2,0];

		YawKnee.x = KneeB3[0,1];
		YawKnee.y = KneeB3[1,1];
		YawKnee.z = KneeB3[2,1];

		YawHip.x = HipB3[0,1];
		YawHip.y = HipB3[1,1];
		YawHip.z = HipB3[2,1];

		Vector3   NcrossHipKneeRoll = Vector3.Cross(YawHip, YawKnee).normalized;
		if (Vector3.Dot(RollHip, YawKnee) < 0 ) /// this case when not obey 180 degree constraint
		{

			OrientationError = HipB3[0,1] * KneeB3[0,1] + HipB3[1,1] * KneeB3[1,1] + HipB3[2,1] * KneeB3[2,1];




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
			CompensationRotationHip = RVector( NcrossHipKneeRoll, +0.5f* CompensationAngle);
			CompensationRotationKnee = RVector(NcrossHipKneeRoll, -0.5f* CompensationAngle);

			// Applying yaw Compensation 
			KneeF4 = multi(KneeF3, CompensationRotationHip);
			HipF4 = multi(HipF3, CompensationRotationKnee);
			KneeB4 = multi(CompensationRotationKnee, KneeB3);
			HipB4 = multi(CompensationRotationHip, HipB3);

		}
		else  /// this case when obey 180 degree constraint just to improve knee angle estimation
		{

			OrientationError = HipB3[0,1] * KneeB3[0,1] + HipB3[1,1] * KneeB3[1,1] + HipB3[2,1] * KneeB3[2,1];

			// Finding Pitch compensation Angle
			// stretch sensor should be added
			CompensationAngle = 0;


			// Building yaw compensation rotation matrices
			CompensationRotationHip = RVector( NcrossHipKneeRoll, +0.5f* CompensationAngle);
			CompensationRotationKnee = RVector(NcrossHipKneeRoll, -0.5f* CompensationAngle);

			// Applying yaw Compensation 
			KneeF4 = multi(KneeF3, CompensationRotationHip);
			HipF4 = multi(HipF3, CompensationRotationKnee);
			KneeB4 = multi(CompensationRotationKnee, KneeB3);
			HipB4 = multi(CompensationRotationHip, HipB3);

		}


		////////////////// setting Hip to Final Body orientation ///////////////////////////////

		Vector3 u = new Vector3(HipB4[0,1] , HipB4[1,1] , HipB4[2,1]);
		CurrentKneeOrientation= RVector(u, (float)3.1415 );

		HipF5 = multi(HipF4, CurrentKneeOrientation);
		HipB5 = multi(CurrentKneeOrientation, HipB4);



		u.Set(HipB5[0,2] , HipB5[1,2] , HipB5[2,2]);
		CurrentKneeOrientation= RVector(u, (float)3.1415 );

		HipF6 = multi(HipF5, CurrentKneeOrientation);
		HipB6 = multi(CurrentKneeOrientation, HipB5);

		u.Set(0 , 0 , 1);
		CurrentKneeOrientation = RVector(u, (float)3.1415 );

		HipF7 = multi(HipF6, CurrentKneeOrientation);
		HipB7 = multi(CurrentKneeOrientation, HipB6);

		u.Set(0 , 1 , 0);
		CurrentKneeOrientation = RVector(u, (float)3.1415 );

		HipF8 = multi(HipF7, CurrentKneeOrientation);
		HipOrientation = multi(CurrentKneeOrientation, HipB7);

		////////////////// setting Knee to Final Body orientation ///////////////////////////////

		Vector3 u2 = new Vector3(KneeB4[0,1] , KneeB4[1,1] , KneeB4[2,1]);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeF5 = multi(KneeF4, CurrentKneeOrientation);
		KneeB5 = multi(CurrentKneeOrientation, KneeB4);



		u2.Set(KneeB5[0,2] , KneeB5[1,2] , KneeB5[2,2]);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeF6 = multi(KneeF5, CurrentKneeOrientation);
		KneeB6 = multi(CurrentKneeOrientation, KneeB5);

		u2.Set(0 , 0 , 1);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeF7 = multi(KneeF6, CurrentKneeOrientation);
		KneeB7 = multi(CurrentKneeOrientation, KneeB6);

		u2.Set(0 , 1 , 0);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeF8 = multi(KneeF7, CurrentKneeOrientation);
		KneeOrientation = multi(CurrentKneeOrientation, KneeB7);

		vLegHeight = KneeOrientation [1, 1] + HipOrientation [1, 1];
		Debug.Log ("Left Knee altitude" + vLegHeight);

	}



	/************
* LegAngles()
*	@This Fuction Calculates and updates the important angles of the arm
*	@param float [,] HipOrientation, Final orientation of the Torso
**	@param float [,] KneeOrientation, Final orientation of the Torso
*	@return void
*/

	public void LegAngles(float [,] HipOrientation , float [,] KneeOrientation, float [,] TorsoOrientation )

	{
		///// calculate the time difference since last call
		float vTimeDifference = Time.time - vTime;
		if (vTimeDifference == 0)
		{
			return;
		}

		vTime = Time.time;



		//////////////// calculate the Knee Flection angle ////////////////////////////////////////

		// Axis 1 and 2 are two axes to calculate each angle 
		Vector3 vAxis1 = new Vector3(HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		Vector3 vAxis2 = new Vector3(KneeOrientation[0,1], KneeOrientation[1,1], KneeOrientation [2,1]);
		float vAngleKneeFlexionNew = Vector3.Angle (vAxis1, vAxis2);
		float vAngularVelocityKneeFlexionNew = (vAngleKneeFlexionNew - vAngleKneeFlexion) / vTimeDifference;

		vAngularAccelerationKneeFlexion = (vAngularVelocityKneeFlexionNew - vAngularVelocityKneeFlexion) / vTimeDifference;
		vAngularVelocityKneeFlexion = vAngularVelocityKneeFlexionNew;
		vAngleKneeFlexion = vAngleKneeFlexionNew;


		//Debug.Log ("Knee Flection Angles" + vAngleKneeFlexion + ", and, " + vAngularVelocityKneeFlexion + ", and, " + vAngularAccelerationKneeFlexion);


		//////////////// calculate the Knee Rotation angle ////////////////////////////////////////

		// Axis 1 and 2 are two axes to calculate each angle 
		vAxis1.Set(HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set(KneeOrientation[0,1], KneeOrientation[1,1], KneeOrientation [2,1]);
		Vector3 vAxis3 = new Vector3(HipOrientation[0,0], HipOrientation[1,0], HipOrientation [2,0]);
		float vAngleKneeRotationNew = Vector3.Angle (vAxis1, vAxis2);

		float vAngularVelocityKneeRotationNew = (vAngleKneeRotationNew - Mathf.Abs(vAngleKneeRotation)) / vTimeDifference;


		if (Vector3.Dot (vAxis2 , vAxis3) < 0)
		{
			vAngleKneeRotationNew = -vAngleKneeRotationNew;
			vAngularVelocityKneeRotationNew = -vAngularVelocityKneeRotationNew;
		}

		vAngularAccelerationKneeRotation = (vAngularVelocityKneeRotationNew - vAngularVelocityKneeRotation) / vTimeDifference;
		vAngularVelocityKneeRotation = vAngularVelocityKneeRotationNew;
		vAngleKneeRotation = vAngleKneeRotationNew;


		//Debug.Log ("Knee Rotation Angles" + vAngleKneeRotation + ", and, " + vAngularVelocityKneeRotation + ", and, " + vAngularAccelerationKneeRotation);


		//////////////// calculate the Hip Flection angle ////////////////////////////////////////

		vAxis1.Set (HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);


		float vAngleRightHipFlexionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityRightHipFlexionNew = (vAngleRightHipFlexionNew - Mathf.Abs(vAngleRightHipFlexion)) / vTimeDifference;

		if (Vector3.Dot (vAxis1 , vAxis3) < 0)
		{
			vAngleRightHipFlexionNew = -vAngleRightHipFlexionNew;
			vAngularVelocityRightHipFlexionNew = -vAngularVelocityRightHipFlexionNew;
		}

		vAngularAccelerationRightHipFlexion = (vAngularVelocityRightHipFlexionNew - vAngularVelocityRightHipFlexion) / vTimeDifference;
		vAngularVelocityRightHipFlexion = vAngularVelocityRightHipFlexionNew;
		vAngleRightHipFlexion = vAngleRightHipFlexionNew;

		//Debug.Log ("EHip Flexion Angles" + vAngleRightHipFlexionNew + ", and, " + vAngularVelocityRightHipFlexion + ", and, " + vAngularAccelerationRightHipFlexion);



		//////////////// calculate the Hip Abduction angle ////////////////////////////////////////

		vAxis1.Set (HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);


		float vAngleRightHipAbductionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityRightHipAbductionNew = (vAngleRightHipAbductionNew - Mathf.Abs(vAngleRightHipAbduction)) / vTimeDifference;

		if (Vector3.Dot (vAxis1 , vAxis3) > 0)
		{
			vAngleRightHipAbductionNew = -vAngleRightHipAbductionNew;
			vAngularVelocityRightHipAbductionNew = -vAngularVelocityRightHipAbductionNew;
		}

		vAngularAccelerationRightHipAbduction = (vAngularVelocityRightHipAbductionNew - vAngularVelocityRightHipAbduction) / vTimeDifference;
		vAngularVelocityRightHipAbduction = vAngularVelocityRightHipAbductionNew;
		vAngleRightHipAbduction = vAngleRightHipAbductionNew;

		//Debug.Log ("Hip Abduction Angles" + vAngleRightHipAbductionNew + ", and, " + vAngularVelocityRightHipAbduction + ", and, " + vAngularAccelerationRightHipAbduction);


		//////////////// calculate the Hip Rotation angle ////////////////////////////////////////

		vAxis1.Set (HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis1.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);


		float vAngleRightHipRotationNew = Vector3.Angle (vAxis3, vAxis1);
		float vAngularVelocityRightHipRotationNew = (vAngleRightHipRotationNew - Mathf.Abs(vAngleRightHipRotation)) / vTimeDifference;

		if (Vector3.Dot (vAxis2 , vAxis3) > 0)
		{
			vAngleRightHipRotationNew = -vAngleRightHipRotationNew;
			vAngularVelocityRightHipRotationNew = -vAngularVelocityRightHipRotationNew;
		}

		vAngularAccelerationRightHipRotation = (vAngularVelocityRightHipRotationNew - vAngularVelocityRightHipRotation) / vTimeDifference;
		vAngularVelocityRightHipRotation = vAngularVelocityRightHipRotationNew;
		vAngleRightHipRotation = vAngleRightHipRotationNew;

		//Debug.Log ("Hip Rotation Angles" + vAngleRightHipRotationNew + ", and, " + vAngularVelocityRightHipRotation + ", and, " + vAngularAccelerationRightHipRotation);


	}



//
//		/**
//	* RotationVector()
//	* @It provides a rotation matrix around an arbitary vector with desired angles
//	* @param float a[][3], The output rotation matrix
//	* @param vec u, arbitary unit vector
//	* @param flaot t, desired angle of rotation
//	* @return void
//	*/
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

		if (NodContainer.vKey == 4)
		{
			GUI.contentColor = Color.black;

			GUI.Label (new Rect (930, 450, 400 , 25), "Right Hip Flexion/Extension: " + vAngleRightHipFlexion , "color");

			GUI.Label (new Rect (930, 480, 400 , 25), "Right Hip Lateral Flexion: " + vAngleRightHipAbduction , "color");

			GUI.Label (new Rect (930, 510, 400, 25), "Right Hip Rotation: " + vAngleRightHipRotation , "color");

			GUI.Label (new Rect (1200, 450, 400, 25), "Knee Flexion/Extension: " +  vAngleKneeFlexion , "color");

			GUI.Label (new Rect (1200, 480, 400 , 25), "Knee Tibial Rotation: " + vAngleKneeRotation , "color");


		}

	}



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

		LegOrientation (vNodRawEuler1, NodIniEuler1 , vNodRawEuler2 ,  NodIniEuler2);
		LegAngles(HipOrientation , KneeOrientation, vaTorsoOrientation);

		NodQuaternionOrientation vNodRawQuat = MatToQuat(HipOrientation);
		Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
		Quaternion vJointQuat = inverseInitRotation * vNodQuat * Quaternion.Inverse(Quaternion.Euler(quaternionFactor));
		NodQuaternionOrientation vNodRawQuat2 = MatToQuat(KneeOrientation);
		Quaternion vNodQuat2 = new Quaternion(vNodRawQuat2.x, vNodRawQuat2.y, vNodRawQuat2.z, vNodRawQuat2.w);
		Quaternion vJointQuat2 = inverseInitRotation * vNodQuat2 * Quaternion.Inverse(Quaternion.Euler(quaternionFactor));

		if(jointTransform != null)
		{
			jointTransform.rotation = vJointQuat;
			jointTransform2.rotation = vJointQuat2;
		}

	}

}
