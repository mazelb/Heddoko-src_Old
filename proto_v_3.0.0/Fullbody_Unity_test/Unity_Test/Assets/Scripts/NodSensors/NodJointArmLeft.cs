using UnityEngine;
using System;
using System.Collections;
using Nod;
using System.IO;

///// joint code for the Left Arm
public class NodJointArmLeft : NodJoint 
{
	// current Upperarm (shoulder) and lower arm (Elbow) joints orientation, UpAr stands for upper arm and LoAr stands for Lower arm (forearm) in this code
	public float [,] UpArOrientation = new float[3,3];
	public float [,] LoArOrientation = new float[3,3];



	// Left Arm Extracted Angles, Angular velocities  and Angular accelerations, The names are chose based on the human body angles document
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


	// This variable stores the time of current frame. It is used for angular velocity and acceleration extraction
	public float vTime = 0;


    public Vector3 vNodRawAcc0;
    public Vector3 vNodRawAcc1;
    public Vector3 vNodInitAcc1;
    public Vector3 vNodInitAcc0;
    public Vector3 vNodRawEuler0;
    public Vector3 vNodRawEuler1;

    //	* GravityRef()
    //	*	@This Function makes gravity as a reference axis for tracking purposes
    //	*	@param matrix B2 and vNodInitAcc
    //	*	@returns 3*3 matrix B4 ,
    //	*/
    public float[,] GravityRefArm(float[,] B2, Vector3 vNodInitAcc)
    {
        float[,] B3 = new float[3, 3];
        float[,] B4 = new float[3, 3];

        //Vector3 u1 = new Vector3(B2[0, 1], B2[1, 1], B2[2, 1]);
        Vector3 u1 = new Vector3(B2[0, 2], B2[1, 2], B2[2, 2]);
        Vector3 v1 = multiMatrixVector(B2, vNodInitAcc);
        float vAngleNew1 = Vector3.Angle(u1, v1);
        Vector3 Ncross1 = Vector3.Cross(u1, v1).normalized;
        float[,] CurrentOrientation = new float[3, 3];
        CurrentOrientation = RVector(Ncross1, -(float)Math.PI * (vAngleNew1 -7.6f/ 180f));

        B3 = multi(CurrentOrientation, B2);

        ////cross product of initial gravity vector and yaw 
        Vector3 pitch = new Vector3(0, 0, 1);
        Vector3 Ncross = Vector3.Cross(vNodInitAcc, pitch).normalized;
        float CompensationAngle = Vector3.Angle(vNodInitAcc, pitch);

        float[,] CompensationRotation = new float[3, 3];
        CompensationRotation = RVector(Ncross, -(float)Math.PI * (CompensationAngle-7.6f / 180f));

        print("vAngleNew1=" + vAngleNew1 + "CompensationAngle= " + CompensationAngle + " vNodInitAcc= " + vNodInitAcc + " Ncross =" + Ncross + " CompensationAngle =" + CompensationAngle);
        B4 = multi(CompensationRotation, B3);

        if (vNodInitAcc.x == 0 && vNodInitAcc.y == 0 && vNodInitAcc.z == 0)
        {
            B4 = B2;
        }

        return B4;
    }



    //	/**
    //* ArmOrientation()
    //*	@ This function calculates and updates the current orientation of the shoulder and elbow joints
    //	*	@param Vector3 NodUpAr, UpperArm Euler Angles Inputs
    //	*	@paramVector3  InitNodUpAr, this is the information of the initial frame for Upper Arm joint
    //	*	@param Vector3 NodLoAr, forearm Euler Angles Inputs
    //	*	@paramVector3  InitNodLoAr, this is the information of the initial frame for Torso joint
    //*	@return void
    //*/
    public void ArmOrientation(Vector3 NodUpAr , Vector3 InitNodUpAr , Vector3 NodLoAr , Vector3 InitNodLoAr)

