using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using Nod;
using UnityEngine.UI;

public class NodKeyboard : NodExampleBase
{
	public Transform cursorTrans;
	public Text messageBody;
	public Text topMatches;

	private const string nodKeyboardPredictionURL = "http://api.nod.com:8888/predictions/2";
	private ArrayList cookie = new ArrayList();
	private ArrayList predictions = new ArrayList();

	private List<Vector2> predictionEnginePath = new List<Vector2>();
	private LineRenderer lineRenderer;
    private string savedText = "";
	private float furthestDistanceTraveled = 0.0f;
	private bool isTyping = false;

	//Dimensions the word prediction engine expects
	private const float imageWidth = 600.0f;
	private const float imageHeight = 450.0f;

	//Valid range x=[0, imageWidth], and y=[0,imageHeight]
	//Start in the center of that range
	private Vector2 cursorPosition = new Vector2(imageWidth / 2.0f, imageHeight / 2.0f);
	private Vector2 previousCursorPos = Vector2.zero;

	private bool previousTactile0 = false;
	private bool previousTactile1 = false;
	private int currentPredictionIndex = 0;

	private bool capLocksDown = false;

	private BoxCollider [] buttonBoundingBoxes;

	public void Awake()
	{
		nodSubscribtionList = new NodSubscriptionType []
		{
			NodSubscriptionType.PointerMode,
			NodSubscriptionType.ButtonMode
		};

		messageBody.text = "";
		topMatches.text = "";
		lineRenderer = GetComponent<LineRenderer>();
		buttonBoundingBoxes = GetComponentsInChildren<BoxCollider>(true);

		nod = NodController.GetNodInterface();
		RestTrackedData();
	}

	public IEnumerator requestWordPredictions(ArrayList jsonCoords)
	{
		WWWForm form = new WWWForm();

		Debug.Log("Request entered.");

		string encodedCookie = MiniJSON.JsonEncode(cookie);
		string encodedCoordinates = MiniJSON.JsonEncode(jsonCoords);
		form.AddField("cookie", encodedCookie);
		form.AddField("coords", encodedCoordinates);
		WWW download = new WWW(nodKeyboardPredictionURL, form);
		yield return download;

		//If you never get to this point it means we can't access the nodKeyboardPredictionURL URL
		//Email "cd@nod-labs.com" and ask him to reboot the keyboard web server for the Unity keyboard sample app
		Debug.Log("Request returned with error response: " + download.error);

		string downloadText = download.text;
		if (string.IsNullOrEmpty(download.error)) {
			Hashtable result = (Hashtable)MiniJSON.JsonDecode(downloadText);
			predictions = (ArrayList)result["predictions"];
			cookie = (ArrayList)result["cookie"];
			if (predictions.Count > 0 && !predictions[0].Equals("&#x0000;")) {
				savedText = messageBody.text;
				if (messageBody.text.Length > 0)
					messageBody.text += " ";
				messageBody.text += predictions[0];

				string predictionWords = "";
				for (int ndx = 0; ndx < predictions.Count - 1; ndx++)
					predictionWords += predictions[ndx].ToString() + ", ";
				predictionWords += predictions[predictions.Count - 1].ToString();

				topMatches.text = predictionWords;
			}
		}
	}

	private void UpdateCursorPosition()
	{
		const float nodPose2DScaleFactor = 0.6f;

		cursorPosition.x += nodPose2DScaleFactor * (float)nodDevice.position2D.x;
		cursorPosition.x = Mathf.Clamp(cursorPosition.x, 0, imageWidth);

		cursorPosition.y += nodPose2DScaleFactor * (float)nodDevice.position2D.y;
		cursorPosition.y = Mathf.Clamp(cursorPosition.y, 0, imageHeight);

		//X,Y range in world space in the scene for the keyboard extents = +/- 4.9
		//Coordinates of the keyboard are X=[0, 600], and Y=[0,450]
		const float worldSpaceWidth = 4.9f;
		const float worldSpaceHeight = 4.9f;
		float xRatio = cursorPosition.x / imageWidth;
		float yRatio = cursorPosition.y / imageHeight;
		float mappedX = xRatio * -worldSpaceWidth + (1.0f - xRatio) * worldSpaceWidth;
		float mappedY = yRatio * worldSpaceHeight + (1.0f - yRatio) * -worldSpaceHeight;

		//Give it a slight offset in Y so its above the plane of the keyboard
		cursorTrans.localPosition = new Vector3(mappedX, 0.1f, mappedY);
	}

