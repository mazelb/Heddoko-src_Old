using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class StretchContainer : MonoBehaviour 
{
	//each joint can be composed of one or multiple sensors simultaneously
	private StretchJoint[] mStretchJoints;

	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints () 
	{
		for (int ndx = 0; ndx < mStretchJoints.Length; ndx++) 
		{
			if(!mStretchJoints[ndx].independantUpdate)
			{
				mStretchJoints[ndx].StartJoint();
			}
		}
	}
	
	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public void UpdateJoints () 
	{
		for (int ndx = 0; ndx < mStretchJoints.Length; ndx++) 
		{
			if(!mStretchJoints[ndx].independantUpdate)
			{
				mStretchJoints[ndx].UpdateJoint();
			}
		}
	}
	
	/// <summary>
	/// Reset the stretch joint sensors.
	/// </summary>
	public void ResetJoints ()
	{
		for (int ndx = 0; ndx < mStretchJoints.Length; ndx++) 
		{
			if(!mStretchJoints[ndx].independantUpdate)
			{
				mStretchJoints[ndx].ResetJoint();
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
	//////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Awake this instance.
	/// </summary>
	void Awake ()
	{
		Application.targetFrameRate = 300;
		mStretchJoints = GetComponentsInChildren<StretchJoint>();
	}
	
	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start() 
	{
		ResetJoints();
		StartJoints();
	}
	
	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update() 
	{
		UpdateJoints();
	}
	
	/// <summary>
	/// GUI Update.
	/// </summary>
	void OnGUI()
	{
		if (GUI.Button (new Rect (20, 20, 300, 100), "Start StretchSensors"))
		{
			ResetJoints();
			StartJoints();
		}

		if (GUI.Button (new Rect (320, 20, 300, 100), "Reset StretchSensors "))
		{			
			ResetJoints();        
		}
	}
}
