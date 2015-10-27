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

public class NodController : MonoBehaviour
{
	protected static NodController nodControllerInstance;
	protected static NodControllerInterface nodInterface;

	#region MonoBehaviour methods
	public void OnApplicationQuit()
	{
		nodInterface.ShutdownNodConnection();
	}
	void OnApplicationFocus(bool focusStatus)
	{
		//Debug.Log ("Application focus changed to: " + focusStatus.ToString());
		nodInterface.ApplicationFocusChanged(focusStatus);
	}

	void OnLevelWasLoaded(int levelNum)
	{
		nodInterface.ClearData();
	}
	#endregion

	public int getNumDevices()
	{
		if (null == nodInterface)
			return 0;
		return nodInterface.GetNumDevices();
	}

	public NodDevice getNodDevice(int index)
	{
		if (null == nodInterface) {
			Debug.Log("NodController not initialized properly.");
			return null;
		}
		return nodInterface.GetNodDevice(index);
	}

	public static NodController GetNodInterface()
	{
		//If a NodController has already been created return it.
		if (null != nodControllerInstance)
			return nodControllerInstance;

		GameObject nodGo = new GameObject("NodController");
		nodControllerInstance = nodGo.AddComponent<NodController>();

		//Prevent the interface from unloading when switching scenes
		DontDestroyOnLoad(nodGo);

		//Figure out what platform we are working with and create the appropriate interface

		//Android
		#if UNITY_ANDROID && !UNITY_EDITOR
		nodInterface = (NodControllerInterface) new NodControllerAndroidImp();
        #else
        //Everything else
        nodInterface = (NodControllerInterface) new NodControllerExternCImp();
        #endif

		if (nodInterface == null)
		{
			Debug.Log ("no interface created");
		}

		nodInterface.ConnectToNod();

		return nodControllerInstance;
	}
}