	{
		//Intermediate arrays until achieve final orientation for shoulder and elbow
		//they are Tagged with F (forward rotation) and B (Backward rotation) and are numbered consecutively
		// UpAr stands for upper arm sensor orientation and lower arm stands for lower arm (forearm) orientation

		float [,] UpArF1 = new float[3,3];
		float [,] UpArF2 = new float[3,3];
		float [,] UpArFi = new float[3,3];

		float [,] UpArB1 = new float[3,3];
		float [,] UpArB2 = new float[3,3];
		float [,] UpArB3 = new float[3,3];
		float [,] UpArB4 = new float[3,3];
		float [,] UpArB5 = new float[3,3];
		float [,] UpArB6 = new float[3,3];
		float [,] UpArB7 = new float[3,3];
		float [,] UpArBi = new float[3,3];
        float [,] UpArB21 = new float[3, 3];
        float [,] UpArB22 = new float[3, 3];
        
        float [,] LoArF1 = new float[3,3];
		float [,] LoArF2 = new float[3,3];
		float [,] LoArFi = new float[3,3];

		float [,] LoArB1 = new float[3,3];
		float [,] LoArB2 = new float[3,3];
		float [,] LoArB3 = new float[3,3];
		float [,] LoArB4 = new float[3,3];
		float [,] LoArB5 = new float[3,3];
		float [,] LoArB6 = new float[3,3];
		float [,] LoArB7 = new float[3,3];
		float [,] LoArBi = new float[3,3];
        float [,] LoArB21 = new float[3, 3];
        float [,] LoArB22 = new float[3, 3];



        float [,] CompensationRotationLoAr = new float[3,3];
		float [,] CompensationRotationUpAr = new float[3,3];
		float [,] CurrentLoArOrientation = new float[3,3];

		float OrientationError = 0;
		float CompensationAngle = 0;


		Vector3 pitchUpAr = new Vector3(0,0,0);
		Vector3 pitchLoAr = new Vector3(0,0,0);

		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////


		/////////// Converting to orientation matrix ///////////////////

		UpArB1 = RotationLocal (NodUpAr.z, NodUpAr.x, NodUpAr.y);
		UpArF1 = RotationGlobal (NodUpAr.z,NodUpAr.x, NodUpAr.y);

		LoArB1 = RotationLocal (NodLoAr.z, NodLoAr.x, NodLoAr.y);
		LoArF1 = RotationGlobal (NodLoAr.z,NodLoAr.x, NodLoAr.y);

		UpArBi = RotationLocal (InitNodUpAr.z, InitNodUpAr.x, InitNodUpAr.y);
		UpArFi = RotationGlobal (InitNodUpAr.z,InitNodUpAr.x, InitNodUpAr.y);

		LoArBi = RotationLocal (InitNodLoAr.z, InitNodLoAr.x, InitNodLoAr.y);
		LoArFi = RotationGlobal (InitNodLoAr.z, InitNodLoAr.x, InitNodLoAr.y);



        /////////// Initial Frame Adjustments ///////////////////

        //UpArB2 = multi(UpArFi, UpArB1);
        //LoArB2 = multi(LoArFi, LoArB1);

        /////////////// Reference based on gravity ////////////////// 
        UpArB21 = multi(UpArFi, UpArB1);
        LoArB21 = multi(LoArFi, LoArB1);

        UpArB22 = TiltNod(UpArB21);
        LoArB22 = TiltNod(LoArB21);

        //UpArB2 = Tilt(UpArB21);
        //LoArB2 = Tilt(LoArB21);

        UpArB2 = GravityRefArm(UpArB22, vNodInitAcc0);
        LoArB2 = GravityRefArm(LoArB22, vNodInitAcc1);

        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////




        ///////////// Yaw Compensation Step ///////////////////

        if (NodContainer.Fusion)
        {
            // finding compensated yaw in one plane with upperarm
            float temp1, temp2, temp3, temp4;
            temp1 = LoArB2[0, 1] - (UpArB2[0, 2] * LoArB2[0, 1] + UpArB2[1, 2] * LoArB2[1, 1] + UpArB2[2, 2] * LoArB2[2, 1]) * UpArB2[0, 2];
            temp2 = LoArB2[1, 1] - (UpArB2[0, 2] * LoArB2[0, 1] + UpArB2[1, 2] * LoArB2[1, 1] + UpArB2[2, 2] * LoArB2[2, 1]) * UpArB2[1, 2];
            temp3 = LoArB2[2, 1] - (UpArB2[0, 2] * LoArB2[0, 1] + UpArB2[1, 2] * LoArB2[1, 1] + UpArB2[2, 2] * LoArB2[2, 1]) * UpArB2[2, 2];
            temp4 = (float)Math.Sqrt((float)(temp1 * temp1 + temp2 * temp2 + temp3 * temp3));
            Vector3 yawLoAr = new Vector3(temp1 / temp4, temp2 / temp4, temp3 / temp4);

            // Finding yaw compensation Angle
            OrientationError = yawLoAr.x * LoArB2[0, 1] + yawLoAr.y * LoArB2[1, 1] + yawLoAr.z * LoArB2[2, 1];
            CompensationAngle = (float)Math.Acos(OrientationError > 1.00f ? 1f : OrientationError);

            // Finding yaw compensation axis
            Vector3 yawLoAr2 = new Vector3(LoArB2[0, 1], LoArB2[1, 1], LoArB2[2, 1]);

            Vector3 NcrossUpArLoAr = new Vector3(0, 0, 0);
            NcrossUpArLoAr = Vector3.Cross(yawLoAr2, yawLoAr).normalized;

            CompensationRotationUpAr = RVector(NcrossUpArLoAr, CompensationAngle);
            CompensationRotationLoAr = RVector(NcrossUpArLoAr, -CompensationAngle);

            // Applying yaw Compensation 
            LoArB3 = multi(CompensationRotationUpAr, LoArB2);

            //LoArB3=LoArB2;

            // this step applies the elbow 180 constraint and can be used also for fusion of elbow stretch sensors and nods in future
            ///////////// LoAr 180 degree Constraint ///////////////////
            Vector3 RollUpAr = new Vector3(UpArB2[0, 0], UpArB2[1, 0], UpArB2[2, 0]);

            yawLoAr = new Vector3(LoArB3[0, 1], LoArB3[1, 1], LoArB3[2, 1]);
            yawLoAr2 = new Vector3(UpArB2[0, 1], UpArB2[1, 1], UpArB2[2, 1]);

            Vector3 NcrossUpArLoArRoll = new Vector3(0, 0, 0);
            NcrossUpArLoArRoll = Vector3.Cross(yawLoAr2, yawLoAr).normalized;
            if (Vector3.Dot(RollUpAr, yawLoAr) > 0) /// this case when not obey 180 degree constraint
            {

                OrientationError = UpArB2[0, 1] * LoArB3[0, 1] + UpArB2[1, 1] * LoArB3[1, 1] + UpArB2[2, 1] * LoArB3[2, 1];


                // Finding yaw compensation Angle

                CompensationAngle = (float)Math.Acos(OrientationError > 1.00f ? 1f : OrientationError);

                // Building yaw compensation rotation matrices
                CompensationRotationUpAr = RVector(NcrossUpArLoArRoll, +0.5f * CompensationAngle);
                CompensationRotationLoAr = RVector(NcrossUpArLoArRoll, -0.5f * CompensationAngle);



                // Applying yaw Compensation 
                LoArB4 = multi(CompensationRotationLoAr, LoArB3);
                UpArB3 = multi(CompensationRotationUpAr, UpArB2);

            }
            else  /// this case when obey 180 degree constraint just to improve LoAr angle estimation
            {

                OrientationError = UpArB2[0, 1] * LoArB3[0, 1] + UpArB2[1, 1] * LoArB3[1, 1] + UpArB2[2, 1] * LoArB3[2, 1];

                // Finding yaw compensation Angle
                CompensationAngle = 0;

                // Building yaw compensation rotation matrices
                CompensationRotationUpAr = RVector(NcrossUpArLoArRoll, +0.5f * CompensationAngle);
                CompensationRotationLoAr = RVector(NcrossUpArLoArRoll, -0.5f * CompensationAngle);



                // Applying yaw Compensation 
                LoArB4 = multi(CompensationRotationLoAr, LoArB3);
                UpArB3 = multi(CompensationRotationUpAr, UpArB2);

            }
        }
        else
        {
            LoArB4 = LoArB2;
            UpArB3 = UpArB2;
        }


		///	/////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////
		/// /////////////////////////////////////////////////////  Mapping /////////////////////////////////////////////////////////////////////


		////////////////// setting to Final Body orientation lower arm///////////////////////////////
		Vector3 u = new Vector3(LoArB4[0,0] , LoArB4[1,0] , LoArB4[2,0]);
		CurrentLoArOrientation= RVector(u, (float)3.1415 /2 );
		//CurrentLoArOrientation= RVector(u, -(float)3.1415 /2 );

		LoArB5= multi(CurrentLoArOrientation, LoArB4);



		u.Set( LoArB5[0,1] , LoArB5[1,1] , LoArB5[2,1]);
		CurrentLoArOrientation= RVector(u, (float)0.0);
	//	CurrentLoArOrientation= RVector(u, (float)3.1415 );
		LoArB6 = multi(CurrentLoArOrientation, LoArB5);


		u.Set(1 , 0 , 0);
		CurrentLoArOrientation= RVector(u, -(float)3.1415 /2 );

		LoArB7 = multi(CurrentLoArOrientation, LoArB6);


		u.Set(0 , 0 , 1);
//		CurrentLoArOrientation= RVector(u, (float)3.1415);
		CurrentLoArOrientation= RVector(u, (float)0.0);
		LoArOrientation = multi(CurrentLoArOrientation, LoArB7);



		////////////////// setting to Final Body orientation upper arm///////////////////////////////
		Vector3 u2 = new Vector3(UpArB3[0,0] , UpArB3[1,0] , UpArB3[2,0]);
		CurrentLoArOrientation= RVector(u2, (float)3.1415/2);
//		CurrentLoArOrientation= RVector(u2, (float)3.1415/2);

		
		UpArB4 = multi(CurrentLoArOrientation, UpArB3);

		u2.Set (UpArB4[0,1] , UpArB4[1,1] , UpArB4[2,1]);
		CurrentLoArOrientation= RVector(u2, (float)0.0 );
//		CurrentLoArOrientation= RVector(u2, (float)3.1415 );
	

		UpArB5 = multi(CurrentLoArOrientation, UpArB4);


		u2.Set(1 , 0 ,0);
		CurrentLoArOrientation= RVector(u2, -(float)3.1415/2 );

		UpArB6 = multi(CurrentLoArOrientation, UpArB5);


		u2.Set(0 , 0 ,1);
		CurrentLoArOrientation= RVector(u2, (float)0.0);
//CurrentLoArOrientation= RVector(u2, (float)3.1415 );
		UpArOrientation = multi(CurrentLoArOrientation, UpArB6);

	}








