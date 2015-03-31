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

public class Example4 : MonoBehaviour 
{
	private Example4Ring [] rings;

	void Awake() 
	{
		rings = GetComponentsInChildren<Example4Ring>();
		for (int ndx = 0; ndx < rings.Length; ndx++) {
			rings[ndx].ringID = ndx;
		}
	}

	// Use this for initialization
	void Start() 
	{
	
	}
	
	// Update is called once per frame
	void Update() 
	{
		if (Input.GetKeyDown(KeyCode.Space)) {
			for (int ndx = 0; ndx < rings.Length; ndx++) {
				rings[ndx].recenter();
			}
		}
	}

	void OnGUI()
	{
		//Show the name of the ring over the joint the NodBone is attached to
		Camera cam = Camera.current;
		if (null == cam)
			return;

		foreach (Example4Ring ring in rings) {
			string msg = ring.RingName();

			Vector3 ringWorldPos = ring.transform.position;
			Vector3 pos = cam.WorldToScreenPoint(ringWorldPos);
			GUI.Label(new Rect(pos.x, Screen.height - pos.y, 150, 150), msg);
		}
	}
}
