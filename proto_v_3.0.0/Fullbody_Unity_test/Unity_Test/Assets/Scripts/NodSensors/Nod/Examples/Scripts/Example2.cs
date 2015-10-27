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
using UnityEngine.UI;
using System.Collections;
using Nod;

public class NodGestureExample : NodExampleBase
{
	public UnityEngine.UI.Text gestureText;

	private const string MRUGesture = "Most Recent Gesture: ";

	public void Awake()
	{
		nodSubscribtionList = new NodSubscriptionType []
		{
			NodSubscriptionType.GestureMode,
			NodSubscriptionType.PointerMode
		};

		//This will create a GameObject in your Hierarchy called "NodController" which will manage
		//interactions with all connected nod devices.  It will presist between scene loads.  Only
		//one instance will be created if you request a nod interface from multiple locations
		//in your code.
		nod = NodController.GetNodInterface();
	}

	public void Start()
	{
		gestureText.text = MRUGesture;
	}

	public void Update()
	{
		if (!NodDeviceConnectedAndInitialized())
			return;

		//Example of applying the nod device orientation to the local transform.
		Vector3 updatePosition = new Vector3(0.001f * (float)nodDevice.position2D.x, -0.001f * nodDevice.position2D.y);
		transform.localPosition = transform.localPosition + updatePosition;

		if (nodDevice.gestureState != GestureEventType.NONE)
			gestureText.text = MRUGesture + nodDevice.gestureState.ToString();
	}

	void OnGUI()
	{
		BaseNodOnGUI();
	}
}
