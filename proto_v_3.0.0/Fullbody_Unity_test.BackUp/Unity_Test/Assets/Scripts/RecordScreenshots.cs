using UnityEngine;
 
public class RecordScreenshots : MonoBehaviour
{
	// The folder we place all screenshots inside.
	// If the folder exists we will append numbers to create an empty folder.
	public string folderName = "StretchSense";
	public int frameRate = 25;
	public int sizeMultiplier = 1;
 
	// Directory where screenshots will be saved.
	private string mFullpath = "";
 
	void Start()
	{
		// Set the playback framerate
		Time.captureFramerate = frameRate;
 
		// Generate a folder name.
		int increment = 1;
		var mainDirectory = "../../Screenshots/"+ folderName;
		mFullpath = mainDirectory +"/1";
		while (System.IO.Directory.Exists(mFullpath)) {
			mFullpath = mainDirectory +"/"+ ++increment;
		}

		// Create the folder
		System.IO.Directory.CreateDirectory(mFullpath);
	}
 
	void Update()
	{
		// Filename for screenshot.
		var filename = string.Format("{0}/{1:D04}.png", mFullpath, Time.frameCount);
 
		// Capture the screenshot.
		Application.CaptureScreenshot(filename, sizeMultiplier);
	}
}
