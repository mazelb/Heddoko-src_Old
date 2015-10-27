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
using Nod;
using System;

public class NodDevice
{
	private NodControllerInterface deviceInterface;
	public int deviceIndex;
	public string deviceAddress;

    public Vector3 eulerRotation;
	public Quaternion rotation;
	private int [] buttonState;
	public GestureEventType gestureState;
	public NodPosition2D position2D;
	public NodAccel acceleration;
	public NodGyro gyro;

	//Values should be in the range of -1.0f to 1.0f for x and y
	public Vector2 joyStickPosition = Vector2.zero;

	//Values should be in the range of 0.0f to 1.0f, 0 for no pressure, 1.0 for full force.
	public float triggerPressure = 0.0f;

	private NodPosition2D rawUnprocessedGameStickPosition;
	private int rawUnprocessedTriggerPressure;

	//For whatever reason the enum doesn't start at 0 for unmanaged implimentations so add 1
	static private int numSubscribableTypes = Enum.GetNames(typeof(NodSubscriptionType)).Length + 1;

	private int [] subscribeCount = new int[numSubscribableTypes];
	private bool [] subscribedTo = new bool[numSubscribableTypes];

	private void init(int index, string address, NodControllerInterface nci)
	{
		deviceIndex = index;
		deviceAddress = address;
		deviceInterface = nci;

		for (int ndx = 0; ndx < numSubscribableTypes; ndx++) {
			subscribedTo[ndx] = false;
		}

		rotation = Quaternion.identity;

		gestureState = GestureEventType.NONE;

		position2D.x = 0;
		position2D.y = 0;

		acceleration.accelX = 0.0f;
		acceleration.accelY = 0.0f;
		acceleration.accelZ = 0.0f;

		gyro.gyroX = 0.0f;
		gyro.gyroY = 0.0f;
		gyro.gyroZ = 0.0f;

		rawUnprocessedGameStickPosition.x = 0;
		rawUnprocessedGameStickPosition.y = 0;

		rawUnprocessedTriggerPressure = 0;
	}

	public NodDevice(int index, NodControllerInterface nci)
	{
		init(index, "unknown", nci);
	}

	public NodDevice(int index, string address, NodControllerInterface nci)
	{
		init(index, address, nci);
	}

	//returns true while the indexed button is held down.
	public bool GetNodButton(int buttonIndex)
	{
		//If we haven't gotten a button update yet or
		//the requested button index is outside the range of what is supported for this device
		//then the button is not down.
		if (buttonState == null || buttonIndex > buttonState.Length)
			return false;

		//1 means pressed, everything else = not pressed
		return buttonState[buttonIndex] == 1;
	}

	public void CheckForUpdate()
	{
		if (subscribedTo[(int)NodSubscriptionType.ButtonMode]) {
			buttonState = deviceInterface.ButtonState(deviceIndex);
		}
		if (subscribedTo[(int)NodSubscriptionType.EulerMode]) {
			//Read the raw quaternion from the nod device
			NodQuaternionOrientation orientation = deviceInterface.QuaternionOrientation(deviceIndex);
			Quaternion rot = new Quaternion(orientation.x, orientation.y, orientation.z, orientation.w);

			rotation = rot;
		    NodEulerOrientation eulerRotation = deviceInterface.EulerOrientation(deviceIndex);
            this.eulerRotation = new Vector3(eulerRotation.pitch, eulerRotation.roll, eulerRotation.yaw);
		}
		if (subscribedTo[(int)NodSubscriptionType.GestureMode]) {
			int gestureEnumValue = deviceInterface.Gesture(deviceIndex);
			gestureState = (GestureEventType)gestureEnumValue;
		}
		if (subscribedTo[(int)NodSubscriptionType.PointerMode]) {
			position2D = deviceInterface.Position2D(deviceIndex);
		}

		//TODO PAW: fix this once Android subscription model is updated
		//Combining these is temporary until Android subscription model seperates out Accel and Gyro
		if (subscribedTo[(int)NodSubscriptionType.AccelMode] || subscribedTo[(int)NodSubscriptionType.GyroMode]) {
			acceleration = deviceInterface.Acceleration(deviceIndex);
			gyro = deviceInterface.Gyro(deviceIndex);
		}

		if (subscribedTo[(int)NodSubscriptionType.GameControlMode]) {
			rawUnprocessedGameStickPosition = deviceInterface.GamePosition(deviceIndex);
			rawUnprocessedTriggerPressure = deviceInterface.TriggerPressure(deviceIndex);

			//Joystick data flows in from 0-255 for x and y, where 128,128 is supposed to be the center.
			joyStickPosition = new Vector2((float)(rawUnprocessedGameStickPosition.x - 128),
			                                            (float)(rawUnprocessedGameStickPosition.y - 128));
			joyStickPosition /= 128.0f;

			triggerPressure = ((float)rawUnprocessedTriggerPressure) / 255.0f;
		}
	}

