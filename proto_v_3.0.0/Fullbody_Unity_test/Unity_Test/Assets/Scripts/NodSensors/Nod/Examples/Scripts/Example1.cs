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
using UnityEngine.UI;
using Nod;

public class NodOrientationExample : NodExampleBase
{
	//Rotation to get the Nod device from where it started to where it should be once we recenter
	private Quaternion inverseInitialRotation = Quaternion.identity;

	public void Awake()
	{
		nodSubscribtionList = new NodSubscriptionType []
		{
			NodSubscriptionType.EulerMode,
			NodSubscriptionType.ButtonMode
		};

		//This will create a GameObject in your Hierarchy called "NodController" which will manage
		//interactions with all connected nod devices.  It will presist between scene loads.  Only
		//one instance will be created if you request a nod interface from multiple locations
		//in your code.
		nod = NodController.GetNodInterface();
	}

	public void Update()
	{
		if (!NodDeviceConnectedAndInitialized())
			return;

		UpdateUGUIDisplay();

		if (Input.GetKeyDown(KeyCode.Space))
			recenter();

		//Example of applying the nod devices orientation to the local transform.
		transform.localRotation = inverseInitialRotation * nodDevice.rotation;
	}

	public void ResetOrientatinButtonPressed()
	{
		recenter();
	}

	private void recenter()
	{
		inverseInitialRotation = Quaternion.Inverse(nodDevice.rotation);
	}

	public Text [] uGUILabels;

	private string [] ringButtonNames = {"Touch0", "Touch1", "Touch2", "Tactile0", "Tactile1"};

	private void UpdateUGUIDisplay()
	{
		//Once the device is connected display the pressed status of each button
		if (nodDeviceConnected) {
			//Display the status of each button
			string [] buttonPressStatus = {
				nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Touch0) ? "Down" : "Up",
				nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Touch1) ? "Down" : "Up",
				nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Touch2) ? "Down" : "Up",
				nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Tactile0) ? "Down" : "Up",
				nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Tactile1) ? "Down" : "Up"
			};

			for (int ndx = 0; ndx < ringButtonNames.Length; ndx++) {
				uGUILabels[ndx].text = ringButtonNames[ndx] + "\n" + buttonPressStatus[ndx];
			}
		}
	}

	public void OnGUI()
	{
		//Deal with displaying error conditions
		BaseNodOnGUI();
    }
}