	private void UpdateCursorTrail()
	{
		//Don't add the same point
		if (previousCursorPos != cursorPosition) {
			predictionEnginePath.Add(cursorPosition);
			lineRenderer.SetVertexCount(predictionEnginePath.Count);
			lineRenderer.SetPosition(predictionEnginePath.Count - 1, cursorTrans.localPosition);

			float distanceFromStart = Vector2.Distance(predictionEnginePath[0], cursorPosition);
			if (distanceFromStart > furthestDistanceTraveled)
				furthestDistanceTraveled = distanceFromStart;
        }
		previousCursorPos = cursorPosition;
    }

	private void ProcessCompletedCursorPath()
	{
		//Half the width of a single button
		const float minTravelDistanceForWords = (imageWidth / 10.0f) / 2.0f;

		//If the distance covered is less then half one letter press the letter we are under instead of doing word prediction
		if (furthestDistanceTraveled < minTravelDistanceForWords) {
			ProcessSingleButtonPress();
		} else {
			//A more sustainable approach would be to integrate List<Vector2> support into MiniJSON
			//but for now work within its existing entry points.
			ArrayList jsonCoords = new ArrayList();
			foreach (Vector2 vec in predictionEnginePath) {
				Hashtable newObject = new Hashtable();
				newObject.Add("x", vec.x);
				newObject.Add("y", vec.y);
				jsonCoords.Add(newObject);
			}
			StartCoroutine(requestWordPredictions(jsonCoords));
		}

		RestTrackedData();
    }

	private void ProcessSingleButtonPress()
	{
		NodKeyboardButton button = null;
		foreach (BoxCollider collider in buttonBoundingBoxes) {
			if (collider.bounds.Contains(cursorTrans.position)) {
				button = collider.GetComponent<NodKeyboardButton>();
				break;
			}
		}
		if (null == button)
			return;

		if (button.isAlphaNumeric) {
			if (capLocksDown)
				messageBody.text += Char.ToUpper(button.letter);
			else
				messageBody.text += button.letter;
		} else {
			switch (button.functionKey) {
			case NodKeyboardButton.FunctionKey.BackSpace:
				if (messageBody.text.Length >= 1)
					messageBody.text = messageBody.text.Remove(messageBody.text.Length - 1);
				break;
			case NodKeyboardButton.FunctionKey.Enter:
				messageBody.text += "\n";
				break;
			case NodKeyboardButton.FunctionKey.Symbols:
				break;
			case NodKeyboardButton.FunctionKey.Shift:
				capLocksDown = !capLocksDown;
				break;
			default:
				break;
			}
		}
	}

	private void RestTrackedData()
	{
		predictionEnginePath = new List<Vector2>();
		lineRenderer.SetVertexCount(0);
		furthestDistanceTraveled = 0.0f;
		previousTactile0 = false;
		previousTactile1 = false;
		currentPredictionIndex = 0;
	}

	void Update()
	{
		if (!NodDeviceConnectedAndInitialized())
			return;

		UpdateCursorPosition();

		if (nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Touch2)) {
			isTyping = true;
		}

		if (isTyping && !nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Touch2)) {
			isTyping = false;
			ProcessCompletedCursorPath();
		}

		if (isTyping)
			UpdateCursorTrail();

		if (predictions.Count > 0) {
			bool wordChanged = false;
			if (nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Tactile0) && !previousTactile0) {
				currentPredictionIndex++;
				if (currentPredictionIndex >= predictions.Count)
					currentPredictionIndex = 0;
				wordChanged = true;
			}
			if (nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Tactile1) && !previousTactile1) {
				currentPredictionIndex--;
				if (currentPredictionIndex < 0)
					currentPredictionIndex = predictions.Count - 1;
				wordChanged = true;
			}

			if (wordChanged) {
				if (savedText.Length > 0)
					messageBody.text = (savedText + " " + predictions[currentPredictionIndex]);
				else
					messageBody.text = predictions[currentPredictionIndex].ToString();
			}

			previousTactile0 = nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Tactile0);
			previousTactile1 = nodDevice.GetNodButton(Nod.ButtonIDs.Ring.Tactile1);
		}
	}

	public void OnGUI()
	{
		BaseNodOnGUI();
	}
}