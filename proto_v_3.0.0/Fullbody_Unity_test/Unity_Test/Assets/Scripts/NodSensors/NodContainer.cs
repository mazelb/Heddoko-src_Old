using UnityEngine;
using System.Collections;
using Nod;

public class NodContainer : MonoBehaviour 
{
	private NodSensor [] rings;

	void Awake() 
	{
		rings = GetComponentsInChildren<NodSensor>();
		for (int ndx = 0; ndx < rings.Length; ndx++) {
			rings[ndx].ringID = ndx;
		}
	}

	// Use this for initialization
	void Start() 
	{
	
	}
	
	// Update is called once per frame
	void Update() 
	{
		if (Input.GetKeyDown(KeyCode.Space)) {
			for (int ndx = 0; ndx < rings.Length; ndx++) {
				rings[ndx].recenter();
			}
		}
	}

	void OnGUI()
	{
		//Show the name of the ring over the joint the NodBone is attached to
		Camera cam = Camera.current;
		if (null == cam)
			return;

		foreach (NodSensor ring in rings) {
			string msg = ring.RingName();

			Vector3 ringWorldPos = ring.transform.position;
			Vector3 pos = cam.WorldToScreenPoint(ringWorldPos);
			GUI.Label(new Rect(pos.x, Screen.height - pos.y, 150, 150), msg);
		}
	}
}
