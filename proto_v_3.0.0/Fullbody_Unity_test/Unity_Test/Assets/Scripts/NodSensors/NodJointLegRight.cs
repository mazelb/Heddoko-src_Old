using UnityEngine;
using System;
using System.Collections;
using Nod;

///// joint code for the Right leg
public class NodJointLegRight : NodJoint 
{

	// current Hips and Knee joints orientation, 
	public float [,] HipOrientation = new float[3,3];
	public float [,] KneeOrientation = new float[3,3];


	// Right leg angles, angular velocities  and angular accelerations, The names are choosed based on the human body angles document
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

	// This variable stores the time of curretn frame. It is used for angular velocity and acceleration extraction
	public float vTime = 0;



	//	/**
	//* LegOrientation()
	//*	@ This function calculates and updates the current orientation of the Hips and Knee
	//	*	@param Vector3 NodHip, Hip Euler Angles Inputs
	//	*	@param Vector3  InitNodHip, this is the information of the initial frame for Hip Nod sensor
	//	*	@param Vector3 NodKnee, knee Euler Angles Inputs
	//	*	@paramVector3  InitNodKnee, this is the information of the initial frame for Kee nod sensor
	//*	@return void
	//*/
	public void LegOrientation(Vector3 NodHip , Vector3 InitNodHip , Vector3 NodKnee , Vector3 InitNodKnee)

	{
		//Intermediate arrays until achive final orienation for hip and knee, 
		//they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

		float [,] HipF1 = new float[3,3];
		float [,] HipF2 = new float[3,3];
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


		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////

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

		HipB2 = multi(HipFi, HipB1);
		KneeB2 = multi(KneeFi, KneeB1);


		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////


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
		KneeB3 = multi(CompensationRotationHip, KneeB2);
		HipB3 = multi(CompensationRotationKnee, HipB2);



		// this step applies the knee 180 constraint and can be used also for fusion of knee stretch sensors and nods in future
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
			KneeB4 = multi(CompensationRotationKnee, KneeB3);
			HipB4 = multi(CompensationRotationHip, HipB3);

		}


		///	/////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
	
		////////////////// setting Hip to Final Body orientation ///////////////////////////////

		Vector3 u = new Vector3(HipB4[0,1] , HipB4[1,1] , HipB4[2,1]);
		CurrentKneeOrientation= RVector(u, (float)3.1415 );

		HipB5 = multi(CurrentKneeOrientation, HipB4);



		u.Set(HipB5[0,2] , HipB5[1,2] , HipB5[2,2]);
		CurrentKneeOrientation= RVector(u, (float)3.1415 );

		HipB6 = multi(CurrentKneeOrientation, HipB5);

		u.Set(0 , 0 , 1);
		CurrentKneeOrientation = RVector(u, (float)3.1415 );

		HipB7 = multi(CurrentKneeOrientation, HipB6);

		u.Set(0 , 1 , 0);
		CurrentKneeOrientation = RVector(u, (float)3.1415 );

		HipOrientation = multi(CurrentKneeOrientation, HipB7);

		////////////////// setting Knee to Final Body orientation ///////////////////////////////

		Vector3 u2 = new Vector3(KneeB4[0,1] , KneeB4[1,1] , KneeB4[2,1]);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeB5 = multi(CurrentKneeOrientation, KneeB4);



		u2.Set(KneeB5[0,2] , KneeB5[1,2] , KneeB5[2,2]);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeB6 = multi(CurrentKneeOrientation, KneeB5);

		u2.Set(0 , 0 , 1);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeB7 = multi(CurrentKneeOrientation, KneeB6);

