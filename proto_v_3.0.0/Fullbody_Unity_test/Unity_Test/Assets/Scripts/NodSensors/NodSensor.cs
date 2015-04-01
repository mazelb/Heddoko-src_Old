using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using Nod;
using System.Diagnostics;
using System.IO;

public class NodSensor : MonoBehaviour 
{
	//TODO: add the skeleton mapping here 

	public int ringID = 0;
	private NodControler nod;
	private NodRing ring = null;
	private bool nodRingConnected = false;
	private Quaternion inverseInitialRotation = Quaternion.identity;

	private bool RingConnectedAndInitialized()
	{
		if (!nodRingConnected) {
			//Ring connections happen asynchronously on mobile devices, check each frame for a connected ring
			int numRingsPaired = nod.getNumDevices();
			if (numRingsPaired > ringID) {
				ring = nod.getRing(ringID);
				if (null == ring)
					return false;
				ring.SubscribeToPose6D();
				ring.SubscribeToButton();
				recenter();
				nodRingConnected = true;
			} else 
				return false;
		}
		
		return true;
	}

	void Start () 
	{
		nod = NodControler.GetNodInterface();

		timer = new Stopwatch();
		timer.Start();
	}

	void OnDisable()
	{
		if (null == ring)
			return;
		
		ring.UnsubscribeToPose6D();
		ring.UnsubscribeToButton();
	}

	void Update () 
	{	
		if (!RingConnectedAndInitialized())
			return;

		//Call this once per update to check for updated ring values.
		ring.CheckForUpdate();
		
		//Example of applying the rings orientation to the local transform.
		transform.localRotation = inverseInitialRotation * ring.ringRotation;
	}
	
	public void recenter()
	{
		inverseInitialRotation = Quaternion.Inverse(ring.ringRotation);
	}

	private Stopwatch timer;
	private bool onTheClock = false;
	private bool done = false;
	private string msgPost = string.Empty;

	private long startTime;
	private long endTime;

	public string RingName()
	{
		if (null == ring)
			return "";

		string result = ring.GetRingName();
		result = "\nRingIndex: " + ringID.ToString();

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
