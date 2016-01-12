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
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.IO;
using Nod;

public class NodMultipleNodDeviceExHelper : MonoBehaviour
{
	public int deviceID = 0;
    public Transform objectTransform; 

	private NodController nod;
	private NodDevice nodDevice = null;
	private bool nodDeviceConnected = false;
    private Quaternion vlocalRotation = Quaternion.identity;
    private Quaternion inverseInitialRotation = Quaternion.identity;
    private Quaternion initialRotation = Quaternion.identity;
    public Vector3 initialRotationEuler = Vector3.zero;
    public Vector3 currentRotationEuler = Vector3.zero;
    private Quaternion rotation90Y = Quaternion.identity;
    private Quaternion inverseRotation90Y = Quaternion.identity;
    private Quaternion difRotation90Y = Quaternion.identity;
    private Vector3 difRotation90Yeuler = Vector3.zero;
    public Vector3 rotation90Yeuler = Vector3.zero;
    private Quaternion rotation90X = Quaternion.identity;
    private Quaternion inverseRotation90X = Quaternion.identity;
    private Quaternion difRotation90X = Quaternion.identity;
    private Vector3 difRotation90Xeuler = Vector3.zero;
    public Vector3 rotation90Xeuler = Vector3.zero;
    //private Quaternion inverseInitialRotation2 = Quaternion.identity;
    //public Vector3 inverseInitialRotationEuler = Vector3.zero;
    //public Vector3 inverseInitialRotation2Euler = Vector3.zero;
    //public Quaternion difRotation2Euler = Quaternion.identity;
    

    private bool NodDeviceConnectedAndInitialized()
	{
		if (!nodDeviceConnected) {
			//Nod device connections happen asynchronously on mobile devices, check each frame for a connected nod device
			int numNodDevicesPaired = nod.getNumDevices();
			if (numNodDevicesPaired > deviceID)
            {
				nodDevice = nod.getNodDevice(deviceID);
				if (null == nodDevice)
					return false;
				nodDevice.Subscribe(NodSubscriptionType.EulerMode);
				nodDevice.Subscribe(NodSubscriptionType.ButtonMode);
				//recenter();
				nodDeviceConnected = true;
			} else
				return false;
		}

		return true;
	}

	void Start ()
	{
		nod = NodController.GetNodInterface();

		timer = new Stopwatch();
		timer.Start();
	}

	void OnDisable()
	{
		if (null == nodDevice)
			return;

		nodDevice.Unsubscribe(NodSubscriptionType.EulerMode);
		nodDevice.Unsubscribe(NodSubscriptionType.ButtonMode);
	}
    /**
    * NodQuaternionOrientation struct 
    * @brief A quaternion structure that is needed to render orientation data 
    */
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

    /// <summary>
    /// Eulers to quaternion.
    /// </summary>
    /// <returns>The to quaternion.</returns>
    /// <param name="pitch">Pitch.</param>
    /// <param name="roll">Roll.</param>
    /// <param name="yaw">Yaw.</param>
    public static NodQuaternionOrientation eulerToQuaternion(float pitch, float roll, float yaw)
    {
        float sinHalfYaw = Mathf.Sin(yaw / 2.0f);
        float cosHalfYaw = Mathf.Cos(yaw / 2.0f);
        float sinHalfPitch = Mathf.Sin(pitch / 2.0f);
        float cosHalfPitch = Mathf.Cos(pitch / 2.0f);
        float sinHalfRoll = Mathf.Sin(roll / 2.0f);
        float cosHalfRoll = Mathf.Cos(roll / 2.0f);

        NodQuaternionOrientation result;
        result.x = -cosHalfRoll * sinHalfPitch * sinHalfYaw
            + cosHalfPitch * cosHalfYaw * sinHalfRoll;
        result.y = cosHalfRoll * cosHalfYaw * sinHalfPitch
            + sinHalfRoll * cosHalfPitch * sinHalfYaw;
        result.z = cosHalfRoll * cosHalfPitch * sinHalfYaw
            - sinHalfRoll * cosHalfYaw * sinHalfPitch;
        result.w = cosHalfRoll * cosHalfPitch * cosHalfYaw
            + sinHalfRoll * sinHalfPitch * sinHalfYaw;

        return result;
    }