	//	/*
	//* LeftArmAngles()
	//*	@This Function Calculates and updates the important angles of the arm
	//*	@param float [,] UpArOrientation, current orientation of the upper arm
	//**@param float [,] LoArOrientation, current orientation of the lower arm
	//	**@param float [,] TorsoOrientation, current orientation of Torso
	//*	@return void
	//*/


	public void LeftArmAngles(float [,] UpArOrientation , float [,] LoArOrientation, float[,] TorsoOrientation)

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


		//////////////// calculate the Elbow Flection angle ////////////////////////////////////////

		/// step 1///
		Vector3 vAxis1 = new Vector3(UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		Vector3 vAxis2 = new Vector3(LoArOrientation[0,2], LoArOrientation[1,2], LoArOrientation [2,2]);
		float vAngleLeftElbowFlexionNew = Vector3.Angle (vAxis1, vAxis2);
		float vAngularVelocityLeftElbowFlexionNew = (vAngleLeftElbowFlexionNew - vAngleLeftElbowFlexion) / vTimeDifference;


		/// step 2///
		vAngularAccelerationLeftElbowFlexion = (vAngularVelocityLeftElbowFlexionNew - vAngularVelocityLeftElbowFlexion) / vTimeDifference;
		vAngularVelocityLeftElbowFlexion = vAngularVelocityLeftElbowFlexionNew;
		vAngleLeftElbowFlexion = vAngleLeftElbowFlexionNew;


		//Debug.Log ("Elbow Flection Angles" + vAngleLeftElbowFlexion + ", and, " + vAngularVelocityLeftElbowFlexion + ", and, " + vAngularAccelerationLeftElbowFlexion);


		//////////////// calculate the Elbow Pronation angle ////////////////////////////////////////

		/// step 1///
		vAxis1.Set(UpArOrientation[0,1], UpArOrientation[1,1], UpArOrientation [2,1]);
		vAxis2.Set(LoArOrientation[0,1], LoArOrientation[1,1], LoArOrientation [2,1]);
		Vector3 vAxis3 = new Vector3(UpArOrientation[0,0], UpArOrientation[1,0], UpArOrientation [2,0]);
		float vAngleLeftElbowPronationNew = Vector3.Angle (vAxis1, vAxis2);
		float vAngularVelocityLeftElbowPronationNew = (vAngleLeftElbowPronationNew - Mathf.Abs(vAngleLeftElbowPronation)) / vTimeDifference;

		/// step 2///
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

		/// step 1///
		vAxis1.Set (-UpArOrientation[0,2], -UpArOrientation[1,2], -UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		float vAngleLeftShoulderFlexionNew = 180 - Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftShoulderFlexionNew = (vAngleLeftShoulderFlexionNew - Mathf.Abs(vAngleLeftShoulderFlexion)) / vTimeDifference;




		/// step 2///
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

		/// step 1///
		vAxis1.Set (UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		float vAngleLeftShoulderAbductionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftShoulderAbductionNew = (vAngleLeftShoulderAbductionNew - Mathf.Abs(vAngleLeftShoulderAbduction)) / vTimeDifference;


		/// step 2///
		if (Vector3.Dot (vAxis1 , vAxis3) < 0)
		{
			vAngleLeftShoulderAbductionNew = -vAngleLeftShoulderAbductionNew;
			vAngularVelocityLeftShoulderAbductionNew = -vAngularVelocityLeftShoulderAbductionNew;
		}

		vAngularAccelerationLeftShoulderAbduction = (vAngularVelocityLeftShoulderAbductionNew - vAngularVelocityLeftShoulderAbduction) / vTimeDifference;
		vAngularVelocityLeftShoulderAbduction = vAngularVelocityLeftShoulderAbductionNew;
		vAngleLeftShoulderAbduction = vAngleLeftShoulderAbductionNew;

		//Debug.Log ("Left Shoulder Abduction Angles" + vAngleLeftShoulderAbduction + ", and, " + vAngularVelocityLeftShoulderAbduction + ", and, " + vAngularAccelerationLeftShoulderAbduction);

		//////////////// calculate the Left Shoulder Rotation angle ////////////////////////////////////////



		/// step 1///
		vAxis1.Set (UpArOrientation[0,2], UpArOrientation[1,2], UpArOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis3 = Vector3.Cross (vAxis2, vAxis1); 
		vAxis3.Normalize();
		vAxis2.Set (UpArOrientation[0,0], UpArOrientation[1,0], UpArOrientation [2,0]);
		vAxis1.Set (UpArOrientation[0,1], UpArOrientation[1,1], UpArOrientation [2,1]);
		float vAngleLeftShoulderRotationNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftShoulderRotationNew = (vAngleLeftShoulderRotationNew - Mathf.Abs(vAngleLeftShoulderRotation)) / vTimeDifference;

		/// step 2///
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







	/// <summary>
	// To Show appropriate Angles in each joint
	/// <summary>
	public override void OnGUIAngles()
	{
        float sideV = 20f;
        float buttonWidth = 200f;
        float buttonHeight = 50f;
        var boxEmpty = new Texture2D(0, 0);
        GUIStyle vRecStyle3 = new GUIStyle(GUI.skin.label);
        vRecStyle3.normal.textColor = Color.white;

        if (NodContainer.vKey == 3)
		{
            GUI.Box(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 5, buttonWidth * 5f, buttonHeight * 2.5f), boxEmpty);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 5, buttonWidth * 2f, buttonHeight * 0.5f), "Left Shoulder Flexion/Extension: " + vAngleLeftShoulderFlexion, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 4, buttonWidth * 2f, buttonHeight * 0.5f), "Left Shoulder Abduction/Adduction: " + vAngleLeftShoulderAbduction, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 3, buttonWidth * 2f, buttonHeight * 0.5f), "Left Shoulder Internal/External Rotation: " + vAngleLeftShoulderRotation, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 2, buttonWidth * 2f, buttonHeight * 0.5f), "Elbow Flexion/Extension: " + vAngleLeftElbowFlexion, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f , buttonWidth * 2f, buttonHeight * 0.5f), "Forearm Supination/Pronation: " + vAngleLeftElbowPronation, vRecStyle3);
		}
	}




	public override void UpdateJoint () 
	{
		for (int ndx = 0; ndx < mNodSensors.Length; ndx++)
		{
			mNodSensors [ndx].UpdateSensor();
		}
			
		Vector3 vNodRawEuler1 = mNodSensors[0].curRotationRawEuler;
		Vector3 vNodRawEuler2 = mNodSensors[1].curRotationRawEuler;
		vNodRawEuler1 = new Vector3(vNodRawEuler1.x , vNodRawEuler1.y , vNodRawEuler1.z );
		vNodRawEuler2 = new Vector3(vNodRawEuler2.x , vNodRawEuler2.y , vNodRawEuler2.z );

		Vector3 NodIniEuler1 = mNodSensors[0].initRotationEuler;
		Vector3 NodIniEuler2 = mNodSensors[1].initRotationEuler;
		NodIniEuler1 = new Vector3(NodIniEuler1.x , NodIniEuler1.y , NodIniEuler1.z );
		NodIniEuler2 = new Vector3(NodIniEuler2.x , NodIniEuler2.y , NodIniEuler2.z );

        vNodRawAcc0 = mNodSensors[0].currentAcceleration;
        vNodRawAcc1 = mNodSensors[1].currentAcceleration;
        vNodInitAcc0 = mNodSensors[0].initAcceleration;
        vNodInitAcc1 = mNodSensors[1].initAcceleration;

        // getting the current Torso orientation for shoulder angle extraction
        float[,] vaTorsoOrientation = new float[3, 3];
		vaTorsoOrientation =  NodContainer.GetTorsoOrientation();   
				
		// call ArmOrientation function to calculate current orientation of the Torso joint
		ArmOrientation (vNodRawEuler1, NodIniEuler1 , vNodRawEuler2 ,  NodIniEuler2);

		// call LeftArmAngles function to calculate the angles of the left arm
		LeftArmAngles(UpArOrientation ,LoArOrientation,vaTorsoOrientation);

		// convert upper arm and forearm orientation from 3*3 matrix to quaternion
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
