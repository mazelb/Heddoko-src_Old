using UnityEngine;
using System;
using System.Collections;
using Nod;
using System.IO;

///// joint code for the Left leg
public class NodJointLegLeft : NodJoint 
{

	// current Hips and Knee joints orientation, 
	public static float [,] HipOrientation = new float[3,3];
	public static float [,] KneeOrientation = new float[3,3];

	// Left leg angles, angular velocities and angular accelerations, The names are chose based on the human body angles document
	public float vAngleKneeFlexion = 0;
	public float vAngularVelocityKneeFlexion = 0;
	public float vAngularAccelerationKneeFlexion = 0;

	public float vAngleKneeRotation = 0;
	public float vAngularVelocityKneeRotation = 0;
	public float vAngularAccelerationKneeRotation = 0;

	public float vAngleLeftHipFlexion = 0;
	public float vAngularVelocityLeftHipFlexion = 0;
	public float vAngularAccelerationLeftHipFlexion = 0;


	public float vAngleLeftHipAbduction = 0;
	public float vAngularVelocityLeftHipAbduction = 0;
	public float vAngularAccelerationLeftHipAbduction = 0;

	public static float vAngleLeftHipRotation = 0;
	public float vAngularVelocityLeftHipRotation = 0;
	public float vAngularAccelerationLeftHipRotation = 0;


	// This variable stores the time of current frame. It is used for angular velocity and acceleration extraction
	public float vTime = 0;

    public Vector3 vNodRawAcc0;
    public Vector3 vNodRawAcc1;
    public Vector3 vNodInitAcc1;
    public Vector3 vNodInitAcc0;
    public Vector3 vNodRawEuler0;
    public Vector3 vNodRawEuler1;
    /**
//	* GravityRef()
//	*	@This Function makes gravity as a reference axis for tracking purposes
//	*	@param matrix B2 and vNodInitAcc
//	*	@returns 3*3 matrix B4 ,
//	*/
    public float[,] GravityRefLeg(float[,] B2, Vector3 vNodInitAcc)
    {
        float[,] B3 = new float[3, 3];
        float[,] B4 = new float[3, 3];
        
        Vector3 u1 = new Vector3(B2[0, 1], B2[1, 1], B2[2, 1]);
        Vector3 v1 = multiMatrixVector(B2, vNodInitAcc);
        float vAngleNew1 = Vector3.Angle(u1, v1);
        Vector3 Ncross1 = Vector3.Cross(u1, v1).normalized;
        float[,] CurrentOrientation = new float[3, 3];
        CurrentOrientation = RVector(Ncross1, (float)Math.PI * (vAngleNew1 / 180f));

        B3 = multi(CurrentOrientation, B2);

        ////cross product of initial gravity vector and yaw 
        Vector3 yaw = new Vector3(0, 1, 0);
        Vector3 Ncross = Vector3.Cross(vNodInitAcc, yaw).normalized;
        float CompensationAngle = Vector3.Angle(vNodInitAcc, yaw);

        float[,] CompensationRotation = new float[3, 3];
        CompensationRotation = RVector(Ncross, (float)Math.PI * (CompensationAngle / 180f));

//      print ("vAngleNew1=" + vAngleNew1+ "CompensationAngle= "+ CompensationAngle +" vNodInitAcc= " + vNodInitAcc+" Ncross =" + Ncross + " CompensationAngle =" + CompensationAngle);
        B4 = multi(CompensationRotation, B3);

        if (vNodInitAcc.x == 0 && vNodInitAcc.y == 0 && vNodInitAcc.z == 0)
        {
            B4 = B2;
        }

        return B4;
    }

