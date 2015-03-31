using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class StretchSensor
{
	//Input source: CSV file
	private Boolean mUseCSVFile = false;
	private String 	mCSVFileName = "";
	private string[] mCSVValues;

	//TODO: Input source: COM port
	private String 	mCOMport = ""; 
	private Int32 	mBaudeRate = 9600;
	private SerialPort mPortStream = null; 

	//TODO: Input source: BLE
	
	//Min - Max
	private Int32 mMaxVal;
	private Int32 mMinVal;

	//data smoothing
	public Boolean useSmoothing = false;
	public Int32 filteringAvgHistory = 5;
	
	private Boolean mIsInitialized = false;

	private Int32 mCurDataIdx = 0;
	private Int32 mDataSize = 0;
	
	//data captured
	private List<Int32> mStretchValues = new List<Int32>(); 
	private List<float> mStretchAngles = new List<float>();
	 
	
	//Arms transforms
	public Transform rightUpperArmTransform = null;
	public Transform rightForeArmTransform = null;
	public Transform leftUpperArmTransform = null;
	public Transform leftForeArmTransform = null;
	
	//Torso transforms
	public Transform upperSpineTransform = null;
	public Transform lowerSpineTransform = null;
	
	//Legs transforms
	public Transform rightThighTransform = null;
	public Transform rightCalfTransform = null;
	public Transform leftCalfTransform = null;
	public Transform leftThighTransform = null;
	
	//Current Angles
	private Vector3 rightForeArmCurrentEulers;
	
	
	private float mapRange(float a1,float a2,float b1,float b2,float s)
	{
		return b1 + (s-a1)*(b2-b1)/(a2-a1);
	}
	
	private void readCSVData()
	{
		if (!String.IsNullOrEmpty (CSVFileName)) 
		{
			mCSVValues = File.ReadAllLines(CSVFileName);
			populateCSVValues();
		} 
		else 
		{
			mCSVValues = File.ReadAllLines(@"..\..\..\..\..\Data\data_stretch_default.csv");
		}
	}
	
	private void readFromStream()
	{
		if (!String.IsNullOrEmpty(COMport)) 
		{
			mPortStream = new SerialPort(COMport, BaudeRate);
		}
	}
	
	private void populateCSVValues()
	{
		mMaxVal = mMinVal = Convert.ToInt32(mCSVValues[0]);
		
		Int32 vCurrentValue = 0;
		mDataSize = 0;
		
		//transform and find min and max
		foreach (string vValue in mCSVValues)//for (int i=0; i < mCSVValues.GetLength(); i++) 
		{
			vCurrentValue = Convert.ToInt32(vValue); 
			mStretchValues.Add(vCurrentValue);
			if(vCurrentValue > mMaxVal)
				mMaxVal = vCurrentValue;
			if(vCurrentValue < mMinVal) 
				mMinVal = vCurrentValue;
			mDataSize++;
		}
	}
	
	void Awake() 
	{
		Application.targetFrameRate = 600;
	}
	
	// Use this for initialization
	void Start () 
	{
		if (!mIsInitialized) 
		{
			mIsInitialized = true;
			mCurDataIdx = 0;
			mDataSize = 0; 
			
			if(useCSVFile)
			{
				readCSVData();
				populateCSVValues();
			}
			else
			{
				readFromStream();
				//TODO
			}
		}
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (mCurDataIdx >= 0 && mCurDataIdx < mStretchValues.Count) 
		{
			if(mCurDataIdx >= (filteringAvgHistory-1) && useSmoothing) 
			{
				float vSum = 0.0f;
				
				for(int i=0; i < filteringAvgHistory; i++) 
				{
					vSum += mStretchValues[mCurDataIdx - i];
				}
				
				float vResult = vSum / filteringAvgHistory;
				mStretchAngles.Add(mapRange(mMinVal, mMaxVal, MinAngle, MaxAngle, vResult));
			}
			else 
			{
				mStretchAngles.Add(mapRange(mMinVal, mMaxVal, MinAngle, MaxAngle, mStretchValues[mCurDataIdx]));
			}
			
			rightForeArmCurrentEulers.x = rightForeArmCurrentEulers.z = 0;
			rightForeArmCurrentEulers.y = -mStretchAngles[mCurDataIdx];
			rightForeArmTransform.localRotation = Quaternion.Euler (rightForeArmCurrentEulers);
			mCurDataIdx++;
		} 
		else 
		{
			mCurDataIdx = 0;
		}
	}

}

