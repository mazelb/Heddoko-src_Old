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

public class NodMultipleNodDeviceExample : MonoBehaviour
{
	private NodMultipleNodDeviceExHelper [] nodDevices;

	void Awake()
	{
		nodDevices = GetComponentsInChildren<NodMultipleNodDeviceExHelper>();
		for (int ndx = 0; ndx < nodDevices.Length; ndx++) {
			nodDevices[ndx].deviceID = ndx;
		}
	}

	// Update is called once per frame
	void Update()
	{
		if (Input.GetKeyDown(KeyCode.Escape)) {
			Application.LoadLevel("NodExampleScenePicker");
        }

		if (Input.GetKeyDown(KeyCode.Space)) {
			for (int ndx = 0; ndx < nodDevices.Length; ndx++) {
				nodDevices[ndx].recenter();
			}
		}

        if (Input.GetKeyDown(KeyCode.A))
        {
            for (int ndx = 0; ndx < nodDevices.Length; ndx++)
            {
                nodDevices[ndx].recenterRotate90Y();
            }
        }

        if (Input.GetKeyDown(KeyCode.S))
        {
            for (int ndx = 0; ndx < nodDevices.Length; ndx++)
            {
                nodDevices[ndx].recenterRotate90X();
            }
        }

        if (Input.GetKeyDown(KeyCode.R))
        {
            for (int ndx = 0; ndx < nodDevices.Length; ndx++)
            {
                nodDevices[ndx].resetAll();
            }
        }

    }

    void OnGUI()
	{
		//Show the name of the nod device
		Camera cam = Camera.current;
		if (null == cam)
			return;

		foreach (NodMultipleNodDeviceExHelper device in nodDevices) {
			string msg = device.DeviceName();

			Vector3 nodDeviceWorldPos = device.transform.position;
			Vector3 pos = cam.WorldToScreenPoint(nodDeviceWorldPos);
			GUI.Label(new Rect(pos.x, Screen.height - pos.y, 150, 150), msg);
		}
	}
}
