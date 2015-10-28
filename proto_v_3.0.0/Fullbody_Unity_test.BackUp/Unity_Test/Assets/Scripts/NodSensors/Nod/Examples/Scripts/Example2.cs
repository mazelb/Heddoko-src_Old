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

public class Example2 : MonoBehaviour 
{
	private NodController nod;
	private NodRing ring;	
	private bool nodRingConnected = false;	
	private const int ringID = 0; //0 for the first connected ring
	
	private bool RingConnectedAndInitialized()
	{
		if (!nodRingConnected) {
			//Ring connections happen asynchronously on mobile devices, check each frame for a connected ring
			int numRingsPaired = nod.getNumDevices();
			if (numRingsPaired > 0) {
				ring = nod.getRing(ringID);
				ring.Subscribe(NodSubscriptionType.Gesture);
				ring.Subscribe(NodSubscriptionType.Position2D);
				nodRingConnected = true;
			} else 
				return false;
		}
		
		return true;
	}

	void OnEnable() 
	{
		//This will create a GameObject in your Hierarchy called "NodController" which will manage
		//interactions with all connected nod rings.  It will presist between scene loads.  Only
		//one instance will be created if you request a nod interface from multiple locations 
		//in your code.
		nod = NodController.GetNodInterface();		
	}

	void OnDisable()
	{
		if (null == ring)
			return;

		ring.Unsubscribe(NodSubscriptionType.Gesture);
		ring.Unsubscribe(NodSubscriptionType.Position2D);
	}
	
	void Update() 
	{
		if (!RingConnectedAndInitialized())
			return;

		//Call this once per update to check for updated ring values.
		ring.CheckForUpdate();
		
		//Example of applying the rings orientation to the local transform.
		Vector3 updatePosition = new Vector3(0.001f * (float)ring.position2D.x, -0.001f * ring.position2D.y);
		transform.localPosition = transform.localPosition + updatePosition;
	}

	private GestureEventType mostRecentGesture = GestureEventType.NONE;

	void OnGUI()
	{
		if (!nodRingConnected) {		
			Rect windowRect = new Rect(Screen.width/2f - Screen.width/8f, 
			                           Screen.height/2f - Screen.height/8f, 
			                           Screen.width/4f,
			                           Screen.height/4f);
			string message = "Unable to find a paired Nod rings.\nLoad the blue tooth settings for your\nmachine and make sure a Nod ring is connected.";
			GUI.Window(0, windowRect, noConnectionWindow, message);
		} else {
			if (ring.gestureState != GestureEventType.NONE)
				mostRecentGesture = ring.gestureState;
			Rect windowRect = new Rect(0, 0, Screen.width, 30);
			string text = "Most recent gesture: " + mostRecentGesture.ToString();
			GUI.Button(windowRect, text);		
		}
	}
	
	private void noConnectionWindow(int windowID) 
	{
		const int buttonWidth = 100;
		const int buttonHeight = 20;
		if (GUI.Button(new Rect(Screen.width/8f - buttonWidth/2f, 
		                        Screen.width/8f - buttonHeight/2f - 15, 
		                        buttonWidth, 
		                        buttonHeight), "Ok")) 
		{
			Application.Quit();
		}
	}
}
