using UnityEngine;
using System.Collections;
using Nod;

public class NodExampleBase : MonoBehaviour
{
	protected NodController nod;
	protected NodDevice nodDevice;
	protected bool nodDeviceConnected = false;
	protected NodSubscriptionType [] nodSubscribtionList;

	//0 for the first connected Nod device
	private const int nodDeviceID = 0;

	//Assume the hypotheticaly connected Nod device supports all requested subscription types until we hear otherwise.
	private bool deviceSupportsSubscriptionTypes = true;

	//This should get called every frame in update so you can put in things like
	//Android back button detection.
	protected bool NodDeviceConnectedAndInitialized()
	{
		if (Input.GetKeyDown(KeyCode.Escape)) {
			UnsubscribeToNod();
			Application.LoadLevel("NodExampleScenePicker");
		}

		if (nodDeviceConnected) {
			//Call this once per update to check for updated nod device values.
			nodDevice.CheckForUpdate();
		}

		if (!nodDeviceConnected) {
			//Nod device connections happen asynchronously on mobile devices, check each frame for a connected nod device
			int numNodDevicesPaired = nod.getNumDevices();
			if (numNodDevicesPaired > 0) {
				nodDevice = nod.getNodDevice(nodDeviceID);
				SubscribeToNod();
				nodDeviceConnected = true;
			} else
				return false;
		}

		return true;
	}

	protected void SubscribeToNod()
	{
		if (null == nodDevice) {
			Debug.Log ("Nod apis not initialized properly.");
			return;
		}

		//Everytime we try to subscribe reset our understanding of what the device supports
		deviceSupportsSubscriptionTypes = true;

		int subscribeCount = nodSubscribtionList.Length;
		for (int ndx = 0; ndx < subscribeCount; ndx++)
			deviceSupportsSubscriptionTypes &= nodDevice.Subscribe(nodSubscribtionList[ndx]);
	}

	protected void UnsubscribeToNod()
	{
		if (null == nodDevice) {
			Debug.Log ("Nod apis not initialized properly.");
			return;
        }

		int subscribeCount = nodSubscribtionList.Length;
		for (int ndx = 0; ndx < subscribeCount; ndx++)
			deviceSupportsSubscriptionTypes &= nodDevice.Unsubscribe(nodSubscribtionList[ndx]);
	}

	// Handle connection and subscription status display for OnGUI
	protected void BaseNodOnGUI()
	{
		//TODO PAW - Anush wanted me to put in a timer of say 4 seconds before showing error messages.

		if (!nodDeviceConnected) {
			Rect windowRect = new Rect(Screen.width/2f - Screen.width/8f,
			                           Screen.height/2f - Screen.height/8f,
			                           Screen.width/4f,
			                           Screen.height/4f);
			string message = "Unable to find a paired Nod devices.\nLoad the blue tooth settings for your\nmachine and make sure a Nod device is connected.";
			GUI.Window(0, windowRect, noConnectionWindow, message);
		}
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