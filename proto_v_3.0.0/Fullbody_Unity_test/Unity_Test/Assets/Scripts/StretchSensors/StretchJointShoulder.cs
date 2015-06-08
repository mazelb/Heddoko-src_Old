using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJointShoulder : StretchJoint
{
	//
	// Gets the axial angle in degrees (starting from a T-pose).
	//
	private float getAxialAngle(StretchSensor axilla, StretchSensor clavicle, StretchSensor deltoid, StretchSensor trapezius)
	{
		int mapTo = 1000;
		float ssAxilla = axilla.getMappedReading(mapTo);
		float ssClavicle = clavicle.getMappedReading(mapTo);
		float ssDeltoid = deltoid.getMappedReading(mapTo);
		float ssTrapezius = trapezius.getMappedReading(mapTo);

		// Correction factors
		float axillaCorrection = 0.05f * ssAxilla / mapTo;
		float deltoidCorrection = 0.05f * ssDeltoid / mapTo;

		// Axial angles
		float anteriorAngle = (ssTrapezius / mapTo) * 90 - axillaCorrection - deltoidCorrection;
		float posteriorAngle = -1 * (ssClavicle / mapTo) * 90 + axillaCorrection + deltoidCorrection;
		
		return anteriorAngle + posteriorAngle;
	}
	
	//
	// Gets the frontal angle in degrees (starting from a T-pose).
	//
	private float getFrontalAngle(StretchSensor axilla, StretchSensor clavicle, StretchSensor deltoid, StretchSensor trapezius)
	{
		int mapTo = 1000;
		float ssAxilla = axilla.getMappedReading(mapTo);
		float ssClavicle = clavicle.getMappedReading(mapTo);
		float ssDeltoid = deltoid.getMappedReading(mapTo);
		float ssTrapezius = trapezius.getMappedReading(mapTo);
		
		// Correction factors
		float clavicleCorrection = 0.05f * ssClavicle / mapTo;
		float trapeziusCorrection = 0.05f * ssTrapezius / mapTo;
		
		// Axial angles
		float superiorAngle = (ssAxilla / mapTo) * 90 - clavicleCorrection - trapeziusCorrection;
		float inferiorAngle = -1 * (ssDeltoid / mapTo) * 90 + clavicleCorrection + trapeziusCorrection;
		
		return superiorAngle + inferiorAngle;
	}
	
	//
	// Gets the rotation angle in degrees (starting from a T-pose).
	//
	private float getRotAngle(StretchSensor axilla, StretchSensor clavicle, StretchSensor deltoid, StretchSensor trapezius)
	{
		float angle = 0.0f;
		
		return angle;
	}

	//
	// Updates joint position and values.
	//
	public override void UpdateJoint()
	{
		// Don't update anything if joint is paused.
		if (pauseAnimation) {
			return;
		}
		
		// Update individual sensors.
		for (int ndx = 0; ndx < mStretchSensors.Length; ndx++) {
			mStretchSensors [ndx].UpdateSensor ();
		}
		
		// Retrieve sensor objects.
		StretchSensor axilla = getSensorByPosition (StretchSensor.positionName.Axilla);
		StretchSensor clavicle = getSensorByPosition (StretchSensor.positionName.Clavicle);
		StretchSensor deltoid = getSensorByPosition (StretchSensor.positionName.Deltoid);
		StretchSensor trapezius = getSensorByPosition (StretchSensor.positionName.Trapezius);
		
		// Update shoulder orientation.
		if (axilla != null && clavicle != null && deltoid != null && trapezius != null) {
			float axialAngle = getAxialAngle (axilla, clavicle, deltoid, trapezius);
			float frontalAngle = getFrontalAngle (axilla, clavicle, deltoid, trapezius);
			float rotAngle = getRotAngle (axilla, clavicle, deltoid, trapezius);
			
			mCurJointRotationEuler.y = axialAngle * rotatesXYZ.y;
			mCurJointRotationEuler.z = frontalAngle * rotatesXYZ.z;
			mCurJointRotationEuler.x = rotAngle * rotatesXYZ.x;
			
			jointTransform.localRotation = Quaternion.Euler (mCurJointRotationEuler);
		} else {
			print ("??");
		}
	}







	/* 
	 * 
	 * ORIGINAL SHOULDER CODE
	 * 
	 */

/*
	private void UpdateJointBAK()
	{
		StretchSensor vShoulderSensor = getSensorByName ("ShoulderStretchSensor");
		StretchSensor vBackSensor = getSensorByName ("BackStretchSensor");
		StretchSensor vFrontSensor = getSensorByName ("FrontStretchSensor");
		StretchSensor vBodySensor = getSensorByName ("BodyStretchSensor");
		
		if (vShoulderSensor != null && 
			vBackSensor != null && 
			vFrontSensor != null && 
			vBodySensor != null) {


			float mAngleI = 0.0f;
			float mAngleS = 0.0f; 
			float mAngleP = 0.0f; 
			float mAngleA = 0.0f;
			
			mAngleI = (factorsABCi.x * (vShoulderSensor.getCurReading () * 90.0f / vShoulderSensor.maxStretchVal)) +
				(factorsABCi.y * vBackSensor.getCurReading () / vBackSensor.maxStretchVal) +
				(factorsABCi.z * vFrontSensor.getCurReading () / vFrontSensor.maxStretchVal);
			
			mAngleS = (factorsABCs.x * (vBodySensor.getCurReading () * 90.0f / vBodySensor.maxStretchVal)) +
				(factorsABCs.y * vBackSensor.getCurReading () / vBackSensor.maxStretchVal) +
				(factorsABCs.z * vFrontSensor.getCurReading () / vFrontSensor.maxStretchVal);
			
			mAngleP = (factorsABCp.x * (vFrontSensor.getCurReading () * 90.0f / vFrontSensor.maxStretchVal)) +
				(factorsABCp.y * vBodySensor.getCurReading () / vBodySensor.maxStretchVal) +
				(factorsABCp.z * vShoulderSensor.getCurReading () / vShoulderSensor.maxStretchVal);
			
			mAngleA = (factorsABCa.x * (vBackSensor.getCurReading () * 90.0f / vShoulderSensor.maxStretchVal)) +
				(factorsABCa.y * vBodySensor.getCurReading () / vBodySensor.maxStretchVal) +
				(factorsABCa.z * vShoulderSensor.getCurReading () / vShoulderSensor.maxStretchVal);
			
			//TODO: apply angles to the joint transformation
			
		}
	}

*/
}

