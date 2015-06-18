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

public class NodRing
{
	private NodControllerInterface ringInterface;
	public int ringIndex;
	public string ringAddress;

	public Vector3 ringEulerRotation;
	public Quaternion ringRotation;
	public NodButtons buttonState;
	public GestureEventType gestureState;
	public NodPosition2D position2D;

#if NOD_BACKSPIN
	public NodPosition2D gamePosition;
	public int triggerPressure;
#endif

	private bool readFirstOrientation = false;
	private Quaternion inverseInitialOrientation = Quaternion.identity;

	private int [] subscribeCount = new int[(int)NodSubscriptionType.Count];
	private bool [] subscribedTo = new bool[(int)NodSubscriptionType.Count];

	private void init(int index, string address, NodControllerInterface nci)
	{
		ringIndex = index;
		ringAddress = address;
		ringInterface = nci;

		for (int ndx = 0; ndx < (int)NodSubscriptionType.Count; ndx++) {
			subscribedTo[ndx] = false;
		}

		ringRotation = Quaternion.identity;

		buttonState.touch0 = false;
		buttonState.touch1 = false;
		buttonState.touch2 = false;
		buttonState.tactile0 = false;
		buttonState.tactile1 = false;

		gestureState = GestureEventType.NONE;

		position2D.x = 0;
		position2D.y = 0;

#if NOD_BACKSPIN
		gamePosition.x = 0;
		gamePosition.y = 0;

		triggerPressure = 0;
#endif
	}

	public NodRing(int index, NodControllerInterface nci)
	{
		init(index, "unknown", nci);
	}

	public NodRing(int index, string address, NodControllerInterface nci)
	{
		init(index, address, nci);
	}

	public void CheckForUpdate()
	{
		if (subscribedTo[(int)NodSubscriptionType.Button]) {
			int buttonBitField = ringInterface.ButtonState(ringIndex);
			buttonState.touch0 = (buttonBitField & (1 << 0)) != 0;
			buttonState.touch1 = (buttonBitField & (1 << 1)) != 0;
			buttonState.touch2 = (buttonBitField & (1 << 2)) != 0;
			buttonState.tactile0 = (buttonBitField & (1 << 3)) != 0;
			buttonState.tactile1 = (buttonBitField & (1 << 4)) != 0;
		}
		if (subscribedTo[(int)NodSubscriptionType.Orientation]) {
			//Read the raw quaternion from the ring
			NodQuaternionOrientation orientation = ringInterface.QuaternionOrientation(ringIndex);
			Quaternion rot = new Quaternion(orientation.x, orientation.y, orientation.z, orientation.w);

			//Make the first value we read the new origin for this ring.  Until the ring updates rot will be Quaternion.identity
			//This will prevent drastic jumps the first time you read from the ring.
			if (!readFirstOrientation && (rot != Quaternion.identity)) {
				readFirstOrientation = true;
				inverseInitialOrientation = Quaternion.Inverse(rot);
			}
			if (readFirstOrientation) {
				rot = inverseInitialOrientation * rot;
			}

			ringRotation = rot;

			NodEulerOrientation eulerRot = ringInterface.EulerOrientation(ringIndex);
			ringEulerRotation = new Vector3(eulerRot.pitch, eulerRot.roll, eulerRot.yaw);
		}
		if (subscribedTo[(int)NodSubscriptionType.Gesture]) {
			int gestureEnumValue = ringInterface.Gesture(ringIndex);
			gestureState = (GestureEventType)gestureEnumValue;
		}
		if (subscribedTo[(int)NodSubscriptionType.Position2D]) {
			position2D = ringInterface.Position2D(ringIndex);
		}

#if NOD_BACKSPIN
		if (subscribedTo[(int)NodSubscriptionType.GameStick]) {
			gamePosition = ringInterface.GamePosition(ringIndex);
			triggerPressure = ringInterface.TriggerPressure(ringIndex);
		}
#endif
	}

