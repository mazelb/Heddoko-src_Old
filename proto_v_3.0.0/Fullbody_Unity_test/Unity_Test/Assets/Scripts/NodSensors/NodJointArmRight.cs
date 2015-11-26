using UnityEngine;
using System;
using System.Collections;
using Nod;
using System.IO;

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

	// This variable stores the time of current frame. It is used for angular velocity and acceleration extraction
	public float vTime = 0;
    public Vector3 vNodRawAcc0;
    public Vector3 vNodRawAcc1;
    public Vector3 vNodInitAcc1;
    public Vector3 vNodInitAcc0;
    public Vector3 vNodRawEuler0;
    public Vector3 vNodRawEuler1;
    //************** get stretch sensor data**********************

    public float vStrechSenseData;
	public float vStrechSenseDataOld;

	public float tetaMaxElbow=141;
	public float tetaminElbow=0;
	//For recording
	public float vRawSSData;

	public float vStrechSenseDataTimeConst;
	public float vStrechSenseDataTimeConstOld;
	
	public float vAngleRightElbowFlexionSS;
	public float vAngleRightElbowFlexionSSLinear;
	public float vAngleRightElbowFlexionNod;
	public float vOptimumAngle;
	public float vOptimumAngle2;
	float CompAnle;



    //***********************************************************

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
        float[,] UpArB21 = new float[3, 3];
        float[,] UpArB22 = new float[3, 3];

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
        float[,] LoArB21 = new float[3, 3];
        float[,] LoArB22 = new float[3, 3];


        float [,] CompensationRotationLoAr = new float[3,3];
		float [,] CompensationRotationUpAr = new float[3,3];
		float [,] CurrentLoArOrientation =   new float[3,3];

		float OrientationError = 0;
		float CompensationAngle = 0;


		Vector3 pitchUpAr = new Vector3(0,0,0);
		Vector3  pitchLoAr = new Vector3(0,0,0);


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

        UpArB21 = multi(UpArFi, UpArB1);
		LoArB21 = multi(LoArFi, LoArB1);

        UpArB22 = TiltNod(UpArB21);
        LoArB22 = TiltNod(LoArB21);

        UpArB2 = GravityRefArm(UpArB22, vNodInitAcc0);
        LoArB2 = GravityRefArm(LoArB22, vNodInitAcc1);


        if (NodContainer.Fusion)
        {
            ///////////// Yaw Compensation Step ///////////////////constraint


            // finding compensated yaw axis of lower arm in one plane with upper arm
            float temp1, temp2, temp3, temp4;
            temp1 = LoArB2[0, 1] - (UpArB2[0, 2] * LoArB2[0, 1] + UpArB2[1, 2] * LoArB2[1, 1] + UpArB2[2, 2] * LoArB2[2, 1]) * UpArB2[0, 2];
            temp2 = LoArB2[1, 1] - (UpArB2[0, 2] * LoArB2[0, 1] + UpArB2[1, 2] * LoArB2[1, 1] + UpArB2[2, 2] * LoArB2[2, 1]) * UpArB2[1, 2];
            temp3 = LoArB2[2, 1] - (UpArB2[0, 2] * LoArB2[0, 1] + UpArB2[1, 2] * LoArB2[1, 1] + UpArB2[2, 2] * LoArB2[2, 1]) * UpArB2[2, 2];
            temp4 = (float)Math.Sqrt((float)(temp1 * temp1 + temp2 * temp2 + temp3 * temp3));
            Vector3 yawLoAr = new Vector3(temp1 / temp4, temp2 / temp4, temp3 / temp4);

            // Finding yaw compensation Angle
            OrientationError = yawLoAr.x * LoArB2[0, 1] + yawLoAr.y * LoArB2[1, 1] + yawLoAr.z * LoArB2[2, 1];
            CompensationAngle = (float)Math.Acos(OrientationError > 1.00f ? 1f : OrientationError);
            //print("CompensationAngle"+CompensationAngle);
            //CompensationAngle = 0;
            // Finding yaw compensation axis
            Vector3 yawLoAr2 = new Vector3(LoArB2[0, 1], LoArB2[1, 1], LoArB2[2, 1]);

            Vector3 NcrossUpArLoAr = new Vector3(0, 0, 0);
            NcrossUpArLoAr = Vector3.Cross(yawLoAr2, yawLoAr).normalized;

            CompensationRotationUpAr = RVector(NcrossUpArLoAr, CompensationAngle);
            CompensationRotationLoAr = RVector(NcrossUpArLoAr, -CompensationAngle);

            // Applying yaw Compensation 
            LoArB3 = multi(CompensationRotationUpAr, LoArB2);

            // to remove the constraint un comment this line
            //		LoArB3=LoArB2;

            // this step applies the elbow 180 constraint and can be used also for fusion of elbow stretch sensors and nods in future
            ///////////// Elbow 180 degree Constraint ///////////////////

            Vector3 RollUpAr = new Vector3(UpArB2[0, 0], UpArB2[1, 0], UpArB2[2, 0]);
            yawLoAr = new Vector3(LoArB3[0, 1], LoArB3[1, 1], LoArB3[2, 1]);
            yawLoAr2 = new Vector3(UpArB2[0, 1], UpArB2[1, 1], UpArB2[2, 1]);

            /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
            /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
            /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
            /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
            /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
            /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////		


            // Read stretch sensor data  for the right arm from channel 1
            vStrechSenseData = NodContainer.svaModuleData[1]; // for now all is 1 I have 1 stretch sensor 
            vRawSSData = vStrechSenseData;
            // print (vStrechSenseData+"   vStrechSenseData"+"   vStrechSenseDataOld   "+vStrechSenseDataOld);
            vStrechSenseDataTimeConst = NodContainer.svaModuleData[1];
            // call filtering algorithm from NodJoint
            vStrechSenseData = SSFiltering(vStrechSenseData, vStrechSenseDataOld);
            // print (vStrechSenseData + "   vStrechSenseDataFilter");
            vStrechSenseDataTimeConst = SSFilteringTimeConst(vStrechSenseDataTimeConst, vStrechSenseDataTimeConstOld);

            // Saving stretch sensor data for the next frame
            vStrechSenseDataOld = vStrechSenseData;
            vStrechSenseDataTimeConstOld = vStrechSenseDataTimeConst;

            // Call stretch sensor angle algorithm from NodJoint
            vAngleRightElbowFlexionSS = SSAngleMapPoly(vStrechSenseData, tetaMaxElbow, tetaminElbow);
            vAngleRightElbowFlexionSSLinear = SSAngleMapLinear(vStrechSenseData, tetaMaxElbow, tetaminElbow);
            // print (vAngleRightElbowFlexionSS+"  vAngleRightElbowFlexionSS");


            Vector3 NcrossUpArLoArRoll = new Vector3(0, 0, 0);
            NcrossUpArLoArRoll = Vector3.Cross(yawLoAr2, yawLoAr).normalized;
            if (Vector3.Dot(RollUpAr, yawLoAr) > 0) /// this case when not obey 180 degree constraint
            {

                OrientationError = UpArB2[0, 1] * LoArB3[0, 1] + UpArB2[1, 1] * LoArB3[1, 1] + UpArB2[2, 1] * LoArB3[2, 1];

                //
                //			// Finding yaw compensation Angle
                //			if ((float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError) > (3.1415f / 2))
                //			{
                //				CompensationAngle = (float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError)  - 3.1415f;
                //			}
                //			else
                //			{
                //				//Finding yaw compensation Angle
                //				CompensationAngle = (float)Math.Acos(OrientationError> 1.00f ? 1f : OrientationError) ;
                //			}

                //Finding yaw compensation Angle
                CompensationAngle = (float)Math.Acos(OrientationError > 1.00f ? 1f : OrientationError);
                //CompensationAngle = 0;
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
                vAngleRightElbowFlexionNod = (float)Math.Acos(OrientationError > 1.00f ? 1f : OrientationError);
                const float PI = (float)Math.PI;



                // Adding stretch sensor 
                if (vStrechSenseData > 1000 && !float.IsNaN(vAngleRightElbowFlexionNod) && !float.IsNaN(vAngleRightElbowFlexionSS))
                {
                    float vAlpha = vAngleRightElbowFlexionNod * 180 / PI / (tetaMaxElbow - tetaminElbow);
                    vOptimumAngle = (vAlpha * vAngleRightElbowFlexionSS + vAngleRightElbowFlexionNod) / (1 + vAlpha);
                    vOptimumAngle2 = (vAngleRightElbowFlexionSS + vAngleRightElbowFlexionNod) / 2;
                    CompensationAngle = (vAngleRightElbowFlexionNod - vOptimumAngle);
                    CompAnle = CompensationAngle;
                }
                else
                {
                    CompensationAngle = 0;
                }


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
            UpArB3 = UpArB2;
            LoArB4 = LoArB2;
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
// 		CurrentLoArOrientation= RVector(u, (float)3.1415 );
		CurrentLoArOrientation= RVector(u, (float)0.0 );
		LoArB5 = multi(CurrentLoArOrientation, LoArB4);

		u.Set( LoArB5[0,0] , LoArB5[1,0] , LoArB5[2,0]);
		CurrentLoArOrientation= RVector(u, -(float)3.1415/2 );
		LoArB6 = multi(CurrentLoArOrientation, LoArB5);

		u.Set(1 , 0 , 0);
		CurrentLoArOrientation= RVector(u, (float)3.1415 /2 );
		LoArB7 = multi(CurrentLoArOrientation, LoArB6);

		u.Set(0 , 0 , 1);
		// CurrentLoArOrientation= RVector(u, (float)3.1415);-also the constraint 180 degree
		CurrentLoArOrientation= RVector(u, (float)0.0);
		LoArOrientation = multi(CurrentLoArOrientation, LoArB7);
		 

		////////////////// setting to Final Body orientation upper arm///////////////////////////////
		Vector3 u2 = new Vector3(UpArB3[0,1] , UpArB3[1,1] , UpArB3[2,1]);
		// CurrentLoArOrientation= RVector(u2, (float)3.1415 );
		CurrentLoArOrientation= RVector(u2, (float)0.0 );
		UpArB4 = multi(CurrentLoArOrientation, UpArB3);

		u2.Set (UpArB4[0,0] , UpArB4[1,0] , UpArB4[2,0]);
		CurrentLoArOrientation= RVector(u2, - (float)3.1415/2);
		UpArB5 = multi(CurrentLoArOrientation, UpArB4);

		u2.Set(1 , 0 ,0);
		CurrentLoArOrientation= RVector(u2, (float)3.1415/2 );
		UpArB6 = multi(CurrentLoArOrientation, UpArB5);



		u2.Set(0 , 0 ,1);
		// CurrentLoArOrientation= RVector(u2, (float)3.1415 );
		CurrentLoArOrientation= RVector(u2, (float)0.0 );
		UpArOrientation = multi(CurrentLoArOrientation, UpArB6);
        //UpArOrientation=UpArB3;

	}

	//	/*
	//* RightArmAngles()
	//*	@This Function Calculates and updates the important angles of the arm
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

		//Debug.Log ("Shoulder Abduction Angles" + vAngleRightShoulderAbductionNew + ", and, " + vAngularVelocityRightShoulderAbduction + ", and, " + vAngularAccelerationRightShoulderAbduction);

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

	/// <summary>
	// To Show appropriate Angles for each joint in GUI
	/// <summary>
	public override void OnGUIAngles()
	{
        float sideV = 20f;
        float buttonWidth = 200f;
        float buttonHeight = 50f;
        var boxEmpty = new Texture2D(0, 0);
        GUIStyle vRecStyle3 = new GUIStyle(GUI.skin.label);
        vRecStyle3.normal.textColor = Color.white;

        if (NodContainer.vKey == 2)
        {
            GUI.Box(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 5, buttonWidth * 5f, buttonHeight * 2.5f), boxEmpty);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 5, buttonWidth * 2f, buttonHeight * 0.5f), "Right Shoulder Flexion/Extension: " + vAngleRightShoulderFlexion, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 4, buttonWidth * 2f, buttonHeight * 0.5f), "Right Shoulder Abduction/Adduction: " + vAngleRightShoulderAbduction, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 3, buttonWidth * 2f, buttonHeight * 0.5f), "Right Shoulder Internal/External Rotation: " + vAngleRightShoulderRotation, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 2, buttonWidth * 2f, buttonHeight * 0.5f), "Elbow Flexion/Extension: " + vAngleRightElbowFlexion, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f, buttonWidth * 2f, buttonHeight * 0.5f), "Forearm Supination/Pronation: " + vAngleRightElbowPronation, vRecStyle3);
        }
	}


