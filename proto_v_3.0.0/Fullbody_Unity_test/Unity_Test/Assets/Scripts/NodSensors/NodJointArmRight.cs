using UnityEngine;
using System;
using System.Collections;
using Nod;


///// joint code for the Right Arm
public class NodJointArmRight : NodJoint 
{
	// current Upperarm (shoulder) and lower arm (Elbow) joints orientation, UpAr stands for upper arm and LoAr stands for Lower arm (forearm) in this code
	public float [,] UpArOrientation = new float[3,3];
	public float [,] LoArOrientation = new float[3,3];




	// Right Arm Extracted Angles, Angular velocities  and Angular accelerations, The names are choosed based on the human body angles document
	public float vAngleRightElbowFlexion = 0;
	public float vAngularVelocityRightElbowFlexion = 0;
	public float vAngularAccelerationRightElbowFlexion = 0;

	public float vAngleRightElbowPronation = 0;
	public float vAngularVelocityRightElbowPronation = 0;
	public float vAngularAccelerationRightElbowPronation = 0;

	public float vAngleRightShoulderFlexion = 0;
	public float vAngularVelocityRightShoulderFlexion = 0;
	public float vAngularAccelerationRightShoulderFlexion = 0;


	public float vAngleRightShoulderAbduction = 0;
	public float vAngularVelocityRightShoulderAbduction = 0;
	public float vAngularAccelerationRightShoulderAbduction = 0;

	public float vAngleRightShoulderRotation = 0;
	public float vAngularVelocityRightShoulderRotation = 0;
	public float vAngularAccelerationRightShoulderRotation = 0;


	// This variable stores the time of curretn frame. It is used for angular velocity and acceleration extraction
	public float vTime = 0;



//


//	/**
//* ArmOrientation()
//*	@ This function calculates and updates the current orientation of the shoulder and elbow joints
//	*	@param Vector3 NodUpAr, UpperArm Euler Angles Inputs
//	*	@paramVector3  InitNodUpAr, this is the information of the initial frame for Upper Arm joint
//	*	@param Vector3 NodLoAr, forearm Euler Angles Inputs
//	*	@paramVector3  InitNodLoAr, this is the information of the initial frame for forearm joint
//*	@return void
//*/
	public void ArmOrientation(Vector3 NodUpAr , Vector3 InitNodUpAr , Vector3 NodLoAr , Vector3 InitNodLoAr)

