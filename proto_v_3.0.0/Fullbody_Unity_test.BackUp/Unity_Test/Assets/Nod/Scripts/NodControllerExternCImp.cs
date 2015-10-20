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

#if UNITY_ANDROID && !UNITY_EDITOR
using UnityEngine;
using System;
using System.Collections;
using Nod;

//This file is out of date Android support in unity currently broken.
public class NodControllerAndroidImp : NodControllerInterface
{
	#region protected data
	protected int numRings = 0;
	protected NodRing[] rings;
	protected AndroidJavaObject unityPlugin;
	#endregion // protected data

	#region NodControllerInterface methods
	public void ConnectToNod()
	{
		AndroidJavaObject activity;

		using (AndroidJavaClass jc =
			new AndroidJavaClass("com.unity3d.player.UnityPlayer")) {
			activity = jc.GetStatic<AndroidJavaObject>("currentActivity");
		}

		using (AndroidJavaClass jc =
			new AndroidJavaClass("com.nod_labs.unityplugin.UnityPlugin")) {
			unityPlugin = jc.CallStatic<AndroidJavaObject>("getInstance");
			unityPlugin.Call("init", activity);
		}
	}

	public void ShutdownNodConnection()
	{
		if (unityPlugin != null) {
			unityPlugin.Call("shutdown");
		}
	}

	protected virtual void InitRings()
	{
		rings = new NodRing[numRings];
		int [] ringIds = unityPlugin.CallStatic<int[]>("getDeviceIds");
		for (int ndx = 0; ndx < numRings; ndx++) {
			int id = ringIds[ndx];
			string address = unityPlugin.CallStatic<string>("getDeviceAddress", id);
			rings[ndx] = new NodRing(id, address, this);
			rings[ndx].ringAddress = address;
		}
	}

	public int GetNumDevices()
	{
		int currentRingCount = unityPlugin.CallStatic<int>("getNumDevices");

		if (currentRingCount != numRings) {
			numRings = currentRingCount;
			InitRings();
		}

		return numRings;
	}

	public NodRing GetRing(int ringID)
	{
		if (ringID >= rings.Length)
			return null;

		return rings[ringID];
	}

	public void ApplicationFocusChanged(bool focusStatus)
	{
		for (int ndx = 0; ndx < numRings; ndx++)
			rings[ndx].SetApplicationPauseStatus(focusStatus);
	}

	public string GetRingName(int ringId)
	{
		return unityPlugin.CallStatic<string>("nodGetName", ringId);
	}

	public void RequestBatteryPercent(int ringIndex)
	{
		unityPlugin.CallStatic("requestBatteryLevel", ringIndex);
	}

	public int BatteryPercent(int ringIndex)
	{
		return unityPlugin.CallStatic<int>("getBatteryLevel", ringIndex);
	}

	public NodQuaternionOrientation QuaternionOrientation(int ringId)
	{
		float [] eulers = unityPlugin.CallStatic<float[]>("getRotationData", ringId);
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

	public int ButtonState(int ringId)
	{
		return unityPlugin.CallStatic<int>("getButtonData", ringId);
	}

	public int Gesture(int ringId)
	{
		return unityPlugin.CallStatic<int>("getGestureData", ringId);
	}

	public NodPosition2D Position2D(int ringId)
	{
		NodPosition2D result;
		int [] pointerData = unityPlugin.CallStatic<int[]>("getPointerData", ringId);
		result.x = pointerData[0];
		result.y = pointerData[1];
		return result;
	}

#if NOD_BACKSPIN
	public NodPosition2D GamePosition(int ringId)
	{
		NodPosition2D result;
		int [] pointerData = unityPlugin.CallStatic<int[]>("getAnalogData", ringId);
		result.x = pointerData[0];
		result.y = pointerData[1];
		return result;
	}

	public int TriggerPressure(int ringId)
	{
		int [] pointerData = unityPlugin.CallStatic<int[]>("getAnalogData", ringId);
		return pointerData[2];
	}
#endif

	public bool Subscribe(NodSubscriptionType type, int ringId)
	{
		bool result = false;
		switch(type){
		case NodSubscriptionType.Button:
			result = unityPlugin.CallStatic<bool>("registerForButtonEvents", ringId);
			break;
		case NodSubscriptionType.Gesture:
			result = unityPlugin.CallStatic<bool>("registerForGestureEvents", ringId);
			break;
		case NodSubscriptionType.Orientation:
			result = unityPlugin.CallStatic<bool>("registerForPose6DEvents", ringId);
			break;
		case NodSubscriptionType.Position2D:
			result = unityPlugin.CallStatic<bool>("registerForPointerEvents", ringId);
			break;

#if NOD_BACKSPIN
		case NodSubscriptionType.GameStick:
			result = unityPlugin.CallStatic<bool>("registerForAnalogDataEvents", ringId);
			break;
#endif

		default:
			Debug.Log ("Unhandeled Subscription type.");
			break;
		}

		return result;
	}

	public bool Unsubscribe(NodSubscriptionType type, int ringId)
	{
		bool result = false;
		switch(type){
		case NodSubscriptionType.Button:
			result = unityPlugin.CallStatic<bool>("unregisterFromButtonEvents", ringId);
			break;
		case NodSubscriptionType.Gesture:
			result = unityPlugin.CallStatic<bool>("unregisterFromGestureEvents", ringId);
			break;
		case NodSubscriptionType.Orientation:
			result = unityPlugin.CallStatic<bool>("unregisterFromPose6DEvents", ringId);
			break;
		case NodSubscriptionType.Position2D:
			result = unityPlugin.CallStatic<bool>("unregisterFromPointerEvents", ringId);
			break;

#if NOD_BACKSPIN
		case NodSubscriptionType.GameStick:
			result = unityPlugin.CallStatic<bool>("unregisterFromAnalogDataEvents", ringId);
			break;
#endif

		default:
			Debug.Log ("Unhandeled unsubscription type.");
			break;
		}

		return result;
	}
	#endregion NodControllerInterface
}
#endif
