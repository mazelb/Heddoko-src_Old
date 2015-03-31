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

public class NodControlerExternCImp : NodControlerInterface
{	
	#region private data
	private int numRings = 0;
	private NodRing [] rings;
	#endregion private data

	private void InitRings() 
	{
		rings = new NodRing[numRings];
		for (int ndx = 0; ndx < numRings; ndx++) {
			rings[ndx] = new NodRing(ndx, this);
		}		
	}

	#region NodControlerInterface methods
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

	public void ShutdownNodConnection()
	{
		for (int ndx = 0; ndx < numRings; ndx++) 
			rings[ndx].StopTracking();
		NodUtilities.NodShutdown();
	}
	
	public int GetNumDevices()
	{
		int currentRingCount = NodUtilities.NodNumRings();
		if (currentRingCount != numRings) {
			numRings = currentRingCount;
			InitRings();
		}
		return numRings;		
	}

	/*
	public void SetTouchToMoveMode(int ringIndex)
	{
		//Don't call mode switches
		//NodUtilities.NodSetMode(ringIndex, (int)NodDeviceModes.MODE_TTM);
	}

	public void Set3DMode(int ringIndex)
	{
		//Don't call mode switches
		//NodUtilities.NodSetMode(ringIndex, (int)NodDeviceModes.MODE_3D);
	}
	*/

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

	public bool SubscribeToButton(int ringIndex)
	{
		return NodUtilities.NodSubscribeToButton(ringIndex);
	}

	public bool SubscribeToPose6D(int ringIndex)
	{
		return NodUtilities.NodSubscribeToPose6D(ringIndex);
	}

	public bool SubscribeToGesture(int ringIndex)
	{
		return NodUtilities.NodSubscribeToGesture(ringIndex);
	}

	public bool SubscribeToPosition2D(int ringIndex)
	{
		return NodUtilities.NodSubscribeToPosition2D(ringIndex);
	}

	public bool UnsubscribeToButton(int ringIndex)
	{
		//TODO for all non-PC clients
		//return NodUtilities.NodUnsubscribeToButton(ringIndex);
		return true;
	}

	public bool UnsubscribeToPose6D(int ringIndex)
	{
		//TODO for all non-PC clients
		//return NodUtilities.NodUnsubscribeToPose6D(ringIndex);
		return true;
	}

	public bool UnsubscribeToGesture(int ringIndex)
	{
		//TODO for all non-PC clients
		//return NodUtilities.NodUnsubscribeToGesture(ringIndex);
		return true;
	}

	public bool UnsubscribeToPosition2D(int ringIndex)
	{
		//TODO for all non-PC clients
		//return NodUtilities.NodUnsubscribeToPosition2D(ringIndex);
		return true;
	}

	#endregion NodControlerInterface	
}
