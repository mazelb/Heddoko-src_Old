using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StretchJointShoulder : StretchJoint
{
	public bool applyCorrectionFactors = true;

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

		// Initial calculations.
		float anteriorAngle = (ssTrapezius / mapTo) * 90;
		float posteriorAngle = -1 * (ssClavicle / mapTo) * 90;

		// Correction factors.
		if (applyCorrectionFactors)
		{
			float axillaCorrection = 0.15f * ssAxilla / mapTo;
			float deltoidCorrection = 0.15f * ssDeltoid / mapTo;
			axillaCorrection = float.IsNaN (axillaCorrection) ? 0.0f : axillaCorrection;
			deltoidCorrection = float.IsNaN (deltoidCorrection) ? 0.0f : deltoidCorrection;

			anteriorAngle = anteriorAngle - axillaCorrection - deltoidCorrection;
			posteriorAngle = posteriorAngle + axillaCorrection + deltoidCorrection;
		}
		
		// Make sure we return floating point numbers.
		anteriorAngle = float.IsNaN (anteriorAngle) ? 0.0f : anteriorAngle;
		posteriorAngle = float.IsNaN (posteriorAngle) ? 0.0f : posteriorAngle;

		if (showDebug) {
			print ("Axial angle (anterior + posterior): " + anteriorAngle + " + " + posteriorAngle);
		}
		
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

		// Initial calculations.
		float superiorAngle = (ssAxilla / mapTo) * 90;
		float inferiorAngle = -1 * (ssDeltoid / mapTo) * 90;
		
		// Correction factors.
		if (applyCorrectionFactors)
		{
			float clavicleCorrection = 0.15f * ssClavicle / mapTo;
			float trapeziusCorrection = 0.15f * ssTrapezius / mapTo;
			clavicleCorrection = float.IsNaN (clavicleCorrection) ? 0.0f : clavicleCorrection;
			trapeziusCorrection = float.IsNaN (trapeziusCorrection) ? 0.0f : trapeziusCorrection;
			
			superiorAngle = superiorAngle - clavicleCorrection - trapeziusCorrection;
			inferiorAngle = inferiorAngle + clavicleCorrection + trapeziusCorrection;
		}
		
		// Make sure we return floating point numbers.
		superiorAngle = float.IsNaN (superiorAngle) ? 0.0f : superiorAngle;
		inferiorAngle = float.IsNaN (inferiorAngle) ? 0.0f : inferiorAngle;
		
		if (showDebug) {
			print ("Frontal angle (superior + inferior): " + superiorAngle + " + " + inferiorAngle);
		}
		
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
		if (axilla != null && clavicle != null && deltoid != null && trapezius != null)
		{
			float axialAngle = getAxialAngle (axilla, clavicle, deltoid, trapezius);
			float frontalAngle = getFrontalAngle (axilla, clavicle, deltoid, trapezius);
			float rotAngle = getRotAngle (axilla, clavicle, deltoid, trapezius);
			
			mCurJointRotationEuler.y = axialAngle * rotatesXYZ.y;
			mCurJointRotationEuler.z = frontalAngle * rotatesXYZ.z;
			mCurJointRotationEuler.x = rotAngle * rotatesXYZ.x;

			if (showDebug) {
				print("Euler angles (x / y / z): "+ mCurJointRotationEuler.x +" / "+ mCurJointRotationEuler.y +" / "+ mCurJointRotationEuler.y);
			}
			
			jointTransform.localRotation = Quaternion.Euler (mCurJointRotationEuler);
		}

		else {
			print ("Missing shoulder sensors...");
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