    /**
* HipOrientation()
*	@This Function Provides The Final compensated Update for the Hip Orientation
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
		//Intermediate arrays until achieve final orientation for hip and knee, they are Tagged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

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

        float[,] KneeB21 = new float[3,3];
        float[,] KneeB22 = new float[3,3];
        float[,] HipB21  = new float[3,3];
        float[,] HipB22  = new float[3,3];
      
        float [,] CompensationRotationKnee = new float[3,3];
		float [,] CompensationRotationHip = new float[3,3];
		float [,] CurrentKneeOrientation = new float[3,3];

		float OrientationError = 0;
		float CompensationAngle = 0;
	

		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////
		/// //////////////////////////////////////////////////  tracking /////////////////////////////////////////////////////////////////////


		/////////// Converting to Orientation matrix ///////////////////

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

        /////////////// Reference based on gravity //////////////////
        //HipB21 = multi(HipFi, HipB1);
        //KneeB21 = multi(KneeFi, KneeB1);
        //HipB2 = GravityRefLeg(HipB21, vNodInitAcc0);
        //KneeB2 = GravityRefLeg(KneeB21, vNodInitAcc1);




        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////  Fusion /////////////////////////////////////////////////////////////////////


        if (NodContainer.Fusion)
        {

            ///////////// Pitch Compensation Step ///////////////////
            Vector3 pitchHip = new Vector3(HipB2[0, 2], HipB2[1, 2], HipB2[2, 2]);
            Vector3 pitchKnee = new Vector3(KneeB2[0, 2], KneeB2[1, 2], KneeB2[2, 2]);
            Vector3 NcrossHipKnee = new Vector3(0, 0, 0);


            // rotation axis for pitch compensation
            NcrossHipKnee = Vector3.Cross(pitchHip, pitchKnee).normalized;
            OrientationError = HipB2[0, 2] * KneeB2[0, 2] + HipB2[1, 2] * KneeB2[1, 2] + HipB2[2, 2] * KneeB2[2, 2];

            // Finding Pitch compensation Angle
            CompensationAngle = (float)Math.Acos(OrientationError > 1.00f ? 1f : OrientationError);

            // Building Pitch compensation rotation matrices
            CompensationRotationHip = RVector(NcrossHipKnee, -0.5f * CompensationAngle);
            CompensationRotationKnee = RVector(NcrossHipKnee, +0.5f * CompensationAngle);

            // Applying Pitch Compensation 
            KneeB3 = multi(CompensationRotationHip, KneeB2);
            HipB3 = multi(CompensationRotationKnee, HipB2);



            // this step applies the knee 180 constraint and can be used also for fusion of knee stretch sensors and nods in future
            ///////////// Knee 180 degree Constraint ///////////////////

            Vector3 RollHip = new Vector3(HipB3[0, 0], HipB3[1, 0], HipB3[2, 0]);
            Vector3 YawKnee = new Vector3(KneeB3[0, 1], KneeB3[1, 1], KneeB3[2, 1]);
            Vector3 YawHip = new Vector3(HipB3[0, 1], HipB3[1, 1], HipB3[2, 1]);


            Vector3 NcrossHipKneeRoll = Vector3.Cross(YawHip, YawKnee).normalized;
            if (Vector3.Dot(RollHip, YawKnee) > 0) /// this case when not obey 180 degree constraint
            {

                OrientationError = HipB3[0, 1] * KneeB3[0, 1] + HipB3[1, 1] * KneeB3[1, 1] + HipB3[2, 1] * KneeB3[2, 1];


                CompensationAngle = (float)Math.Acos(OrientationError > 1.00f ? 1f : OrientationError);

                // Building yaw compensation rotation matrices
                CompensationRotationHip = RVector(NcrossHipKneeRoll, +0.5f * CompensationAngle);
                CompensationRotationKnee = RVector(NcrossHipKneeRoll, -0.5f * CompensationAngle);

                // Applying yaw Compensation 
                KneeB4 = multi(CompensationRotationKnee, KneeB3);
                HipB4 = multi(CompensationRotationHip, HipB3);

            }
            else  /// this case when obey 180 degree constraint just to improve knee angle estimation
            {

                OrientationError = HipB3[0, 1] * KneeB3[0, 1] + HipB3[1, 1] * KneeB3[1, 1] + HipB3[2, 1] * KneeB3[2, 1];

                // Finding Pitch compensation Angle
                // stretch sensor should be added
                CompensationAngle = 0;


                // Building yaw compensation rotation matrices
                CompensationRotationHip = RVector(NcrossHipKneeRoll, +0.5f * CompensationAngle);
                CompensationRotationKnee = RVector(NcrossHipKneeRoll, -0.5f * CompensationAngle);

                // Applying yaw Compensation 
                KneeB4 = multi(CompensationRotationKnee, KneeB3);
                HipB4 = multi(CompensationRotationHip, HipB3);

            }
        }
        else
        {
            KneeB4 = KneeB2;
            HipB4 = HipB2;
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
		//CurrentKneeOrientation= RVector(u, (float)3.1415 );
	CurrentKneeOrientation= RVector(u, (float)0.0 );
		HipB5 = multi(CurrentKneeOrientation, HipB4);



		u.Set(HipB5[0,2] , HipB5[1,2] , HipB5[2,2]);
		CurrentKneeOrientation= RVector(u, (float)3.1415 );

		HipB6 = multi(CurrentKneeOrientation, HipB5);

		u.Set(0 , 0 , 1);
		CurrentKneeOrientation = RVector(u, (float)3.1415 );

		HipB7 = multi(CurrentKneeOrientation, HipB6);

		u.Set(0 , 1 , 0);
		//CurrentKneeOrientation = RVector(u, (float)3.1415 );
		CurrentKneeOrientation = RVector(u, (float)0.0 );
		HipOrientation = multi(CurrentKneeOrientation, HipB7);

		////////////////// setting Knee to Final Body orientation ///////////////////////////////

		Vector3 u2 = new Vector3( KneeB4[0,1] , KneeB4[1,1] , KneeB4[2,1]);
		CurrentKneeOrientation= RVector(u2, (float)0.0 );
		//		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeB5 = multi(CurrentKneeOrientation, KneeB4);



		u2.Set(KneeB5[0,2] , KneeB5[1,2] , KneeB5[2,2]);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeB6 = multi(CurrentKneeOrientation, KneeB5);

		u2.Set(0 , 0 , 1);
		CurrentKneeOrientation= RVector(u2, (float)3.1415 );

		KneeB7 = multi(CurrentKneeOrientation, KneeB6);

		u2.Set(0 , 1 , 0);
		CurrentKneeOrientation= RVector(u2, (float)0.0 );
//		CurrentKneeOrientation= RVector(u2, (float)3.1415 );
		KneeOrientation = multi(CurrentKneeOrientation, KneeB7);

//		vLegHeight = KneeOrientation [1, 1] + HipOrientation [1, 1];
//		Debug.Log ("Left Knee altitude" + vLegHeight);

	}

	/**
* LeftLegMovement()
*	@This Function Anchors feet to the ground (enables sitting and squats)- will be called in NodJointTorso
*	@param float vUpperLeftLegLength, Upper Leg Length
*	@param float vLowerLeftLegLength, Lower Leg Length
*	@return vLeftLegHeight : Left leg vertical movement 
*/	
public static float LeftLegMovement (float vUpperLegLength,float vLowerLegLength) 
	{

		float vLeftLegHeight = KneeOrientation [1, 1]*0.5f*vLowerLegLength + HipOrientation [1, 1]*0.5f*vUpperLegLength;
		return vLeftLegHeight;
//		Debug.Log ("Left Knee altitude" + vLegHeight);
	}
	
	
	
