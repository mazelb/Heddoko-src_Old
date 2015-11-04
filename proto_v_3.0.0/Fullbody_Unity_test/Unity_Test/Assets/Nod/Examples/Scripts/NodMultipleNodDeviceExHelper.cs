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

using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.IO;
using Nod;

public class NodMultipleNodDeviceExHelper : MonoBehaviour
{
	public int deviceID = 0;
	private NodController nod;
	private NodDevice nodDevice = null;
	private bool nodDeviceConnected = false;
	private Quaternion inverseInitialRotation = Quaternion.identity;

	private bool NodDeviceConnectedAndInitialized()
	{
		if (!nodDeviceConnected) {
			//Nod device connections happen asynchronously on mobile devices, check each frame for a connected nod device
			int numNodDevicesPaired = nod.getNumDevices();
			if (numNodDevicesPaired > deviceID) {
				nodDevice = nod.getNodDevice(deviceID);
				if (null == nodDevice)
					return false;
				nodDevice.Subscribe(NodSubscriptionType.EulerMode);
				nodDevice.Subscribe(NodSubscriptionType.ButtonMode);
				recenter();
				nodDeviceConnected = true;
			} else
				return false;
		}

		return true;
	}

	void Start ()
	{
		nod = NodController.GetNodInterface();

		timer = new Stopwatch();
		timer.Start();
	}

	void OnDisable()
	{
		if (null == nodDevice)
			return;

		nodDevice.Unsubscribe(NodSubscriptionType.EulerMode);
		nodDevice.Unsubscribe(NodSubscriptionType.ButtonMode);
	}

	void Update ()
	{
		if (!NodDeviceConnectedAndInitialized())
			return;

        //Example of applying the nod device orientation to the local transform.
        nodDevice.CheckForUpdate();
        transform.localRotation = inverseInitialRotation * nodDevice.rotation;
	}

	public void recenter()
	{
		inverseInitialRotation = Quaternion.Inverse(nodDevice.rotation);
	}

	private Stopwatch timer;
	private bool onTheClock = false;
	private bool done = false;
	private string msgPost = string.Empty;

	private long startTime;
	private long endTime;

	public string DeviceName()
	{
		if (null == nodDevice)
			return "";

		string result = nodDevice.GetNodDeviceName();
		result = "\nNodDeviceIndex: " + deviceID.ToString();

		Vector3 eulers = transform.localEulerAngles;
		if (!onTheClock && eulers.y > 25.0f && eulers.y < 180.0f) {
			onTheClock = true;
			startTime = timer.ElapsedMilliseconds;
			msgPost = "\nStart: " + timer.ElapsedMilliseconds;
		}
		if (!done && onTheClock && eulers.y > 80.0f && eulers.y < 180.0f) {
			done = true;
			endTime = timer.ElapsedMilliseconds;
			msgPost += "\nStop: " + timer.ElapsedMilliseconds;
			timer.Stop();
			msgPost += "\nDelta: " + (endTime - startTime).ToString();
		}

		return result + msgPost;
	}
}
