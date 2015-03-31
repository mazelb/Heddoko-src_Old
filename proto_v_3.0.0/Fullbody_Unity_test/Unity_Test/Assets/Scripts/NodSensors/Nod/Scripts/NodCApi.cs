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
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// Wraps access to C API so we can pass structures between DLL's

namespace Nod {
		
	[StructLayout(LayoutKind.Sequential)]
	public struct NodEulerOrientation
	{
		public float pitch;
		public float roll;
		public float yaw;

		public NodEulerOrientation(float p, float r, float y)
		{
			pitch = p;
			roll = r;
			yaw = y;
		}
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct NodQuaternionOrientation
	{
		public float x;
		public float y;
		public float z;
		public float w;
		
		public NodQuaternionOrientation(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct NodPosition2D
    {
		public int x;
		public int y;

		public NodPosition2D(int _x, int _y)
		{
			x = _x;
			y = _y;
		}
	};

	//This isn't for crossing DLL boundaries,
	public struct NodButtons
	{
		public bool touch0;
		public bool touch1;
		public bool touch2;
		public bool tactile0;
		public bool tactile1;
	}

	public enum GestureEventType {
		NONE = -1,
		SWIPE_DOWN = 0,
		SWIPE_LEFT = 1,
		SWIPE_RIGHT = 2,
		SWIPE_UP = 3,
		CW = 4,
		CCW = 5,
		SLIDER_LEFT = 6,
		SLIDER_RIGHT = 7
	};

	public enum NodDeviceModes {
		MODE_TTM = 0,
		MODE_GAMEPAD = 1,
		MODE_3D = 2
	};
	
	public class NodUtilities 
	{
		#region Nod Plugin DLL Imports
		#if UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN || UNITY_ANDROID
		private const string strNodLib = "NodPlugin";
		#elif UNITY_IPHONE
		private const string strNodLib = "__Internal";
		#else
		private const string strNodLib = "NodPlugin";
		#endif
		
		[DllImport(strNodLib)]
		public static extern bool NodInitialize();
		[DllImport(strNodLib)]
		public static extern bool NodShutdown();
		
		[DllImport(strNodLib)]
		public static extern int NodNumRings();
		[DllImport(strNodLib)]
		public static extern IntPtr NodGetRingName(int ringID);

		//Do not call
		/*
		[DllImport(strNodLib)]
		public static extern bool NodSetMode(int ringID, int mode);
		*/
		
		[DllImport(strNodLib)]
		public static extern bool NodSubscribeToButton(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodSubscribeToPose6D(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodSubscribeToGesture(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodSubscribeToPosition2D(int ringID);

		[DllImport(strNodLib)]
		public static extern bool NodUnsubscribeToButton(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodUnsubscribeToPose6D(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodUnsubscribeToGesture(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodUnsubscribeToPosition2D(int ringID);
		
		[DllImport(strNodLib)]
		public static extern int NodGetButtonState(int ringID);
		[DllImport(strNodLib)]
		public static extern NodEulerOrientation NodGetEulerOrientation(int ringID);
		[DllImport(strNodLib)]
		public static extern NodQuaternionOrientation NodGetQuaternionOrientation(int ringID);
		[DllImport(strNodLib)]
		public static extern int NodGetGesture(int ringID);	
		[DllImport(strNodLib)]
		public static extern NodPosition2D NodGetPosition2D(int ringID);

		[DllImport(strNodLib)]
		public static extern float NodBatteryPercent(int ringID);

		[DllImport(strNodLib)]
		public static extern  int NodReadMode(int ringID);
		[DllImport(strNodLib)]
		public static extern  bool NodShutdownRing(int ringID);

		[DllImport(strNodLib)]
		public static extern IntPtr NodCurrentFirmwareVersion(int ringID);
		[DllImport(strNodLib)]
		public static extern IntPtr NodDeviceAddress(int ringID);

		[DllImport(strNodLib)]
		public static extern bool NodRecalibrate(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodRecenter(int ringID);
		[DllImport(strNodLib)]
		public static extern bool NodFlipY(int ringID);

		#endregion	
	}
} // end Nod namespace