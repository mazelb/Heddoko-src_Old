/*
 * Copyright 2014 Nod Labs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#if UNITY_ANDROID // && !UNITY_EDITOR
using UnityEngine;
using System;
using System.Collections;
using Nod;

//This file is out of date Android support in unity currently broken.
public class NodControllerAndroidImp : NodControllerInterface
{
	#region protected data
	protected int numNodDevices = 0;
	protected NodDevice [] nodDevices;
	protected AndroidJavaObject unityPlugin;
	protected AndroidJavaObject openSpatialActivity;
	#endregion // protected data

	#region NodControllerInterface methods
	public void ConnectToNod()
	{
		using (AndroidJavaClass jc = new AndroidJavaClass("com.unity3d.player.UnityPlayer")) {
			openSpatialActivity = jc.GetStatic<AndroidJavaObject>("currentActivity");
		}

		using (AndroidJavaClass jc = new AndroidJavaClass("com.nod_labs.unityplugin.UnityPlugin")) {
			unityPlugin = jc.CallStatic<AndroidJavaObject>("getInstance");
			unityPlugin.Call("init", openSpatialActivity);
		}
	}
	
	public void ShutdownNodConnection()
	{
		if (unityPlugin != null) {
			unityPlugin.CallStatic("shutdown");
		}
	}

	protected virtual void InitNodDevices()
	{
		nodDevices = new NodDevice[numNodDevices];
		int [] nodDeviceIds = unityPlugin.CallStatic<int[]>("getDeviceIds");
		for (int ndx = 0; ndx < numNodDevices; ndx++) {
			int id = nodDeviceIds[ndx];
			string address = unityPlugin.CallStatic<string>("getDeviceAddress", id);
			nodDevices[ndx] = new NodDevice(id, address, this);
			nodDevices[ndx].deviceAddress = address;
		}
	}

	public int GetNumDevices()
	{
		int currentNodDeviceCount = unityPlugin.CallStatic<int>("getNumDevices");

		if (currentNodDeviceCount != numNodDevices) {
			numNodDevices = currentNodDeviceCount;
			InitNodDevices();
		}

		return numNodDevices;
	}

	public NodDevice GetNodDevice(int deviceIndex)
	{
		if (deviceIndex >= nodDevices.Length)
			return null;

		return nodDevices[deviceIndex];
	}

	public void ApplicationFocusChanged(bool focusStatus)
	{
		for (int ndx = 0; ndx < numNodDevices; ndx++)
			nodDevices[ndx].SetApplicationPauseStatus(focusStatus);
	}

	public void ClearData()
	{
		for (int ndx = 0; ndx < numNodDevices; ndx++)
			nodDevices[ndx].ClearData();
	}

	public string GetNodDeviceName(int deviceIndex)
	{
		return unityPlugin.CallStatic<string>("nodGetName", deviceIndex);
	}

	public void RequestBatteryPercent(int deviceIndex)
	{
		unityPlugin.CallStatic("requestBatteryLevel", deviceIndex);
	}

	public int BatteryPercent(int deviceIndex)
	{
		return unityPlugin.CallStatic<int>("getBatteryLevel", deviceIndex);
	}

	public NodQuaternionOrientation QuaternionOrientation(int deviceIndex)
	{
		float [] eulers = unityPlugin.CallStatic<float[]>("getRotationData", deviceIndex);
		return eulerToQuaternion(eulers[0], eulers[1], eulers[2]);
	}

	private static NodQuaternionOrientation eulerToQuaternion(float pitch, float roll, float yaw)
	{
		float sinHalfYaw = Mathf.Sin(yaw / 2.0f);
		float cosHalfYaw = Mathf.Cos(yaw / 2.0f);
		float sinHalfPitch = Mathf.Sin(pitch / 2.0f);
		float cosHalfPitch = Mathf.Cos(pitch / 2.0f);
		float sinHalfRoll = Mathf.Sin(roll / 2.0f);
		float cosHalfRoll = Mathf.Cos(roll / 2.0f);

		NodQuaternionOrientation result;
		result.x = -cosHalfRoll * sinHalfPitch * sinHalfYaw
			+ cosHalfPitch * cosHalfYaw * sinHalfRoll;
		result.y = cosHalfRoll * cosHalfYaw * sinHalfPitch
			+ sinHalfRoll * cosHalfPitch * sinHalfYaw;
		result.z = cosHalfRoll * cosHalfPitch * sinHalfYaw
			- sinHalfRoll * cosHalfYaw * sinHalfPitch;
		result.w = cosHalfRoll * cosHalfPitch * cosHalfYaw
			+ sinHalfRoll * sinHalfPitch * sinHalfYaw;

		return result;
	}

	public int [] ButtonState(int deviceIndex)
	{
		int numButtons = unityPlugin.CallStatic<int>("getNumButtons");

		//this could be more efficient, but we are just hacking things to see what works at this point.
		int [] buttonStates = new int[numButtons];
		for (int ndx = 0; ndx < numButtons; ndx++) {
			buttonStates[ndx] = unityPlugin.CallStatic<int>("getButtonState", deviceIndex, ndx);
		}

		return buttonStates;
	}

	public int Gesture(int deviceIndex)
	{
		return unityPlugin.CallStatic<int>("getGestureData", deviceIndex);
	}

	public NodPosition2D Position2D(int deviceIndex)
	{
		NodPosition2D result;
		int [] pointerData = unityPlugin.CallStatic<int[]>("getPointerData", deviceIndex);
		result.x = pointerData[0];
		result.y = pointerData[1];
		return result;
	}

	public NodGyro Gyro(int deviceIndex)
	{
		NodGyro tempGyro;

		float [] gyroData = unityPlugin.CallStatic<float[]>("getGyroData", deviceIndex);
		tempGyro.gyroX = gyroData[0];
		tempGyro.gyroY = gyroData[1];
		tempGyro.gyroZ = gyroData[2];

		return tempGyro;
	}

	public NodAccel Acceleration(int deviceIndex)
	{

		NodAccel tempAccel;
		float [] accelData = unityPlugin.CallStatic<float[]>("getAccelData", deviceIndex);

		tempAccel.accelX = accelData[0];
		tempAccel.accelY = accelData[1];
		tempAccel.accelZ = accelData[2];

		return tempAccel;
	}

	public NodPosition2D GamePosition(int deviceIndex)
	{
		NodPosition2D result;
		int [] pointerData = unityPlugin.CallStatic<int[]>("getAnalogData", deviceIndex);
		result.x = pointerData[0];
		result.y = pointerData[1];
		return result;
	}

	public int TriggerPressure(int deviceIndex)
	{
		int [] pointerData = unityPlugin.CallStatic<int[]>("getAnalogData", deviceIndex);
		return pointerData[2];
	}

	public bool Subscribe(NodSubscriptionType type, int deviceIndex)
	{
		bool result = false;
		switch(type){
		case NodSubscriptionType.ButtonMode:
			result = unityPlugin.CallStatic<bool>("registerForButtonEvents", deviceIndex);
			break;
		case NodSubscriptionType.GestureMode:
			result = unityPlugin.CallStatic<bool>("registerForGestureEvents", deviceIndex);
			break;
		case NodSubscriptionType.EulerMode:
			result = unityPlugin.CallStatic<bool>("registerForPose6DEvents", deviceIndex);
			break;
		case NodSubscriptionType.PointerMode:
			result = unityPlugin.CallStatic<bool>("registerForPointerEvents", deviceIndex);
			break;
		case NodSubscriptionType.GyroMode:
			result = unityPlugin.CallStatic<bool>("registerForMotion6DEvents", deviceIndex);
			break;
		case NodSubscriptionType.GameControlMode:
			result = unityPlugin.CallStatic<bool>("registerForAnalogDataEvents", deviceIndex);
			break;
		default:
			Debug.Log ("Unhandeled Subscription type.");
			break;
		}

		return result;
	}

	public bool Unsubscribe(NodSubscriptionType type, int deviceIndex)
	{
		bool result = false;
		switch(type){
		case NodSubscriptionType.ButtonMode:
			result = unityPlugin.CallStatic<bool>("unregisterFromButtonEvents", deviceIndex);
			break;
		case NodSubscriptionType.GestureMode:
			result = unityPlugin.CallStatic<bool>("unregisterFromGestureEvents", deviceIndex);
			break;
		case NodSubscriptionType.EulerMode:
			result = unityPlugin.CallStatic<bool>("unregisterFromPose6DEvents", deviceIndex);
			break;
		case NodSubscriptionType.PointerMode:
			result = unityPlugin.CallStatic<bool>("unregisterFromPointerEvents", deviceIndex);
			break;
		case NodSubscriptionType.GyroMode:
			result = unityPlugin.CallStatic<bool>("unregisterFromMotion6DEvents", deviceIndex);
			break;
		case NodSubscriptionType.GameControlMode:
			result = unityPlugin.CallStatic<bool>("unregisterFromAnalogDataEvents", deviceIndex);
			break;
		default:
			Debug.Log ("Unhandeled unsubscription type.");
			break;
		}

		return result;
	}
	#endregion NodControllerInterface
}
#endif