public void wririting()
{
			//Write data of stretch sensor and Nod sensor
			
			TextWriter tw1 = new StreamWriter("RawSSDataArmR.txt",true);
			tw1.WriteLine(vRawSSData);   
			tw1.Close();
			
			TextWriter tw2 = new StreamWriter("ssFilteredArmR.txt",true);
			tw2.WriteLine(vStrechSenseData);   
			tw2.Close();
			
			TextWriter tw3 = new StreamWriter("ssFilteredTimeConstArmR.txt",true);
			tw3.WriteLine(vStrechSenseDataTimeConst);   
			tw3.Close();
			
			TextWriter tw4 = new StreamWriter("ssAngleArmRPoly.txt",true);
			tw4.WriteLine(vAngleRightElbowFlexionSS);   
			tw4.Close();

			TextWriter tw5 = new StreamWriter("ssAngleArmRLinear.txt",true);
			tw5.WriteLine(vAngleRightElbowFlexionSSLinear);   
			tw5.Close();
			
			TextWriter tw6 = new StreamWriter("NodAngleArmR.txt",true);
			tw6.WriteLine(vAngleRightElbowFlexionNod);   
			tw6.Close();

			TextWriter tw7 = new StreamWriter("OptimumAngle1ArmR.txt",true);
			tw7.WriteLine(vOptimumAngle);   
			tw7.Close();
			
			TextWriter tw8 = new StreamWriter("OptimumAngle2ArmR.txt",true);
			tw8.WriteLine(vOptimumAngle2);   
			tw8.Close();
			
			TextWriter tw9 = new StreamWriter("AngleArmR.txt",true);
			tw9.WriteLine(vAngleRightElbowFlexion);   
			tw9.Close();
			
			TextWriter tw10 = new StreamWriter("CompAngleArmR.txt",true);
			tw10.WriteLine(CompAnle);   
			tw10.Close();
}

	/**
	 * Sample method showing how to add a heading for custom data to be recorded.
	 * 
	 * Define the method in the child class as "public override string[] GetRecordedExtrasHeading() { ... }"
	 */
	public override string[] GetRecordedExtrasHeading()
	{
		string[] vExtraHeadings = {"RawSSDataArmR", "ssFilteredArmR","ssFilteredTimeConstArmR", "ssAngleArmRPoly","ssAngleArmRLinear", "NodAngleArmR","OptimumAngle1ArmR", "OptimumAngle2ArmR","AngleArmR", "CompAngleArmR"};

		return vExtraHeadings;
	}
	
	/**
	 * Sample method showing how to add values for custom data to be recorded.
	 * 
	 * Define the method in the child class as "public override float[] GetRecordedExtras() { ... }"
	 */
	public override float[] GetRecordedExtras()
	{
		float[] vExtras = {vRawSSData,vStrechSenseData,vStrechSenseDataTimeConst,vAngleRightElbowFlexionSS,vAngleRightElbowFlexionSSLinear,vAngleRightElbowFlexionNod,vOptimumAngle,vOptimumAngle2,vAngleRightElbowFlexion, CompAnle};

		return vExtras;
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

                vNodRawAcc0 = mNodSensors[0].currentAcceleration;
                vNodRawAcc1 = mNodSensors[1].currentAcceleration;
                vNodInitAcc0 = mNodSensors[0].initAcceleration;
                vNodInitAcc1 = mNodSensors[1].initAcceleration;

                // getting the current Torso orientation for shoulder angle extraction
                float[,] vaTorsoOrientation = new float[3, 3];
				vaTorsoOrientation =  NodContainer.GetTorsoOrientation();     	
				
				// call ArmOrientation function to calculate current orientation of the Shoulder and Elbow
				ArmOrientation (vNodRawEuler1, NodIniEuler1 , vNodRawEuler2 ,  NodIniEuler2);
			
				
				// call RightArmAngles function to update current right arm angles 
				RightArmAngles(UpArOrientation ,LoArOrientation , vaTorsoOrientation );
				
				//wririting();
				
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
