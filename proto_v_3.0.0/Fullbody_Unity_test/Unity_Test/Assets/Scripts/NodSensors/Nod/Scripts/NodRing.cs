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
	private NodControlerInterface ringInterface;
	public int ringIndex;
	public string ringAddress;

	public Quaternion ringRotation;
	public NodButtons buttonState;
	public GestureEventType gestureState;
	public NodPosition2D position2D;

	public enum RingModes { TouchToMove, Pose6D, GamePad };

	private bool subscribedToButton;
	private bool subscribedToPose6D;
	private bool subscribedToGesture;
	private bool subscribedToPosition2D;

	private bool readFirstOrientation = false;
	private Quaternion inverseInitialOrientation = Quaternion.identity;

	private void init(int index, string address, NodControlerInterface nci)
	{
		ringIndex = index;
		ringAddress = address;
		ringInterface = nci;

		subscribedToButton = false;
		subscribedToPose6D = false;
		subscribedToGesture = false;
		subscribedToPosition2D = false;

		ringRotation = Quaternion.identity;

		buttonState.touch0 = false;
		buttonState.touch1 = false;
		buttonState.touch2 = false;
		buttonState.tactile0 = false;
		buttonState.tactile1 = false;

		gestureState = GestureEventType.NONE;

		position2D.x = 0;
		position2D.y = 0;
	}

	public NodRing(int index, NodControlerInterface nci)
	{
		init(index, "unknown", nci);
	}

	public NodRing(int index, string address, NodControlerInterface nci)
	{
		init(index, address, nci);
	}

	/*
	public void SetMode(RingModes mode)
	{
		return;

		//Don't switch mode in code
		if (mode == RingModes.TouchToMove)
			ringInterface.SetTouchToMoveMode(ringIndex);
		else if (mode == RingModes.Pose6D)
			ringInterface.Set3DMode(ringIndex);
		else if (mode == RingModes.GamePad) {
			//Not currently exposed
		}
	}
	*/

	public void CheckForUpdate()
	{
		if (subscribedToButton) {
			int buttonBitField = ringInterface.ButtonState(ringIndex);
			buttonState.touch0 = (buttonBitField & (1 << 0)) != 0;
			buttonState.touch1 = (buttonBitField & (1 << 1)) != 0;
			buttonState.touch2 = (buttonBitField & (1 << 2)) != 0;
			buttonState.tactile0 = (buttonBitField & (1 << 3)) != 0;
			buttonState.tactile1 = (buttonBitField & (1 << 4)) != 0;
		}
		if (subscribedToPose6D) {
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
		}
		if (subscribedToGesture) {
			int gestureEnumValue = ringInterface.Gesture(ringIndex);
			gestureState = (GestureEventType)gestureEnumValue;
		}
		if (subscribedToPosition2D) {
			position2D = ringInterface.Position2D(ringIndex);
		}
	}

	public void PrintCurrentState()
	{
		if (subscribedToButton) {
			Debug.Log ("Button state: " +
			           "touch0: " + (buttonState.touch0 ? "D" : "U") + ", " +
			           "touch1: " + (buttonState.touch1 ? "D" : "U") + ", " +
			           "touch2: " + (buttonState.touch2 ? "D" : "U") + ", " +
			           "tactile0: " + (buttonState.tactile0 ? "D" : "U") + ", " +
			           "tactile1: " + (buttonState.tactile1 ? "D" : "U") + ", ");
		}
		if (subscribedToPose6D) {
			Debug.Log ("Rotation quaternion: " + ringRotation.ToString());
		}
		if (subscribedToGesture) {
			Debug.Log("Gesture Event type: " + gestureState.ToString());
		}
		if (subscribedToPosition2D) {
			Debug.Log("Position Delta (x, y): " + position2D.x + ", " + position2D.y);
		}
	}

	public void StopTracking()
	{
		if (subscribedToButton)
			UnsubscribeToButton();
		if (subscribedToPose6D)
			UnsubscribeToPose6D();
		if (subscribedToGesture)
			UnsubscribeToGesture();
		if (subscribedToPosition2D)
			UnsubscribeToPosition2D();
	}

	private int buttonSubscribeCount = 0;
	public bool SubscribeToButton()
	{
		buttonSubscribeCount++;
		if (1 == buttonSubscribeCount) {
			bool subscriptionWorked = ringInterface.SubscribeToButton(ringIndex);
			subscribedToButton = subscriptionWorked;
			return subscriptionWorked;
		} 

		return true;
	}

	public bool UnsubscribeToButton()
	{
		buttonSubscribeCount--;
		if (0 == buttonSubscribeCount) {
			subscribedToButton = false;
			return ringInterface.UnsubscribeToButton(ringIndex);
		}

		return true;
	}

	private int pose6DSubscribeCount = 0;
	public bool SubscribeToPose6D()
	{
		pose6DSubscribeCount++;
		if (1 == pose6DSubscribeCount) {
			bool subscriptionWorked = ringInterface.SubscribeToPose6D(ringIndex);
			subscribedToPose6D = subscriptionWorked;
			return subscriptionWorked;
		}

		return subscribedToPose6D;
	}

	public bool UnsubscribeToPose6D()
	{
		pose6DSubscribeCount--;
		if (0 == pose6DSubscribeCount) {
			subscribedToPose6D = false;
			return ringInterface.UnsubscribeToPose6D(ringIndex);
		} 

		return true;
	}

	private int gestureSubscribeCount = 0;
	public bool SubscribeToGesture()
	{
		gestureSubscribeCount++;
		if (1 == gestureSubscribeCount) {
			bool subscriptionWorked = ringInterface.SubscribeToGesture(ringIndex);
			subscribedToGesture = subscriptionWorked;
			return subscriptionWorked;
		}

		return subscribedToGesture;
	}

	public bool UnsubscribeToGesture()
	{
		gestureSubscribeCount--;
		if (0 == gestureSubscribeCount) {
			subscribedToGesture = false;
			return ringInterface.UnsubscribeToGesture(ringIndex);
		}

		return true;
	}

	private int position2DSubscribeCount = 0;
	public bool SubscribeToPosition2D()
	{
		position2DSubscribeCount++;
		if (1 == position2DSubscribeCount) {
			bool subscriptionWorked = ringInterface.SubscribeToPosition2D(ringIndex);
			subscribedToPosition2D = subscriptionWorked;
			return subscriptionWorked;
		}

		return subscribedToPosition2D;
	}

	public bool UnsubscribeToPosition2D()
	{
		position2DSubscribeCount--;
		if (0 == position2DSubscribeCount) {
			subscribedToPosition2D = false;
			return ringInterface.UnsubscribeToPosition2D(ringIndex);
		}

		return true;
	}

	public string GetRingName()
	{
		return ringInterface.GetRingName(ringIndex);
	}
}
