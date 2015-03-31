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

#if UNITY_ANDROID && !UNITY_EDITOR_WIN
using UnityEngine;
using System;
using System.Collections;
using Nod;

//This file is out of date Android support in unity currently broken.
public class NodControlerAndroidImp : NodControlerInterface
{
	#region private data
	private NodRing[] rings;
	private AndroidJavaObject unityPlugin;
	#endregion // private data

	#region NodControlerInterface methods
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

		int[] ringIds = unityPlugin.Call<int[]>("getDeviceIds");
		int numRings = ringIds.Length;

		if (numRings <= 0) {
			// No rings are paired
			Debug.Log ("No Nod rings are paired with this Android device.");
			return;
		}

		rings = new NodRing[numRings];
		for (int i = 0; i < numRings; i++) {
			int id = ringIds[i];
			string address = unityPlugin.Call<string>("getDeviceAddress", id);

			rings[i] = new NodRing(id, address, this);
		}
	}

	public void ShutdownNodConnection()
	{
		if (unityPlugin != null) {
			unityPlugin.Call("shutdown");
		}
	}

	public int GetNumDevices()
	{
		if (null == rings) {
			return 0;
		}

		return rings.Length;
	}

	public NodRing GetRing(int ringID)
	{
		if (ringID >= rings.Length)
			return null;

		return rings[ringID];
	}

	public string GetRingName(int ringId)
	{
		return rings[ringId].ringAddress;
	}

	/*
	public void SetTouchToMoveMode(int ringId)
	{
		//TODO
	}

	public void Set3DMode(int ringId)
	{
		//TODO
	}
	*/

	public NodQuaternionOrientation QuaternionOrientation(int ringId)
	{
		float [] eulers = unityPlugin.Call<float[]>("getRotationData", ringId);
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
		return unityPlugin.Call<int>("getButtonData", ringId);
	}

	public int Gesture(int ringId)
	{
		return unityPlugin.Call<int>("getGestureData", ringId);
	}

	public NodPosition2D Position2D(int ringId)
	{
		NodPosition2D result;
		int [] pointerData = unityPlugin.Call<int[]>("getPointerData", ringId);
		result.x = pointerData[0];
		result.y = pointerData[1];
		return result;
	}

	public bool SubscribeToButton(int ringId)
	{
		return unityPlugin.Call<bool>("registerForButtonEvents", ringId);
	}

	public bool SubscribeToPose6D(int ringId)
	{
		return unityPlugin.Call<bool>("registerForPose6DEvents", ringId);
	}

	public bool SubscribeToGesture(int ringId)
	{
		return unityPlugin.Call<bool>("registerForGestureEvents", ringId);
	}

	public bool SubscribeToPosition2D(int ringId)
	{
		return unityPlugin.Call<bool>("registerForPointerEvents", ringId);
	}

	public bool UnsubscribeToButton(int ringId)
	{
		return unityPlugin.Call<bool>("unregisterFromButtonEvents", ringId);
	}

	public bool UnsubscribeToPose6D(int ringId)
	{
		return unityPlugin.Call<bool>("unregisterFromPose6DEvents", ringId);
	}

	public bool UnsubscribeToGesture(int ringId)
	{
		return unityPlugin.Call<bool>("unregisterFromGestureEvents", ringId);
	}

	public bool UnsubscribeToPosition2D(int ringId)
	{
		return unityPlugin.Call<bool>("unregisterFromPointerEvents", ringId);
	}
	#endregion NodControlerInterface
}
#endif
