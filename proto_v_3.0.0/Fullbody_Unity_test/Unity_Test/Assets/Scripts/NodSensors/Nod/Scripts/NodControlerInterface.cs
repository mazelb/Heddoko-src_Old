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
using Nod;

public interface NodControlerInterface
{
	void ConnectToNod();
	void ShutdownNodConnection();

	int GetNumDevices();

	NodRing GetRing(int ringID);
	string GetRingName(int ringIndex);

	//API's specific to a ring
	//void SetTouchToMoveMode(int ringIndex);
	//void Set3DMode(int ringIndex);

	NodQuaternionOrientation QuaternionOrientation(int ringIndex);
	int ButtonState(int ringIndex);
	int Gesture(int ringIndex);
	NodPosition2D Position2D(int ringIndex);

	bool SubscribeToButton(int ringIndex);
	bool SubscribeToPose6D(int ringIndex);
	bool SubscribeToGesture(int ringIndex);
	bool SubscribeToPosition2D(int ringIndex);

	bool UnsubscribeToButton(int ringIndex);
	bool UnsubscribeToPose6D(int ringIndex);
	bool UnsubscribeToGesture(int ringIndex);
	bool UnsubscribeToPosition2D(int ringIndex);
}