	public void PrintCurrentState()
	{
		if (subscribedTo[(int)NodSubscriptionType.ButtonMode]) {
			if (buttonState == null || buttonState.Length < 5) {
			} else {
				Debug.Log ("Button state: " +
				           "touch0: " + (buttonState[0] == 1 ? "D" : "U") + ", " +
				           "touch1: " + (buttonState[1] == 1 ? "D" : "U") + ", " +
				           "touch2: " + (buttonState[2] == 1 ? "D" : "U") + ", " +
				           "tactile0: " + (buttonState[3] == 1 ? "D" : "U") + ", " +
				           "tactile1: " + (buttonState[4] == 1 ? "D" : "U") + ", ");
			}
		}
		if (subscribedTo[(int)NodSubscriptionType.EulerMode]) {
			Debug.Log ("Rotation quaternion: " + rotation.ToString());
		}
		if (subscribedTo[(int)NodSubscriptionType.GestureMode]) {
			Debug.Log("Gesture Event type: " + gestureState.ToString());
		}
		if (subscribedTo[(int)NodSubscriptionType.PointerMode]) {
			Debug.Log("Position Delta (x, y): " + position2D.x + ", " + position2D.y);
		}
		if (subscribedTo[(int)NodSubscriptionType.GameControlMode]) {
			Debug.Log ("Game (x, y): " + rawUnprocessedGameStickPosition.x + ", " + rawUnprocessedGameStickPosition.y);
		}
	}

	public void StopTracking()
	{
		for (int ndx = 0; ndx < numSubscribableTypes; ndx++) {
			if (subscribedTo[ndx]) {
				Unsubscribe((NodSubscriptionType)ndx);
			}
		}
	}

	public bool Subscribe(NodSubscriptionType type)
	{
		if ((int)type >= numSubscribableTypes || (int)type < 0) {
			Debug.Log("Unknown subscription type");
			return false;
		}

		bool subscriptionWorked = false;

		int index = (int)type;

		subscribeCount[index]++;
		if (1 == subscribeCount[index]) {
			subscriptionWorked = deviceInterface.Subscribe(type, deviceIndex);
			subscribedTo[index] = subscriptionWorked;
		} else
			subscriptionWorked = subscribedTo[index];

		return subscriptionWorked;
	}

	public bool Unsubscribe(NodSubscriptionType type)
	{
		if ((int)type >= numSubscribableTypes || (int)type < 0) {
			Debug.Log("Unknown subscription type");
			return false;
		}

		bool unSubscriptionWorked = false;

		int index = (int)type;

		subscribeCount[index]--;
		if (0 == subscribeCount[index]) {
			subscribedTo[index] = false;
			unSubscriptionWorked = deviceInterface.Unsubscribe(type, deviceIndex);
		} else
			unSubscriptionWorked = true;

		return unSubscriptionWorked;
	}

	public void SetApplicationPauseStatus(bool focusStatus)
	{
		//Deal with subscription and unsubscription with out impacting the count
		//or the tracking of what is subscribed to.  Those values shouldn't be
		//changing while the app doesn't have focus anyways.  This is necessary
		//to get into a zero subscription state while the app is out of focus
		//so that the nod device will start advertising mouse and keyboard events again
		//as a HID device.
		if (focusStatus) {
			bool subscriptionWorked = true;
			for (int ndx = 0; ndx < numSubscribableTypes; ndx++) {
				if (subscribedTo[ndx]) {
					subscriptionWorked = deviceInterface.Subscribe((NodSubscriptionType)ndx, deviceIndex);
					if (!subscriptionWorked) {
						Debug.Log("Can't seem to re-subscribe after loss of focus.");
					}
				}
			}
		} else {
			bool unSubscriptionWorked = true;
			for (int ndx = 0; ndx < numSubscribableTypes; ndx++) {
				if (subscribedTo[ndx]) {
					unSubscriptionWorked = deviceInterface.Unsubscribe((NodSubscriptionType)ndx, deviceIndex);
					if (!unSubscriptionWorked) {
						Debug.Log("Can't seem to unsubscribe during focus loss.");
					}
				}
			}
		}
	}

	public void ClearData()
	{
		rotation = Quaternion.identity;

		gestureState = GestureEventType.NONE;

		position2D.x = 0;
		position2D.y = 0;

		//TODO PAW need to check that these are the appropriate defaults
		rawUnprocessedGameStickPosition.x = 128;
		rawUnprocessedGameStickPosition.y = 128;
		rawUnprocessedTriggerPressure = 0;
	}

	public string GetNodDeviceName()
	{
		return deviceInterface.GetNodDeviceName(deviceIndex);
	}

	public void RequestBatteryPercent()
	{
		deviceInterface.RequestBatteryPercent(deviceIndex);
	}

	private const int APIError = -2;
	private const int ValueNotRecievedYet = -1;

	public bool BatteryPercent(ref int batteryPercent)
	{
		int readValue = deviceInterface.BatteryPercent(deviceIndex);
		if (readValue == APIError) {
			return false;
		} else if (readValue == ValueNotRecievedYet) {
			return false;
		} else {
			//Do a basic sanity check on the input
			batteryPercent = readValue;
			if (batteryPercent > 100 || batteryPercent < 0) {
				Debug.Log("Unexpected battery percentage: " + batteryPercent.ToString());
				return false;
			}
		}

		return true;
	}
}
