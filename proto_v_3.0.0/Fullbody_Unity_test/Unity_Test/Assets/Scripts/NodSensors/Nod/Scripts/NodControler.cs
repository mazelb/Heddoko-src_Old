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

public class NodControler : MonoBehaviour
{
	private static NodControler nodControlerInstance;
	private static NodControlerInterface nodInterface;

	#region MonoBehaviour methods
	public void OnApplicationQuit()
	{
		nodInterface.ShutdownNodConnection();
	}
	#endregion

	public int getNumDevices()
	{
		if (null == nodInterface)
			return 0;
		return nodInterface.GetNumDevices();
	}

	public NodRing getRing(int index)
	{
		if (null == nodInterface) {
			Debug.Log("NodControler not initialized properly.");
			return null;
		}
		return nodInterface.GetRing(index);
	}

	public static NodControler GetNodInterface()
	{
		//If a NodControler has already been created return it.
		if (null != nodControlerInstance)
			return nodControlerInstance;

		GameObject nodGo = new GameObject("NodControler");
		nodControlerInstance = nodGo.AddComponent<NodControler>();

		//Prevent the interface from unloading when switching scenes
		DontDestroyOnLoad(nodGo);

		//Figure out what platform we are working with and create the appropriate interface

		//Android
		#if UNITY_ANDROID && !UNITY_EDITOR_WIN
		nodInterface = (NodControlerInterface) new NodControlerAndroidImp();
        #else
        //Everything else
        nodInterface = (NodControlerInterface) new NodControlerExternCImp();
        #endif

		if (nodInterface == null)
		{
			Debug.Log ("no interface created");
		}

		nodInterface.ConnectToNod();

		return nodControlerInstance;
	}
}
