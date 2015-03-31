﻿/*
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

public class Example1 : MonoBehaviour 
{
	private NodControler nod;
	private NodRing ring;
	private bool nodRingConnected = false;
	private const int ringID = 0; //0 for the first connected ring

	//Rotation to get the ring from where it started to where it should be once we recenter
	private Quaternion inverseInitialRotation = Quaternion.identity;
	
	private bool RingConnectedAndInitialized()
	{
		if (!nodRingConnected) {
			//Ring connections happen asynchronously on mobile devices, check each frame for a connected ring
			int numRingsPaired = nod.getNumDevices();
			if (numRingsPaired > 0) {
				ring = nod.getRing(ringID);
				ring.SubscribeToPose6D();
				ring.SubscribeToButton();
				recenter();
				nodRingConnected = true;
			} else 
				return false;
		}
		
		return true;
	}

	public void OnEnable() 
	{
		//This will create a GameObject in your Hierarchy called "NodControler" which will manage
		//interactions with all connected nod rings.  It will presist between scene loads.  Only
		//one instance will be created if you request a nod interface from multiple locations 
		//in your code.
		nod = NodControler.GetNodInterface();
	}

	public void OnDisable()
	{
		if (null == ring)
			return;

		ring.UnsubscribeToPose6D();
		ring.UnsubscribeToButton();
	}
	
	public void Update() 
	{
		if (!RingConnectedAndInitialized())
			return;

		//Call this once per update to check for updated ring values.
		ring.CheckForUpdate();

		if (Input.GetKeyDown(KeyCode.Space))
			recenter();

		//Example of applying the rings orientation to the local transform.
		transform.localRotation = inverseInitialRotation  * ring.ringRotation;
	}

	public void OnGUI()
	{
		if (!nodRingConnected) {		
			Rect windowRect = new Rect(Screen.width/2f - Screen.width/8f, 
			                           Screen.height/2f - Screen.height/8f, 
			                           Screen.width/4f,
			                           Screen.height/4f);
			string message = "Unable to find a paired Nod rings.\nLoad the blue tooth settings for your\nmachine and make sure a Nod ring is connected.";
			GUI.Window(0, windowRect, noConnectionWindow, message);
        } else {

			//Display the status of each button
			string [] buttonNames = {"touch0", "touch1", "touch2", "tactile0", "tactile1"};
			string [] buttonPressStatus = {
				ring.buttonState.touch0 ? "Down" : "Up",
				ring.buttonState.touch1 ? "Down" : "Up",
				ring.buttonState.touch2 ? "Down" : "Up",
				ring.buttonState.tactile0 ? "Down" : "Up",
				ring.buttonState.tactile1 ? "Down" : "Up"
			};

			int numWindows = buttonNames.Length;
			float windowWidth = Screen.width / numWindows;
			float windowHeight = 40;
			for (int ndx = 0; ndx < numWindows; ndx++) {
				float currentX = windowWidth * ndx;
				Rect windowRect = new Rect(currentX, 0, 
				                           windowWidth, windowHeight);
				string text = buttonNames[ndx] + " status:\n " + buttonPressStatus[ndx];
				GUI.Button(windowRect, text);
			}

			//Button to reorient the model relative to the current orientation of the ring
			Rect bottomWindowRect = new Rect(0, Screen.height - 30, Screen.width, 30);
			string message = "Click here, or tap space, to reorient the model relative to the current orientation of the ring";
			if (GUI.Button(bottomWindowRect, message)) {
				recenter();
			}
		}
    }

	private void recenter()
	{
		inverseInitialRotation = Quaternion.Inverse(ring.ringRotation);
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