	{
		//Intermediate arrays until achive final orienation for shoulder and elbow
		//they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively
		// UpAr stands for upper arm sensor orientation and lower arm stands for lower arm (forearm) orientation

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
		float [,] CurrentLoArOrientation =   new float[3,3];

		float OrientationError = 0;
		float CompensationAngle = 0;


		Vector3 pitchUpAr = new Vector3(0,0,0);
		Vector3  pitchLoAr = new Vector3(0,0,0);

	


		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////



		/////////// Converting to orientaion matrix ///////////////////

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







		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////



		///////////// Yaw Compensation Step ///////////////////


		// finding compensated yaw axis of lower arm in one plane with upperarm
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

		// Applying yaw Compensation 
		LoArF3 = multi(LoArF2, CompensationRotationLoAr);

		LoArB3 = multi(CompensationRotationUpAr, LoArB2);






	
		// this step applies the elbow 180 constraint and can be used also for fusion of elbow stretch sensors and nods in future
		///////////// Elbow 180 degree Constriant ///////////////////

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




		///	/////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////


		////////////////// setting to Final Body orientation lower arm ///////////////////////////////

		Vector3 u = new Vector3(LoArB4[0,1] , LoArB4[1,1] , LoArB4[2,1]);
		CurrentLoArOrientation= RVector(u, (float)3.1415 );


		LoArF5 = multi(LoArF4, CurrentLoArOrientation);
		LoArB5 = multi(CurrentLoArOrientation, LoArB4);



		u.Set( LoArB5[0,0] , LoArB5[1,0] , LoArB5[2,0]);
		CurrentLoArOrientation= RVector(u, -(float)3.1415/2 );

		LoArF6 = multi(LoArF5, CurrentLoArOrientation);
		LoArB6 = multi(CurrentLoArOrientation, LoArB5);

		u.Set(1 , 0 , 0);
		CurrentLoArOrientation= RVector(u, (float)3.1415 /2 );

		LoArF7 = multi(LoArF6, CurrentLoArOrientation);
		LoArB7 = multi(CurrentLoArOrientation, LoArB6);


		u.Set(0 , 0 , 1);
		CurrentLoArOrientation= RVector(u, (float)3.1415);

		LoArF8 = multi(LoArF7, CurrentLoArOrientation);
		LoArOrientation = multi(CurrentLoArOrientation, LoArB7);



		////////////////// setting to Final Body orientation upper arm///////////////////////////////
		Vector3 u2 = new Vector3(UpArB3[0,1] , UpArB3[1,1] , UpArB3[2,1]);
		CurrentLoArOrientation= RVector(u2, (float)3.1415 );

		UpArF4 = multi(UpArF3, CurrentLoArOrientation);
		UpArB4 = multi(CurrentLoArOrientation, UpArB3);

		u2.Set (UpArB4[0,0] , UpArB4[1,0] , UpArB4[2,0]);
		CurrentLoArOrientation= RVector(u2, -(float)3.1415/2 );

		UpArF5 = multi(UpArF4, CurrentLoArOrientation);
		UpArB5 = multi(CurrentLoArOrientation, UpArB4);

		u2.Set(1 , 0 ,0);
		CurrentLoArOrientation= RVector(u2, (float)3.1415/2 );

		UpArF6 = multi(UpArF5, CurrentLoArOrientation);
		UpArB6 = multi(CurrentLoArOrientation, UpArB5);



		u2.Set(0 , 0 ,1);
		CurrentLoArOrientation= RVector(u2, (float)3.1415 );

		UpArF7 = multi(UpArF6, CurrentLoArOrientation);
		UpArOrientation = multi(CurrentLoArOrientation, UpArB6);


	}






	//	/*
	//* RightArmAngles()
	//*	@This Fuction Calculates and updates the important angles of the arm
	//*	@param float [,] UpArOrientation, current orientation of the upper arm
	//**@param float [,] LoArOrientation, current orientation of the lower arm
	//**@param float [,] TorsoOrientation, current orientation of Torso
	//*	@return void
	//*/

	public void RightArmAngles(float [,] UpArOrientation , float [,] LoArOrientation , float [,] TorsoOrientation)

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
		/// 

		// Axes 1 to 4 are intermediate variables used to calculate angles. 
		// In the first step, with appropriate matrix calculations each angle and angular velocities are calculated
		// In the second step, the sign of these angles will be determined and the angles will be updated


		//////////////// calculate the Elbow Flection angle ////////////////////////////////////////



