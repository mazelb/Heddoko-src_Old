using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class SSJointHip : SSJoint
{
	/**
     * @brief                           Gets the abduction/adduction angle of the hip in degrees.
     * @param StretchSensor vMaximus    StretchSensor attached to gluteus maximus muscle.
     * @param StretchSensor vMedius     StretchSensor attached to gluteus medius muscle.
     * @param StretchSensor vIliacus    StretchSensor attached to iliacus muscle.
     * @return float                    Abduction/adduction angle of the hip.
     */
	private float GetAbductionAngle(StretchSensor vMaximus, StretchSensor vMedius, StretchSensor vIliacus)
	{
		// Initial calculations.
		int vMapTo = 1000;
		float vThetaMax = 80.0f;
		float vThetaRef = 30.0f;
		float vMediusValue = vMedius.GetMappedReading(vMapTo);
		float vAngle = (vMapTo - vMediusValue) * (vThetaMax / vMapTo) - vThetaRef;
		
		// Corrections.
		float vCorrection = (float) Math.Pow(vMaximus.GetFractionalReading(), 2);
		vAngle -= vCorrection * vThetaMax;
		
		if (vShowDebug)
		{
			print("medius / correction / angle: " + vMediusValue + " / " + vCorrection + " / " + vAngle);
		}
		
		return vAngle;
	}
	
	/**
     * @brief                           Gets the flexion/extension angle of the hip in degrees.
     * @param StretchSensor vMaximus    StretchSensor attached to gluteus maximus muscle.
     * @param StretchSensor vMedius     StretchSensor attached to gluteus medius muscle.
     * @param StretchSensor vIliacus    StretchSensor attached to iliacus muscle.
     * @return float                    Flexion/extension angle of the hip.
     */
	private float GetFlexionAngle(StretchSensor vMaximus, StretchSensor vMedius, StretchSensor vIliacus)
	{
		// Initial calculations.
		int vMapTo = 1000;
		float vIliacusValue = vIliacus.GetMappedReading(vMapTo);
		float vAngle = (vMapTo - vIliacusValue) * (150.0f / vMapTo) - 30.0f;

		// Corrections.
		
		if (vShowDebug)
		{
			print("iliacus / flex angle: " + vIliacusValue + " / " + vAngle);
		}
		
		return vAngle;
	}
	
	/**
     * @brief                           Gets the pronation/supination angle of the hip in degrees.
     * @param StretchSensor vMaximus    StretchSensor attached to gluteus maximus muscle.
     * @param StretchSensor vMedius     StretchSensor attached to gluteus medius muscle.
     * @param StretchSensor vIliacus    StretchSensor attached to iliacus muscle.
     * @return float                    Pronation/supination angle of the hip.
     */
	private float GetPronationAngle(StretchSensor vMaximus, StretchSensor vMedius, StretchSensor vIliacus)
	{
		float vAngle = 0.0f;
		
		return vAngle;
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
		StretchSensor vMaximus = GetSensorByPosition(StretchSensor.PositionName.GluteusMaximus);
		StretchSensor vMedius = GetSensorByPosition(StretchSensor.PositionName.GluteusMedius);
		StretchSensor vIliacus = GetSensorByPosition(StretchSensor.PositionName.Iliacus);

        // Update hip orientation.
		if (vMaximus != null && vMedius != null && vIliacus != null)
		{
			mOrientationEuler.x = vRotationalDirections.x * GetFlexionAngle(vMaximus, vMedius, vIliacus);
			mOrientationEuler.y = vRotationalDirections.y * GetPronationAngle(vMaximus, vMedius, vIliacus);
			mOrientationEuler.z = vRotationalDirections.z * GetAbductionAngle(vMaximus, vMedius, vIliacus);

			vJointObject.localRotation = Quaternion.Euler(mOrientationEuler);
		}
		
		else
		{
			print("Missing hip sensors...");
		}
    }
}