	public void PrintCurrentState()
	{
		if (subscribedTo[(int)NodSubscriptionType.Button]) {
			Debug.Log ("Button state: " +
			           "touch0: " + (buttonState.touch0 ? "D" : "U") + ", " +
			           "touch1: " + (buttonState.touch1 ? "D" : "U") + ", " +
			           "touch2: " + (buttonState.touch2 ? "D" : "U") + ", " +
			           "tactile0: " + (buttonState.tactile0 ? "D" : "U") + ", " +
			           "tactile1: " + (buttonState.tactile1 ? "D" : "U") + ", ");
		}
		if (subscribedTo[(int)NodSubscriptionType.Orientation]) {
			Debug.Log ("Rotation quaternion: " + ringRotation.ToString());
		}
		if (subscribedTo[(int)NodSubscriptionType.Gesture]) {
			Debug.Log("Gesture Event type: " + gestureState.ToString());
		}
		if (subscribedTo[(int)NodSubscriptionType.Position2D]) {
			Debug.Log("Position Delta (x, y): " + position2D.x + ", " + position2D.y);
		}

#if NOD_BACKSPIN
		if (subscribedTo[(int)NodSubscriptionType.GameStick]) {
			Debug.Log ("Game (x, y): " + gamePosition.x + ", " + gamePosition.y);
		}
#endif
	}

	public void StopTracking()
	{
		for (int ndx = 0; ndx < (int)NodSubscriptionType.Count; ndx++) {
			if (subscribedTo[ndx]) {
				Unsubscribe((NodSubscriptionType)ndx);
			}
		}
	}

	public bool Subscribe(NodSubscriptionType type)
	{
		if ((int)type >= (int)NodSubscriptionType.Count || (int)type < 0) {
			Debug.Log("Unknown subscription type");
			return false;
		}

		bool subscriptionWorked = false;

		int index = (int)type;

		subscribeCount[index]++;
		if (1 == subscribeCount[index]) {
			subscriptionWorked = ringInterface.Subscribe(type, ringIndex);
			subscribedTo[index] = subscriptionWorked;
		} else
			subscriptionWorked = subscribedTo[index];

		return subscriptionWorked;
	}

	public bool Unsubscribe(NodSubscriptionType type)
	{
		if ((int)type >= (int)NodSubscriptionType.Count || (int)type < 0) {
			Debug.Log("Unknown subscription type");
			return false;
		}

		bool unSubscriptionWorked = false;

		int index = (int)type;

		subscribeCount[index]--;
		if (0 == subscribeCount[index]) {
			subscribedTo[index] = false;
			unSubscriptionWorked = ringInterface.Unsubscribe(type, ringIndex);
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
		//so that the ring will start advertising mouse and keyboard events again
		//as a HID device.
		if (focusStatus) {
			bool subscriptionWorked = true;
			for (int ndx = 0; ndx < (int)NodSubscriptionType.Count; ndx++) {
				if (subscribedTo[ndx]) {
					subscriptionWorked = ringInterface.Subscribe((NodSubscriptionType)ndx, ringIndex);
					if (!subscriptionWorked) {
						Debug.Log("Can't seem to re-subscribe after loss of focus.");
					}
				}
			}
		} else {
			bool unSubscriptionWorked = true;
			for (int ndx = 0; ndx < (int)NodSubscriptionType.Count; ndx++) {
				if (subscribedTo[ndx]) {
					unSubscriptionWorked = ringInterface.Unsubscribe((NodSubscriptionType)ndx, ringIndex);
					if (!unSubscriptionWorked) {
						Debug.Log("Can't seem to unsubscribe during focus loss.");
					}
				}
			}
		}
	}

	public string GetRingName()
	{
		return ringInterface.GetRingName(ringIndex);
	}

	public void RequestBatteryPercent()
	{
		ringInterface.RequestBatteryPercent(ringIndex);
	}

	private const int APIError = -2;
	private const int ValueNotRecievedYet = -1;

	public bool BatteryPercent(ref int batteryPercent)
	{
		int readValue = ringInterface.BatteryPercent(ringIndex);
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