		/// step 1///
		Vector3 vAxis1 = new Vector3(UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		Vector3 vAxis2 = new Vector3(LoArOrientation[0,2], LoArOrientation[1,2], LoArOrientation [2,2]);
		float vAngleRightElbowFlexionNew = Vector3.Angle (vAxis1, vAxis2);
		float vAngularVelocityRightElbowFlexionNew = (vAngleRightElbowFlexionNew - vAngleRightElbowFlexion) / vTimeDifference;

		/// step 2///
		vAngularAccelerationRightElbowFlexion = (vAngularVelocityRightElbowFlexionNew - vAngularVelocityRightElbowFlexion) / vTimeDifference;
		vAngularVelocityRightElbowFlexion = vAngularVelocityRightElbowFlexionNew;
		vAngleRightElbowFlexion = vAngleRightElbowFlexionNew;


		//Debug.Log ("Elbow Flection Angles" + vAngleRightElbowFlexion + ", and, " + vAngularVelocityRightElbowFlexion + ", and, " + vAngularAccelerationRightElbowFlexion);


		//////////////// calculate the Elbow Pronation angle ////////////////////////////////////////

		/// step 1///
		vAxis1.Set(UpArOrientation[0,1], UpArOrientation[1,1], UpArOrientation [2,1]);
		vAxis2.Set(LoArOrientation[0,1], LoArOrientation[1,1], LoArOrientation [2,1]);
		Vector3 vAxis3 = new Vector3(UpArOrientation[0,0], UpArOrientation[1,0], UpArOrientation [2,0]);
		float vAngleRightElbowPronationNew = Vector3.Angle (vAxis1, vAxis2);
		float vAngularVelocityRightElbowPronationNew = (vAngleRightElbowPronationNew - Mathf.Abs(vAngleRightElbowPronation)) / vTimeDifference;

		/// step 2///
		if (Vector3.Dot (vAxis2 , vAxis3) < 0)
		{
			vAngleRightElbowPronationNew = -vAngleRightElbowPronationNew;
			vAngularVelocityRightElbowPronationNew = -vAngularVelocityRightElbowPronationNew;
		}
		vAngularAccelerationRightElbowPronation = (vAngularVelocityRightElbowPronationNew - vAngularVelocityRightElbowPronation) / vTimeDifference;
		vAngularVelocityRightElbowPronation = vAngularVelocityRightElbowPronationNew;
		vAngleRightElbowPronation = vAngleRightElbowPronationNew;


		//Debug.Log ("Elbow Pronation Angles" + vAngleRightElbowPronation + ", and, " + vAngularVelocityRightElbowPronation + ", and, " + vAngularAccelerationRightElbowPronation);


		//////////////// calculate the Shoulder Flection angle ////////////////////////////////////////



		/// step 1///
		vAxis1.Set (UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		float vAngleRightShoulderFlexionNew = 180 - Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityRightShoulderFlexionNew = (vAngleRightShoulderFlexionNew - Mathf.Abs(vAngleRightShoulderFlexion)) / vTimeDifference;


		/// step 2///
		if (Vector3.Dot (vAxis1 , vAxis3) > 0)
		{
			vAngleRightShoulderFlexionNew = -vAngleRightShoulderFlexionNew;
			vAngularVelocityRightShoulderFlexionNew = -vAngularVelocityRightShoulderFlexionNew;
		}

		vAngularAccelerationRightShoulderFlexion = (vAngularVelocityRightShoulderFlexionNew - vAngularVelocityRightShoulderFlexion) / vTimeDifference;
		vAngularVelocityRightShoulderFlexion = vAngularVelocityRightShoulderFlexionNew;
		vAngleRightShoulderFlexion = vAngleRightShoulderFlexionNew;

		//Debug.Log ("EShoulder Flexion Angles" + vAngleRightShoulderFlexionNew + ", and, " + vAngularVelocityRightShoulderFlexion + ", and, " + vAngularAccelerationRightShoulderFlexion);



		//////////////// calculate the Shoulder Abduction angle ////////////////////////////////////////

		/// step 1///
		vAxis1.Set (UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		float vAngleRightShoulderAbductionNew = Vector3.Angle (vAxis3, -vAxis2);
		float vAngularVelocityRightShoulderAbductionNew = (vAngleRightShoulderAbductionNew - Mathf.Abs(vAngleRightShoulderAbduction)) / vTimeDifference;

		/// step 2///
		if (Vector3.Dot (vAxis1 , vAxis3) < 0)
		{
			vAngleRightShoulderAbductionNew = -vAngleRightShoulderAbductionNew;
			vAngularVelocityRightShoulderAbductionNew = -vAngularVelocityRightShoulderAbductionNew;
		}

		vAngularAccelerationRightShoulderAbduction = (vAngularVelocityRightShoulderAbductionNew - vAngularVelocityRightShoulderAbduction) / vTimeDifference;
		vAngularVelocityRightShoulderAbduction = vAngularVelocityRightShoulderAbductionNew;
		vAngleRightShoulderAbduction = vAngleRightShoulderAbductionNew;

		Debug.Log ("Shoulder Abduction Angles" + vAngleRightShoulderAbductionNew + ", and, " + vAngularVelocityRightShoulderAbduction + ", and, " + vAngularAccelerationRightShoulderAbduction);



		//////////////// calculate the Shoulder Rotation angle ////////////////////////////////////////

		/// step 1///
		vAxis1.Set (UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis3 = Vector3.Cross (vAxis2, vAxis1); 
		vAxis3.Normalize();
		vAxis2.Set (UpArOrientation[0,0], UpArOrientation[1,0], UpArOrientation [2,0]);
		vAxis1.Set (UpArOrientation[0,1], UpArOrientation[1,1], UpArOrientation [2,1]);
		float vAngleRightShoulderRotationNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityRightShoulderRotationNew = (vAngleRightShoulderRotationNew - Mathf.Abs(vAngleRightShoulderRotation)) / vTimeDifference;

		/// step 2///
		if (Vector3.Dot (vAxis1 , vAxis3) > 0)
		{
			vAngleRightShoulderRotationNew = -vAngleRightShoulderRotationNew;
			vAngularVelocityRightShoulderRotationNew = -vAngularVelocityRightShoulderRotationNew;
		}
		vAngularAccelerationRightShoulderRotation = (vAngularVelocityRightShoulderRotationNew - vAngularVelocityRightShoulderRotation) / vTimeDifference;
		vAngularVelocityRightShoulderRotation = vAngularVelocityRightShoulderRotationNew;
		vAngleRightShoulderRotation = vAngleRightShoulderRotationNew;

		//Debug.Log ("Shoulder Rotation Angles" + vAngleRightShoulderRotationNew + ", and, " + vAngularVelocityRightShoulderRotation + ", and, " + vAngularAccelerationRightShoulderRotation);

	}








	//////////////////////////// These are the reference function for matrix calculation and transformation ///////////////////////////////

	/**
	//	* RotationGlobal()
	//	* @ This Performs transformation From Global Coordinate System To local Nod coordinates
	//	* @params yaw pitch and roll of each nod
	//	* @return 3*3 orientation matrix in nod local coordinate system
	//	*/// 
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
	//	* RotationLocal()
	//	* @ This Performs transformation From Nods Local Coordinate System To global oordinates
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



	//
	//	/**
	//* SIGN()
	//* @It provides sign of an input
	//* @param float x is the input
	//* @return float sign of the x
	//*/
	public static float SIGN(float x) { return (x >= 0.0f) ? +1.0f : -1.0f; }




	//	/**
	//* MatToQuat
	//* @It converts a Matrix to a Quatrenion
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


	//	/**
	//* multi()
	//*	@This Fuction do multiplication between two 3*3 matrices
	//*	@param matrix a and b
	//*	@return c = a * b,
	//*/
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

		if (NodContainer.vKey == 2)
		{
			GUI.contentColor = Color.black;

			GUI.Label (new Rect (880, 450, 400 , 25), "Right Shoulder Flexion/Extension: " + vAngleRightShoulderFlexion , "color");

			GUI.Label (new Rect (880, 480, 400 , 25), "Right Shoulder Abduction/Adduction: " + vAngleRightShoulderAbduction , "color");

			GUI.Label (new Rect (880, 510, 400, 25), "Right Shoulder Internal/External Rotation: " + vAngleRightShoulderRotation, "color");

			GUI.Label (new Rect (1180, 450, 400, 25), "Elbow Flexion/Extension: " + vAngleRightElbowFlexion, "color");

			GUI.Label (new Rect (1180, 480, 400 , 25), "Forearm Supination/Pronation: " + vAngleRightElbowPronation, "color");
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

				// getting the current Torso orientation for shoulder angle extraction
				float[,] vaTorsoOrientation = new float[3, 3];
				vaTorsoOrientation =  NodContainer.GetTorsoOrientation();     	
				
				// call ArmOrientation function to calculate current orientation of the Shoulder and Elbow
				ArmOrientation (vNodRawEuler1, NodIniEuler1 , vNodRawEuler2 ,  NodIniEuler2);

				// call RightArmAngles function to update curretn right arm angles 
				RightArmAngles(UpArOrientation ,LoArOrientation , vaTorsoOrientation );

				// convert upper arm and forearm orientation from 3*3 matrix to quatrenion
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
