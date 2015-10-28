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
	protected int numRings = 0;
	protected NodRing [] rings;
	#endregion protected data

	protected virtual void InitRings()
	{
		rings = new NodRing[numRings];
		for (int ndx = 0; ndx < numRings; ndx++) {
			rings[ndx] = new NodRing(ndx, this);
		}
	}

	#region NodControllerInterface methods
	//For working with Unity
	public void ConnectToNod()
	{
		NodUtilities.NodInitialize();
	}

	public NodRing GetRing(int ringIndex)
	{
		if (ringIndex >= numRings)
			return null;

		return rings[ringIndex];
	}

	public string GetRingName(int ringIndex)
	{
		return Marshal.PtrToStringAnsi(NodUtilities.NodGetRingName(ringIndex));
	}

	public void RequestBatteryPercent(int ringIndex)
	{
		NodUtilities.NodRequestBatteryPercentage(ringIndex);
	}

	public int BatteryPercent(int ringIndex)
	{
		return NodUtilities.NodGetBatteryPercentage(ringIndex);
	}

	public void ShutdownNodConnection()
	{
		for (int ndx = 0; ndx < numRings; ndx++)
			rings[ndx].StopTracking();
		NodUtilities.NodShutdown();
	}

	public void ApplicationFocusChanged(bool focusStatus)
	{
		for (int ndx = 0; ndx < numRings; ndx++)
			rings[ndx].SetApplicationPauseStatus(focusStatus);
	}

	public int GetNumDevices()
	{
		int currentRingCount = NodUtilities.NodNumRings ();
		if (currentRingCount != numRings) {
			numRings = currentRingCount;
			InitRings ();
		}
		return numRings;
	}

	public NodEulerOrientation EulerOrientation(int ringIndex)
	{
		return NodUtilities.NodGetEulerOrientation(ringIndex);
	}

	public NodQuaternionOrientation QuaternionOrientation(int ringIndex)
	{
		return NodUtilities.NodGetQuaternionOrientation(ringIndex);
	}

	public int ButtonState(int ringIndex)
	{
		return NodUtilities.NodGetButtonState(ringIndex);
	}

	public int Gesture(int ringIndex)
	{
		return NodUtilities.NodGetGesture(ringIndex);
	}

	public NodPosition2D Position2D(int ringIndex)
	{
		return NodUtilities.NodGetPosition2D(ringIndex);
	}

#if NOD_BACKSPIN
	public NodPosition2D GamePosition(int ringIndex)
	{
		return NodUtilities.NodGetGamePosition(ringIndex);
	}

	public int TriggerPressure(int ringIndex)
	{
		return NodUtilities.NodGetTrigger(ringIndex);
	}
#endif

	public bool Subscribe(NodSubscriptionType type, int ringIndex)
	{
		bool result = false;
		switch(type){
		case NodSubscriptionType.Button:
			result = NodUtilities.NodSubscribeToButton(ringIndex);
			break;
		case NodSubscriptionType.Gesture:
			result = NodUtilities.NodSubscribeToGesture(ringIndex);
			break;
		case NodSubscriptionType.Orientation:
			result = NodUtilities.NodSubscribeToPose6D(ringIndex);
			break;
		case NodSubscriptionType.Position2D:
			result = NodUtilities.NodSubscribeToPosition2D(ringIndex);
			break;

#if NOD_BACKSPIN
		case NodSubscriptionType.GameStick:
			result = NodUtilities.NodSubscribeToGameControl(ringIndex);
			break;
#endif

		default:
			Debug.Log ("Unhandeled Subscription type.");
			break;
		}

		return result;
	}

	public bool Unsubscribe(NodSubscriptionType type, int ringIndex)
	{
		bool result = false;
		switch(type){
		case NodSubscriptionType.Button:
			result = NodUtilities.NodUnsubscribeToButton(ringIndex);
			break;
		case NodSubscriptionType.Gesture:
			result = NodUtilities.NodUnsubscribeToGesture(ringIndex);
			break;
		case NodSubscriptionType.Orientation:
			result = NodUtilities.NodUnsubscribeToPose6D(ringIndex);
			break;
		case NodSubscriptionType.Position2D:
			result = NodUtilities.NodUnsubscribeToPosition2D(ringIndex);
			break;

#if NOD_BACKSPIN
		case NodSubscriptionType.GameStick:
			result = NodUtilities.NodUnSubscribeToGameControl(ringIndex);
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