    Quaternion vQuaternionFactor = Quaternion.Euler(new Vector3(0, 0, 0));

    void Update ()
	{
        //x = yaw, y = roll, z = pitch
        if (!NodDeviceConnectedAndInitialized())
            return;
        nodDevice.CheckForUpdate();
        //Quaternion vQuatFinal = Quaternion.Euler(0, -nodDevice.eulerRotation.z * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.y * 180f / (float)Mathf.PI - 7.6f);
        Quaternion vQuatFinal = Quaternion.Euler(nodDevice.eulerRotation.x * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.z * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.y * 180f / (float)Mathf.PI);
        transform.localRotation = inverseInitialRotation * vQuatFinal;

        /*
        if (!NodDeviceConnectedAndInitialized())
			return;

        //Example of applying the nod device orientation to the local transform.
        nodDevice.CheckForUpdate();
        //NodQuaternionOrientation vQuat = eulerToQuaternion(nodDevice.eulerRotation.x - (7.6f * (float)Mathf.PI/180f), nodDevice.eulerRotation.z, nodDevice.eulerRotation.z);
        //Quaternion vQuatFinal = Quaternion.Euler(nodDevice.eulerRotation.z * 180f / (float)Mathf.PI, nodDevice.eulerRotation.x * 180f / (float)Mathf.PI, nodDevice.eulerRotation.y * 180f / (float)Mathf.PI - 7.6f);// new Quaternion(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

        //float angle = 0.0F;
        //Vector3 axis = Vector3.zero;
        //vQuatFinal.ToAngleAxis(out angle, out axis);
        //Quaternion vError = Quaternion.Inverse(Quaternion.FromToRotation(Vector3.up, axis));

        //NodQuaternionOrientation vQuat = eulerToQuaternion(0, nodDevice.eulerRotation.x , 0);
        Quaternion vQuatFinal = Quaternion.Euler(0, -nodDevice.eulerRotation.z * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.y * 180f / (float)Mathf.PI - 7.6f);
            // new Quaternion(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
        vlocalRotation = inverseInitialRotation * vQuatFinal;
        transform.rotation = vlocalRotation;// * Quaternion.Inverse(vQuaternionFactor);

        //Quaternion vlocalRotation = inverseInitialRotation * difRotation90Y * difRotation90X * nodDevice.rotation;
        //Quaternion vlocalRotation = inverseInitialRotation * nodDevice.rotation;
        //Quaternion vlocalRotation = inverseInitialRotation * Quaternion.AngleAxis(angle, new Vector3(0, 1, 0));
        //Quaternion vlocalRotation = nodDevice.rotation;
        //inverseInitialRotation2 = Quaternion.Inverse(nodDevice.rotation);
        //inverseInitialRotationEuler = inverseInitialRotation.eulerAngles;
        //inverseInitialRotation2Euler = inverseInitialRotation2.eulerAngles;
        //difRotation2Euler = inverseInitialRotation * inverseInitialRotation2;
        //transform.localRotation = difRotation2Euler;

        //transform.localRotation = vlocalRotation * difRotation90Y;

        //objectTransform = transform;
        //currentRotationEuler = nodDevice.rotation.eulerAngles;//*/
    }

