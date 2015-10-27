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

public interface NodControllerInterface
{
	void ConnectToNod();
	void ShutdownNodConnection();

	void ApplicationFocusChanged(bool focusStatus);
	void ClearData();

	int GetNumDevices();

	NodDevice GetNodDevice(int deviceIndex);
	string GetNodDeviceName(int deviceIndex);

	void RequestBatteryPercent(int deviceIndex);
	int BatteryPercent(int deviceIndex);

	NodQuaternionOrientation QuaternionOrientation(int deviceIndex);

    NodEulerOrientation EulerOrientation(int deviceIndex);
	int [] ButtonState(int deviceIndex);
	int Gesture(int deviceIndex);
	NodPosition2D Position2D(int deviceIndex);

	NodAccel Acceleration(int deviceIndex);
	NodGyro Gyro(int deviceIndex);

	NodPosition2D GamePosition(int deviceIndex);
	int TriggerPressure(int deviceIndex);

	bool Subscribe(NodSubscriptionType type, int deviceIndex);
	bool Unsubscribe(NodSubscriptionType type, int deviceIndex);
}