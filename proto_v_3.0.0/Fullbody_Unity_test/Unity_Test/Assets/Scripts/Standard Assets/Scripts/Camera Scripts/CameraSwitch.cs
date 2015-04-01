using UnityEngine;
using System.Collections;

public class CameraSwitch : MonoBehaviour {

	Camera nodCamera = null;
	Camera StretchCamera = null;

	// Use this for initialization
	void Start () 
	{
		nodCamera = GameObject.Find("NodSensor Camera").camera;
		StretchCamera = GameObject.Find("NodSensor Camera").camera;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (Input.GetKeyDown(KeyCode.C)) 
		{
			if(Camera.main == nodCamera)
			{
				//Camera.main = StretchCamera;
			}
			else if(Camera.main == StretchCamera)
			{
				//Camera.main = nodCamera;
			}
		}
	}
}
