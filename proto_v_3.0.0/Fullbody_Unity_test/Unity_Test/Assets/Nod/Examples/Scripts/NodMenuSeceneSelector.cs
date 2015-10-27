using UnityEngine;
using System.Collections;

public class NodMenuSeceneSelector : MonoBehaviour
{
	public void OrientationButtonPressed()
	{
		Application.LoadLevel("Example_OrientationAndButtons");
	}

	public void GesturesButtonPressed()
	{
		Application.LoadLevel("Example_GesturesAndMouse");
	}

	public void JoystickButtonPressed()
	{
		Application.LoadLevel("Example_JoystickTrigger");
	}

	public void KeyboardButtonPressed()
	{
		Application.LoadLevel("Example_Keyboard");
	}

	public void MultipleDeviceButtonPressed()
	{
		Application.LoadLevel("Example_MultipleNodDevices");
	}

	public void AccelGyroButtonPressed()
	{
		Application.LoadLevel("Example_AccelAndGyro");
	}

	public void Update()
	{
		//So Android can exit out when the user hits the back button
		if (Input.GetKeyDown (KeyCode.Escape)) {
			Application.Quit();
		}
	}
}