    public void recenter()
    {
        //x = yaw, y = roll, z = pitch
        //Quaternion vQuatFinal = Quaternion.Euler(0, -nodDevice.eulerRotation.z * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.y * 180f / (float)Mathf.PI - 7.6f);
        Quaternion vQuatFinal = Quaternion.Euler(nodDevice.eulerRotation.x * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.z * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.y * 180f / (float)Mathf.PI);
        inverseInitialRotation = Quaternion.Inverse(vQuatFinal);

        /*
        //this.eulerRotation = new Vector3(eulerRotation.pitch, eulerRotation.roll, eulerRotation.yaw);
        //Quaternion vQuaternionFactor = Quaternion.Euler(new Vector3(0, 90, 0));
        //Quaternion vQuaternionFactor2 = Quaternion.Euler(new Vector3(90, 0, 0));
        //Quaternion vQuaternionFactor3 = Quaternion.Euler(new Vector3(0, 180, 180));

        //NodQuaternionOrientation vQuat = eulerToQuaternion(nodDevice.eulerRotation.x - (7.6f * (float)Mathf.PI / 180f), nodDevice.eulerRotation.y, nodDevice.eulerRotation.z);
        //Quaternion vQuatFinal = Quaternion.Euler(nodDevice.eulerRotation.z * 180f/(float)Mathf.PI, nodDevice.eulerRotation.x * 180f / (float)Mathf.PI, nodDevice.eulerRotation.y * 180f / (float)Mathf.PI - 7.6f);// new Quaternion(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

        //NodQuaternionOrientation vQuat = eulerToQuaternion(0, nodDevice.eulerRotation.x, 0);
        Quaternion vQuatFinal = Quaternion.Euler(0, -nodDevice.eulerRotation.z * 180f / (float)Mathf.PI, -nodDevice.eulerRotation.y * 180f / (float)Mathf.PI - 7.6f);
            //new Quaternion(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
        inverseInitialRotation = Quaternion.Inverse(vQuatFinal * Quaternion.Inverse(vQuaternionFactor));

        //inverseInitialRotation = Quaternion.Inverse(vQuatFinal * Quaternion.Inverse(vQuaternionFactor3) * Quaternion.Inverse(vQuaternionFactor2) * Quaternion.Inverse(vQuaternionFactor));
        //initialRotationEuler = nodDevice.rotation.eulerAngles;
        //inverseInitialRotation = Quaternion.Inverse(nodDevice.rotation);//*/
    }

    public void recenterRotate90Y()
    {
        rotation90Y = vlocalRotation;
        //inverseRotation90Y = Quaternion.Inverse(rotation90Y);
        //rotation90Yeuler = rotation90Y.eulerAngles;

        //Quaternion vlocalRotation = inverseInitialRotation * nodDevice.rotation;

        difRotation90Y = Quaternion.Inverse(vlocalRotation) * Quaternion.Euler(new Vector3(0,90,0));
        //difRotation90Yeuler = difRotation90Y.eulerAngles;
    }

    public void recenterRotate90X()
    {
        rotation90X = nodDevice.rotation;
        inverseRotation90X = Quaternion.Inverse(rotation90X);
        rotation90Xeuler = rotation90X.eulerAngles;

        Quaternion vlocalRotation = inverseInitialRotation * nodDevice.rotation;

        difRotation90X = Quaternion.Inverse(vlocalRotation) * Quaternion.Euler(new Vector3(90, 0, 0));
        difRotation90Xeuler = difRotation90X.eulerAngles;
    }

    public void resetAll()
    {
        difRotation90Y = Quaternion.identity;
        difRotation90X = Quaternion.identity;
        recenter();
    }

    private Stopwatch timer;
	private bool onTheClock = false;
	private bool done = false;
	private string msgPost = string.Empty;

	private long startTime;
	private long endTime;

	public string DeviceName()
	{
		if (null == nodDevice)
			return "";

		string result = nodDevice.GetNodDeviceName();
		result = "\nNodDeviceIndex: " + deviceID.ToString();

		Vector3 eulers = transform.localEulerAngles;
		if (!onTheClock && eulers.y > 25.0f && eulers.y < 180.0f) {
			onTheClock = true;
			startTime = timer.ElapsedMilliseconds;
			msgPost = "\nStart: " + timer.ElapsedMilliseconds;
		}
		if (!done && onTheClock && eulers.y > 80.0f && eulers.y < 180.0f) {
			done = true;
			endTime = timer.ElapsedMilliseconds;
			msgPost += "\nStop: " + timer.ElapsedMilliseconds;
			timer.Stop();
			msgPost += "\nDelta: " + (endTime - startTime).ToString();
		}

		return result + msgPost;
	}
}
