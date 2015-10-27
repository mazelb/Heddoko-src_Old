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

	namespace ButtonIDs
	{
		class Ring
		{
			public const int Touch0 = 0;
			public const int Touch1 = 1;
			public const int Touch2 = 2;
			public const int Tactile0 = 3;
			public const int Tactile1 = 4;
		}

		class Backspin
		{
			public const int A = 0;
			public const int B = 1;
			public const int X = 2;
			public const int Y = 3;
			public const int Logo = 4; // Button immediately above the left and right buttons with nod logo on it.
			public const int Bumper = 5;
			public const int Joystick = 6;
			public const int Left = 7;
			public const int Right = 8;
			public const int Grip = 9; // Red button on the side that says "nod"
		}

		//TODO PAW - add a Backspin enumeration, for now just use integers (different backspins have different number of buttons)
	}

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

	[StructLayout(LayoutKind.Sequential)]
	public struct NodAccel
	{
		public float accelX;
		public float accelY;
		public float accelZ;

		public NodAccel(float _accelX, float _accelY, float _accelZ)
		{
			accelX = _accelX;
			accelY = _accelY;
			accelZ = _accelZ;
		}
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct NodGyro
	{
		public float gyroX;
		public float gyroY;
		public float gyroZ;

		public NodGyro(float _gyroX, float _gyroY, float _gyroZ)
		{
			gyroX = _gyroX;
			gyroY = _gyroY;
			gyroZ = _gyroZ;
		}
	};

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

	public enum NodSubscriptionType {
		ButtonMode = 1,
		AccelMode = 2,
		EulerMode = 3,
		GameControlMode = 4,
		GestureMode = 5,
		PointerMode = 6,
		SliderMode = 7,
		DataMode = 8,
		TranslationMode = 9,
		GyroMode = 10
	};

	public class NodUtilities
	{
		#region Nod Plugin DLL Imports
		#if UNITY_STANDALONE_WIN || UNITY_EDITOR || UNITY_ANDROID
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
		public static extern IntPtr NodGetRingName(int deviceID);

		[DllImport(strNodLib)]
		public static extern int NodRequestBatteryPercentage(int deviceID);
		[DllImport(strNodLib)]
		public static extern int NodGetBatteryPercentage(int deviceID);

		[DllImport(strNodLib)]
		public static extern bool NodSubscribe(int mode, int deviceID);
		[DllImport(strNodLib)]
		public static extern bool NodUnsubscribe(int mode, int deviceID);

		[DllImport(strNodLib)]
		public static extern int NodGetButtons(int deviceID);
		[DllImport(strNodLib)]
		public static extern int NodGetButtonState(int deviceID, int button);

		[DllImport(strNodLib)]
		public static extern NodEulerOrientation NodGetEulerOrientation(int deviceID);
		[DllImport(strNodLib)]
		public static extern NodQuaternionOrientation NodGetQuaternionOrientation(int deviceID);
		[DllImport(strNodLib)]
		public static extern int NodGetGesture(int deviceID);
		[DllImport(strNodLib)]
		public static extern NodPosition2D NodGetPosition2D(int deviceID);

		[DllImport(strNodLib)]
		public static extern NodAccel NodGetAccel(int deviceID);
		[DllImport(strNodLib)]
		public static extern NodGyro NodGetGyro(int deviceID);

		[DllImport(strNodLib)]
		public static extern bool NodSubscribeToGameControl(int deviceID);
		[DllImport(strNodLib)]
		public static extern bool NodUnSubscribeToGameControl(int deviceID);

		[DllImport(strNodLib)]
		public static extern NodPosition2D NodGetGamePosition(int deviceID);
		[DllImport(strNodLib)]
		public static extern int NodGetTrigger(int deviceID);

		#endregion
	}
} // end Nod namespace