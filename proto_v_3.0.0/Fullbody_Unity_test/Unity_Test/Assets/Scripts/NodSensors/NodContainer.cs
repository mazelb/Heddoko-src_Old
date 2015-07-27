using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic; 
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class NodContainer : MonoBehaviour 
{
	//each joint can be composed of one or multiple sensors simultaneously
	private static NodJoint[] mNodJoints;

	// This variable is used to specify the angle information of which part of body be shown on the screen
	public static float vKey;


	/// <summary>
	/// Call this function to start reading data from the sensors for the joint values.
	/// </summary>
	public void StartJoints () 
	{
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].StartJoint();
			}
		}
	}
	
	/// <summary>
	/// Call this function to update current Joint values.
	/// </summary>
	public void UpdateJoints () 
	{
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].UpdateJoint();
			}
		}
	}
	
	/// <summary>
	/// Reset the stretch joint sensors.
	/// </summary>
	public void ResetJoints ()
	{
		for (int ndx = 0; ndx < mNodJoints.Length; ndx++) 
		{
			if(!mNodJoints[ndx].independantUpdate)
			{
				mNodJoints[ndx].ResetJoint();
			}
		}
	}



	//	/ <summary>
	//	/ Provides the Torso Orientation for other joints 
	//	/ </summary>
	public static float [,] GetTorsoOrientation ()
	{
		return mNodJoints [0].ReturnTorsoOrientation();
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
		mNodJoints = GetComponentsInChildren<NodJoint>();
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
		if (GUI.Button (new Rect (20, 70, 200, 50), "Start Sensors"))
		{
			ResetJoints();
			StartJoints();
		}

		if (GUI.Button (new Rect (220, 70, 200, 50), "Reset Sensors "))
		{			
			ResetJoints();        
		}


		if (GUI.Button (new Rect (880, 550, 120 , 25), "Thoracolumbar"))
		{			
			vKey = 1;        
		}

		if (GUI.Button (new Rect (1005, 550, 110 , 25), "Right Arm"))
		{			
			vKey = 2;        
		}


		if (GUI.Button (new Rect (1120, 550, 110 , 25), "Left Arm"))
		{			
			vKey = 3;         
		}


		if (GUI.Button (new Rect (1235, 550, 110 , 25), "Right Leg"))
		{			
			vKey = 4;        
		}

		if (GUI.Button (new Rect (1350, 550, 110 , 25), "Left Leg"))
		{			
			vKey = 5;         
		}


//		string[] names = new string[] {"Matt", "Joanne", "Robert", "kazem"};
//		GUI.SelectionGrid (new Rect (1000, 500, 200, 40), 0, names , 2);
//		//GUI.Label (new Rect (1000, 520, 150, 50), "angle: " + Time.time.ToString ());
//
//
//
//		float progress = Time.time * 0.05f;
//		var progressBarEmpty = new Texture2D(0,0);
//		var progressBarFull = new Texture2D(0,0);
//
//
//		GUI.BeginGroup (new Rect (20, 40, 400, 20));
//		GUI.Box (new Rect (0,0,  400, 20),progressBarEmpty);
//
//		// draw the filled-in part:
//		GUI.BeginGroup (new Rect (200 - (400 * progress), 0, 400 * progress, 20f));
//		GUI.Box (new Rect (0,0,  400, 20),progressBarFull);
//		GUI.EndGroup ();
//
//		GUI.EndGroup ();
//
	}
}
