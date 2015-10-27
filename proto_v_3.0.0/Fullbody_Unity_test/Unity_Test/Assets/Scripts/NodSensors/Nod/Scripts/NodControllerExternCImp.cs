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
using System.Runtime.InteropServices;
using Nod;

public class NodControllerExternCImp : NodControllerInterface
{
	#region protected data
	protected int numNodDevices = 0;
	protected NodDevice [] nodDevices;
	#endregion protected data

	protected virtual void InitNodDevices()
	{
		nodDevices = new NodDevice[numNodDevices];
		for (int ndx = 0; ndx < numNodDevices; ndx++) {
			nodDevices[ndx] = new NodDevice(ndx, this);
		}
	}

	#region NodControllerInterface methods
	//For working with Unity
	public void ConnectToNod()
	{
		NodUtilities.NodInitialize();
	}

	public NodDevice GetNodDevice(int deviceIndex)
	{
		if (deviceIndex >= numNodDevices)
			return null;

		return nodDevices[deviceIndex];
	}

	public string GetNodDeviceName(int deviceIndex)
	{
		//TODO PAW update this once api names change away from ring to device
		return Marshal.PtrToStringAnsi(NodUtilities.NodGetRingName(deviceIndex));
	}

	public void RequestBatteryPercent(int deviceIndex)
	{
		NodUtilities.NodRequestBatteryPercentage(deviceIndex);
	}

	public int BatteryPercent(int deviceIndex)
	{
		return NodUtilities.NodGetBatteryPercentage(deviceIndex);
	}

	public void ShutdownNodConnection()
	{
		for (int ndx = 0; ndx < numNodDevices; ndx++)
			nodDevices[ndx].StopTracking();
		NodUtilities.NodShutdown();
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

	public int GetNumDevices()
	{
		//TODO PAW update this from num rings to devices once the windows side api changes.
		int currentNodDeviceCount = NodUtilities.NodNumRings();
		if (currentNodDeviceCount != numNodDevices) {
			numNodDevices = currentNodDeviceCount;
			InitNodDevices();
		}
		return numNodDevices;
	}

	public NodQuaternionOrientation QuaternionOrientation(int deviceIndex)
	{
		return NodUtilities.NodGetQuaternionOrientation(deviceIndex);
	}

    public NodEulerOrientation EulerOrientation(int deviceIndex)
    {
        return NodUtilities.NodGetEulerOrientation(deviceIndex);
    }

	public int [] ButtonState(int deviceIndex)
	{
		int numButtons = NodUtilities.NodGetButtons(deviceIndex);
		//this could be more efficient, but we are just hacking things to see what works at this point.
		int [] buttonStates = new int[numButtons];
		for (int ndx = 0; ndx < numButtons; ndx++) {
			buttonStates[ndx] = NodUtilities.NodGetButtonState(deviceIndex, ndx);
		}

		//new way
		return buttonStates;
	}

	public int Gesture(int deviceIndex)
	{
		return NodUtilities.NodGetGesture(deviceIndex);
	}

	public NodPosition2D Position2D(int deviceIndex)
	{
		return NodUtilities.NodGetPosition2D(deviceIndex);
	}

	public NodAccel Acceleration(int deviceIndex)
	{
		return NodUtilities.NodGetAccel(deviceIndex);
	}

	public NodGyro Gyro(int deviceIndex)
	{
		return NodUtilities.NodGetGyro(deviceIndex);
	}

	public NodPosition2D GamePosition(int deviceIndex)
	{
		return NodUtilities.NodGetGamePosition(deviceIndex);
	}

	public int TriggerPressure(int deviceIndex)
	{
		return NodUtilities.NodGetTrigger(deviceIndex);
	}

	public bool Subscribe(NodSubscriptionType type, int deviceIndex)
	{
		bool result = false;
		result = NodUtilities.NodSubscribe((int)type, deviceIndex);

		return result;
	}

	public bool Unsubscribe(NodSubscriptionType type, int deviceIndex)
	{
		bool result = false;
		result = NodUtilities.NodUnsubscribe((int)type, deviceIndex);

		return result;
	}

	#endregion NodControllerInterface
}