	/************
* LegAngles()
*	@This Function Calculates and updates the important angles of the leg
*	@param float [,] HipOrientation, current orientation of the hip
*	@param float [,] KneeOrientation, Final orientation of the knee
*	@param float [,] torsoOrientation, Final orientation of the Torso
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
		Vector3 vAxis3 = new Vector3(HipOrientation[0,0], HipOrientation[1,0], HipOrientation [2,0]);
		float vAngleKneeRotationNew = Vector3.Angle (vAxis1, vAxis2);

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


		//////////////// calculate the Left Hip Flection angle ////////////////////////////////////////

		/// step1 ///
		vAxis1.Set (HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		float vAngleLeftHipFlexionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftHipFlexionNew = (vAngleLeftHipFlexionNew - Mathf.Abs(vAngleLeftHipFlexion)) / vTimeDifference;

		/// step1 ///
		if (Vector3.Dot (vAxis1 , vAxis3) > 0)
		{
			vAngleLeftHipFlexionNew = -vAngleLeftHipFlexionNew;
			vAngularVelocityLeftHipFlexionNew = -vAngularVelocityLeftHipFlexionNew;
		}

		vAngularAccelerationLeftHipFlexion = (vAngularVelocityLeftHipFlexionNew - vAngularVelocityLeftHipFlexion) / vTimeDifference;
		vAngularVelocityLeftHipFlexion = vAngularVelocityLeftHipFlexionNew;
		vAngleLeftHipFlexion = vAngleLeftHipFlexionNew;

		//Debug.Log ("EHip Flexion Angles" + vAngleLeftHipFlexionNew + ", and, " + vAngularVelocityLeftHipFlexion + ", and, " + vAngularAccelerationLeftHipFlexion);



		//////////////// calculate the Left Hip Abduction angle ////////////////////////////////////////

		/// step1 ///
		vAxis1.Set (HipOrientation[0,1], HipOrientation[1,1], HipOrientation [2,1]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis1.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		float vAngleLeftHipAbductionNew = Vector3.Angle (vAxis3, vAxis2);
		float vAngularVelocityLeftHipAbductionNew = (vAngleLeftHipAbductionNew - Mathf.Abs(vAngleLeftHipAbduction)) / vTimeDifference;


		/// step1 ///
		if (Vector3.Dot (vAxis1 , vAxis3) < 0)
		{
			vAngleLeftHipAbductionNew = -vAngleLeftHipAbductionNew;
			vAngularVelocityLeftHipAbductionNew = -vAngularVelocityLeftHipAbductionNew;
		}

		vAngularAccelerationLeftHipAbduction = (vAngularVelocityLeftHipAbductionNew - vAngularVelocityLeftHipAbduction) / vTimeDifference;
		vAngularVelocityLeftHipAbduction = vAngularVelocityLeftHipAbductionNew;
		vAngleLeftHipAbduction = vAngleLeftHipAbductionNew;

		//Debug.Log ("Hip Abduction Angles" + vAngleLeftHipAbductionNew + ", and, " + vAngularVelocityLeftHipAbduction + ", and, " + vAngularAccelerationLeftHipAbduction);



		//////////////// calculate the Left Hip Rotation angle ////////////////////////////////////////

		/// step1 ///
		vAxis1.Set (HipOrientation[0,2], HipOrientation[1,2], HipOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,1], TorsoOrientation[1,1], TorsoOrientation [2,1]);
		vAxis3 = vAxis1 - (Vector3.Dot (vAxis1, vAxis2)) * vAxis2;
		vAxis3.Normalize();
		vAxis1.Set (TorsoOrientation[0,2], TorsoOrientation[1,2], TorsoOrientation [2,2]);
		vAxis2.Set (TorsoOrientation[0,0], TorsoOrientation[1,0], TorsoOrientation [2,0]);
		float vAngleLeftHipRotationNew = Vector3.Angle (vAxis3, vAxis1);
		float vAngularVelocityLeftHipRotationNew = (vAngleLeftHipRotationNew - Mathf.Abs(vAngleLeftHipRotation)) / vTimeDifference;

		/// step2 ///
		if (Vector3.Dot (vAxis2 , vAxis3) > 0)
		{
			vAngleLeftHipRotationNew = -vAngleLeftHipRotationNew;
			vAngularVelocityLeftHipRotationNew = -vAngularVelocityLeftHipRotationNew;
		}

		vAngularAccelerationLeftHipRotation = (vAngularVelocityLeftHipRotationNew - vAngularVelocityLeftHipRotation) / vTimeDifference;
		vAngularVelocityLeftHipRotation = vAngularVelocityLeftHipRotationNew;
		vAngleLeftHipRotation = vAngleLeftHipRotationNew;

		//Debug.Log ("Hip Rotation Angles" + vAngleLeftHipRotationNew + ", and, " + vAngularVelocityLeftHipRotation + ", and, " + vAngularAccelerationLeftHipRotation);


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

        if (NodContainer.vKey == 5)
        {
            GUI.Box(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 5, buttonWidth * 5f, buttonHeight * 2.5f), boxEmpty);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 5, buttonWidth * 2f, buttonHeight * 0.5f), "Left Hip Flexion/Extension: " + vAngleLeftHipFlexion, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 4, buttonWidth * 2f, buttonHeight * 0.5f), "Left Hip Abduction/Adduction: " + vAngleLeftHipAbduction, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 3, buttonWidth * 2f, buttonHeight * 0.5f), "Left Hip Internal/External Rotation: " + vAngleLeftHipRotation, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f * 2, buttonWidth * 2f, buttonHeight * 0.5f), "Knee Flexion/Extension: " + vAngleKneeFlexion, vRecStyle3);
            GUI.Label(new Rect(Screen.width - sideV - 5 * buttonWidth, Screen.height * 0.9f - buttonHeight * 0.5f, buttonWidth * 2f, buttonHeight * 0.5f), "Tibial Internal/External Rotation: " + vAngleKneeRotation, vRecStyle3);
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

         vNodRawAcc0 = mNodSensors[0].currentAcceleration;
         vNodRawAcc1 = mNodSensors[1].currentAcceleration;
         vNodInitAcc0= mNodSensors[0].initAcceleration;
         vNodInitAcc1= mNodSensors[1].initAcceleration; 
        

    // getting the current Torso orientation for Hip angle extraction
        float[,] vaTorsoOrientation = new float[3, 3];
		vaTorsoOrientation =  NodContainer.GetTorsoOrientation();

		// call LegOrientation function to calculate current orientation of leg joints
		LegOrientation (vNodRawEuler1, NodIniEuler1 , vNodRawEuler2 ,  NodIniEuler2);

		// call LegAngles function to update leg angles
		LegAngles(HipOrientation , KneeOrientation, vaTorsoOrientation);

		// convert leg orientations from 3*3 matrix to quaternion orientation
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
