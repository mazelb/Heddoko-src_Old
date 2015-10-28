using UnityEngine;
using System.Collections;

public class FMSAngle : MonoBehaviour
{
	private static int mAngle = 0;

	public static void SetAngle(int vAngle) {
		mAngle = vAngle;
	}

	void OnGUI()
	{
		// Show current angle on screen.
		GUIStyle vStyle = new GUIStyle();
		vStyle.normal.textColor = Color.black;
		vStyle.fontSize = 50;
		GUI.Label(new Rect (70, 100, 100, 20), mAngle +"°", vStyle);
	}
}
