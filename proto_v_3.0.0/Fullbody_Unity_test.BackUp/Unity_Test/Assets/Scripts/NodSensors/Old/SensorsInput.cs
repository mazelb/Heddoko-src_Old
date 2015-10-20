using UnityEngine;
using System.Collections;

public class SensorsInput : MonoBehaviour 
{
	public GUIText guiText;
	
	#if UNITY_ANDROID
	AndroidJavaClass androidClass = null;
	AndroidJavaObject androidActivity = null;
	#endif

	// Use this for initialization
	void Start () 
	{
		#if UNITY_ANDROID
		AndroidJNI.AttachCurrentThread();
		androidClass = new AndroidJavaClass("com.Heddoko.HeddokoDemo.SensorUnityPlugin");
		androidActivity = androidClass.GetStatic<AndroidJavaObject>("mContext");
		#endif
	}

	public void HelloFromAndroid(string dataReceived) 
	{
		#if UNITY_ANDROID
		Debug.Log("Received data from Android plugin: " + dataReceived);
		guiText.text = dataReceived;
		#endif
	}

	// Update is called once per frame
	void Update () 
	{
		#if UNITY_ANDROID
		if (androidClass != null && androidActivity != null) 
		{
			androidActivity.Call("nonStaticMethod");
		}
		#endif
	}
}

/*using UnityEngine;
using System.Collections;

public class NativeBridge : MonoBehaviour {
	
	public GUIText guiText;
	
	#if UNITY_ANDROID
	AndroidJavaClass androidClass;
	#endif
	
	void Start () 
	{
		#if UNITY_ANDROID
		AndroidJNI.AttachCurrentThread();
		androidClass = new AndroidJavaClass("com.hello.world.UnityBridge");
		#endif
	}
	
	#if UNITY_ANDROID
	public void checkMyIntOnJava(string message){
		if (message == "READY") {
			string myInt = androidClass.CallStatic<int>("getMyInt");
			guiText.text = "My Int: " + myInt;
		}
	}
	#endif
	
	void Update()
	{		
		if (Input.GetMouseButtonDown (0)) {
			Debug.Log ("Pressed left click.");
			#if UNITY_ANDROID
			object[] args = new[] { Random.Range(1,100), this.gameObject.name, "checkMyIntOnJava" };
			androidClass.CallStatic("callbackToUnityMethod", args);
			#endif
		}
	}
}*/