		u2.Set(0 , 1 , 0);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeOrientation = multi(CurrentKneeOrientation, KneeB7);

//		vLegHeight = KneeOrientation [1, 1] + HipOrientation [1, 1];
//		Debug.Log ("Left Knee altitude" + vLegHeight);

	}



	/************
* LegAngles()
*	@This Fuction Calculates and updates the important angles of the leg
*	@param float [,] HipOrientation, current orientation of the hip
*	@param float [,] KneeOrientation, Final orientation of the knee
**	@param float [,] TorsoOrientation, Final orientation of the Torso
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

		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////
		/// 	///	/////////////////////////////////////////////////////  Angle extraction /////////////////////////////////////////////////////////////////////

		// Axes 1 to 4 are intermediate variables used to calculate angles. 
		// In the first step, with appropriate matrix calculations each angle and angular velocities are calculated
		// In the second step, the sign of these angles will be determined and the angles will be updated


		//////////////// calculate the Knee Flection angle ////////////////////////////////////////

		/// step1 ///
		Vector3 vAxis1 = new Vector3(HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		Vector3 vAxis2 = new Vector3(KneeOrientation[0,1], KneeOrientation[1,1], KneeOrientation [2,1]);
		float vAngleKneeFlexionNew = Vector3.Angle (vAxis1, vAxis2);
		float vAngularVelocityKneeFlexionNew = (vAngleKneeFlexionNew - vAngleKneeFlexion) / vTimeDifference;

		/// step2 ///
		vAngularAccelerationKneeFlexion = (vAngularVelocityKneeFlexionNew - vAngularVelocityKneeFlexion) / vTimeDifference;
		vAngularVelocityKneeFlexion = vAngularVelocityKneeFlexionNew;
		vAngleKneeFlexion = vAngleKneeFlexionNew;


		//Debug.Log ("Knee Flection Angles" + vAngleKneeFlexion + ", and, " + vAngularVelocityKneeFlexion + ", and, " + vAngularAccelerationKneeFlexion);


		//////////////// calculate the Knee Rotation angle ////////////////////////////////////////

		/// step1 ///
		vAxis1.Set(HipOrientation[0,2], HipOrientation[1,2], HipOrientation [2,2]);
		vAxis2.Set(KneeOrientation[0,2], KneeOrientation[1,2], KneeOrientation [2,2]);
		Vector3 vAxis3 = new Vector3(HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);

		float vAngleKneeRotationNew = Vector3.Angle (vAxis1, vAxis2);
		Debug.Log ("Knee Flection Angles" + vAngleKneeRotationNew);
		float vAngularVelocityKneeRotationNew = (vAngleKneeRotationNew - Mathf.Abs(vAngleKneeRotation)) / vTimeDifference;

		/// step2 ///
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

		/// step1 ///
		vAxis1.Set (HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		float vAngleRightHipFlexionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityRightHipFlexionNew = (vAngleRightHipFlexionNew - Mathf.Abs(vAngleRightHipFlexion)) / vTimeDifference;

		/// step2 ///
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

		/// step1 ///
		vAxis1.Set (HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		float vAngleRightHipAbductionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityRightHipAbductionNew = (vAngleRightHipAbductionNew - Mathf.Abs(vAngleRightHipAbduction)) / vTimeDifference;

		/// step2 ///
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

		/// step1 ///
		vAxis1.Set (HipOrientation[0,2], HipOrientation[1,2], HipOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis1.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		float vAngleRightHipRotationNew = Vector3.Angle (vAxis3, vAxis1);
		float vAngularVelocityRightHipRotationNew = (vAngleRightHipRotationNew - Mathf.Abs(vAngleRightHipRotation)) / vTimeDifference;

		/// step2 ///
		if (Vector3.Dot (vAxis2 , vAxis3) < 0)
		{
			vAngleRightHipRotationNew = -vAngleRightHipRotationNew;
			vAngularVelocityRightHipRotationNew = -vAngularVelocityRightHipRotationNew;
		}

		vAngularAccelerationRightHipRotation = (vAngularVelocityRightHipRotationNew - vAngularVelocityRightHipRotation) / vTimeDifference;
		vAngularVelocityRightHipRotation = vAngularVelocityRightHipRotationNew;
		vAngleRightHipRotation = vAngleRightHipRotationNew;

		//Debug.Log ("Hip Rotation Angles" + vAngleRightHipRotationNew + ", and, " + vAngularVelocityRightHipRotation + ", and, " + vAngularAccelerationRightHipRotation);


	}






	/// <summary>
	// To Show appropriate Angles for each joint in GUI
	/// <summary>
	public override void OnGUIAngles()
	{

		if (NodContainer.vKey == 4)
		{
			GUI.contentColor = Color.black;

			GUI.Label (new Rect (880, 450, 400 , 25), "Right Hip Flexion/Extension: " + vAngleRightHipFlexion , "color");

			GUI.Label (new Rect (880, 480, 400 , 25), "Right Hip Abduction/Adduction: " + vAngleRightHipAbduction , "color");

			GUI.Label (new Rect (880, 510, 400, 25), "Right Hip Internal/External Rotation: " + vAngleRightHipRotation , "color");

			GUI.Label (new Rect (1180, 450, 400, 25), "Knee Flexion/Extension: " +  vAngleKneeFlexion , "color");

			GUI.Label (new Rect (1180, 480, 400 , 25), "Tibial Internal/External Rotation: " + vAngleKneeRotation , "color");


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

		// getting the current Torso orientation for Hip angle extraction
		float[,] vaTorsoOrientation = new float[3, 3];
		vaTorsoOrientation =  NodContainer.GetTorsoOrientation();

		// call LegOrientation function to calculate current orientation of leg joints
		LegOrientation (vNodRawEuler1, NodIniEuler1 , vNodRawEuler2 ,  NodIniEuler2);

		// call LegAngles function to update leg angles
		LegAngles(HipOrientation , KneeOrientation, vaTorsoOrientation);

		// convert leg orientations from 3*3 matrix to quatrenion
		NodQuaternionOrientation vNodRawQuat = MatToQuat(HipOrientation);
		Quaternion vNodQuat = new Quaternion(vNodRawQuat.x, vNodRawQuat.y, vNodRawQuat.z, vNodRawQuat.w);
		Quaternion vJointQuat = inverseInitRotation * vNodQuat * Quaternion.Inverse (Quaternion.Euler(quaternionFactor));
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
