/**
 * @file ss_joint_shoulder.cs
 * @brief Describe what this script does.
 * @note Add notes.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class SSJointShoulder : SSJoint
{
    /**
     * 
     */
    public bool vApplyCorrection = true;
	
    /**
     * @brief                           Gets the axial angle of the shoulder in degrees.
     * @param StretchSensor vAxilla     StretchSensor attached to axilla.
     * @param StretchSensor vClavicle   StretchSensor attached to clavicle bone.
     * @param StretchSensor vDeltoid    StretchSensor attached to deltoid muscle.
     * @param StretchSensor vTrapezius  StretchSensor attached to trapezius.
     * @return float                    Axial angle of the shoulder.
     */
    private float GetAxialAngle(StretchSensor vAxilla, StretchSensor vClavicle, StretchSensor vDeltoid, StretchSensor vTrapezius)
    {
        int vMapTo = 1000;
        float vAxillaValue = vAxilla.GetMappedReading(vMapTo);
        float vClavicleValue = vClavicle.GetMappedReading(vMapTo);
		float vClavicleFraction = vClavicle.GetFractionalReading();
        float vDeltoidValue = vDeltoid.GetMappedReading(vMapTo);
        float vTrapeziusValue = vTrapezius.GetMappedReading(vMapTo);

        // Initial calculations.
		float vAnteriorAngle = (vTrapeziusValue / vMapTo) * 110.0f;
		//float vPosteriorAngle = -1 * (vClavicleValue / vMapTo) * 80.0f * (float) (Math.Sin(vClavicleFraction * Math.PI - Math.PI/2) + 1)/2.0f;
		float vPosteriorAngle = -1 * (vClavicleValue / vMapTo) * 80.0f;

        // Correction factors.
        if (vApplyCorrection)
        {
            float vAxillaCorrection = 0.15f * vAxillaValue / vMapTo;
            float vDeltoidCorrection = 0.15f * vDeltoidValue / vMapTo;
            vAxillaCorrection = float.IsNaN(vAxillaCorrection) ? 0.0f : vAxillaCorrection;
            vDeltoidCorrection = float.IsNaN(vDeltoidCorrection) ? 0.0f : vDeltoidCorrection;

            vAnteriorAngle = vAnteriorAngle - vAxillaCorrection - vDeltoidCorrection;
            vPosteriorAngle = vPosteriorAngle + vAxillaCorrection + vDeltoidCorrection;
        }

        // Make sure we return floating point numbers.
        vAnteriorAngle = float.IsNaN(vAnteriorAngle) ? 0.0f : vAnteriorAngle;
        vPosteriorAngle = float.IsNaN(vPosteriorAngle) ? 0.0f : vPosteriorAngle;

        if (vShowDebug)
        {
            print("Axial angle (anterior + posterior): " + vAnteriorAngle + " + " + vPosteriorAngle);
        }

        return vAnteriorAngle + vPosteriorAngle;
    }

    /**
     * @brief                           Gets the frontal angle of the shoulder in degrees.
     * @param StretchSensor vAxilla     StretchSensor attached to axilla.
     * @param StretchSensor vClavicle   StretchSensor attached to clavicle bone.
     * @param StretchSensor vDeltoid    StretchSensor attached to deltoid muscle.
     * @param StretchSensor vTrapezius  StretchSensor attached to trapezius.
     * @return float                    Frontal angle of the shoulder.
     */
    private float GetFrontalAngle(StretchSensor vAxilla, StretchSensor vClavicle, StretchSensor vDeltoid, StretchSensor vTrapezius)
    {
        int vMapTo = 1000;
        float vAxillaValue = vAxilla.GetMappedReading(vMapTo);
        float vClavicleValue = vClavicle.GetMappedReading(vMapTo);
        float vDeltoidValue = vDeltoid.GetMappedReading(vMapTo);
        float vTrapeziusValue = vTrapezius.GetMappedReading(vMapTo);

        // Initial calculations.
        float vSuperiorAngle = (vAxillaValue / vMapTo) * 90;
        float vInferiorAngle = -1 * (vDeltoidValue / vMapTo) * 90;

        // Correction factors.
        if (vApplyCorrection)
        {
            // TODO: describe these factors.
            float vClavicleCorrection = 0.15f * vClavicleValue / vMapTo;
            float vTrapeziusCorrection = 0.15f * vTrapeziusValue / vMapTo;
            vClavicleCorrection = float.IsNaN (vClavicleCorrection) ? 0.0f : vClavicleCorrection;
            vTrapeziusCorrection = float.IsNaN (vTrapeziusCorrection) ? 0.0f : vTrapeziusCorrection;

            vSuperiorAngle = vSuperiorAngle - vClavicleCorrection - vTrapeziusCorrection;
            vInferiorAngle = vInferiorAngle + vClavicleCorrection + vTrapeziusCorrection;
        }

        // Make sure we return floating point numbers.
        vSuperiorAngle = float.IsNaN(vSuperiorAngle) ? 0.0f : vSuperiorAngle;
        vInferiorAngle = float.IsNaN(vInferiorAngle) ? 0.0f : vInferiorAngle;

        if (vShowDebug)
        {
            print("Frontal angle (superior + inferior): " + vSuperiorAngle + " + " + vInferiorAngle);
        }

        return vSuperiorAngle + vInferiorAngle;
    }
	
    /**
     * @brief                           Gets the rotational angle of the shoulder in degrees.
     * @param StretchSensor vAxilla     StretchSensor attached to axilla.
     * @param StretchSensor vClavicle   StretchSensor attached to clavicle bone.
     * @param StretchSensor vDeltoid    StretchSensor attached to deltoid muscle.
     * @param StretchSensor vTrapezius  StretchSensor attached to trapezius.
     * @return float                    Frontal angle of the shoulder.
     */
    private float GetRotationalAngle(StretchSensor vAxilla, StretchSensor vClavicle, StretchSensor vDeltoid, StretchSensor vTrapezius)
    {
        float angle = 0.0f;

        return angle;
    }
	
    /**
     * @brief           Updates joint position and values.
     * @return void
     */
    public override void UpdateJoint()
    {
        // Don't update anything if joint is paused.
        if (vPauseAnimation)
        {
            return;
        }

        // Update individual sensors.
        for (int i = 0; i < maSensors.Length; i++)
        {
            maSensors[i].UpdateSensor();
        }

        // Retrieve sensor objects.
        StretchSensor vAxilla = GetSensorByPosition(StretchSensor.PositionName.Axilla);
        StretchSensor vClavicle = GetSensorByPosition(StretchSensor.PositionName.Clavicle);
        StretchSensor vDeltoid = GetSensorByPosition(StretchSensor.PositionName.Deltoid);
        StretchSensor vTrapezius = GetSensorByPosition(StretchSensor.PositionName.Trapezius);

        // Update shoulder orientation.
        if (vAxilla != null && vClavicle != null && vDeltoid != null && vTrapezius != null)
        {
			mOrientationEuler.x = vRotationalDirections.x * GetRotationalAngle(vAxilla, vClavicle, vDeltoid, vTrapezius);
			mOrientationEuler.y = vRotationalDirections.y * GetAxialAngle(vAxilla, vClavicle, vDeltoid, vTrapezius);
			mOrientationEuler.z = vRotationalDirections.z * GetFrontalAngle(vAxilla, vClavicle, vDeltoid, vTrapezius);

            if (vShowDebug)
            {
                print("Euler angles (x / y / z): "+ mOrientationEuler.x +" / "+ mOrientationEuler.y +" / "+ mOrientationEuler.y);
            }

            vJointObject.localRotation = Quaternion.Euler(mOrientationEuler);
        }

        else
        {
            print("Missing shoulder sensors...");
        }
    }
}

