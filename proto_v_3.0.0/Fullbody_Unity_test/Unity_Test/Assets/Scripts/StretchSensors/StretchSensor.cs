using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class StretchSensor : MonoBehaviour 
{
    //
    // Details about the sensor's position on the body.
    //
    public enum positionName {Forearm, Elbow, Knee};
    public positionName bodyPosition = positionName.Knee;
	public string fullName = "";

	//
	// Thresholds for sensor data.
	//
	public Int32 minValue = 0;
	public Int32 maxValue = 9999;

	//
	// Filtering
	//
	public enum filtering {None, MovingAverage, LowPass, HighPass};
	public filtering filteringAlgorithm = filtering.MovingAverage;
	public Int32 filteringAvgHistory = 5;

	//
	// CSV details.
	//
	public bool usingCSVFile = false;
	public bool overwriteMinMax = true;
	public string CSVFileName = "";
	public string CSVDataSet = "";
    private string mDefaultCSVFileName = "../../Data/default/default.csv";
	private string[] mCSVStringValues;
	private List<Int32> mCSValues = new List<Int32>();
	private Int32 mCSVDataSize = 0;
	private Int32 mCurCSVDataIdx = 0;

	//
	// COM port details.
	//
	public bool usingCOMPort = false;
	public enum Channel {One = 1, Two = 2, Three = 3, Four = 4, Five = 5};
	public Channel dataChannel = Channel.One;

	//
	// TODO: BLE details.
	//
	public bool usingBLE = false;
	
	// When True the stretch sensor updates independently,
	// otherwise the sensor only updates when the stretchJoint asks.
	public bool independentUpdate = false;
	
	// Circular buffer for data captured and filtering.
	private Int32[] mStretchValBuffer;
	private Int32 mCurCircularIdx = 0;
	private Int32 mCircularBufferSize = 20;

	// Current readings index
	private float mCurStretchAngle = 0.0f;

	public int stretchID = 0;

	/// <summary>
	/// Reads the CSV data.
	/// </summary>
	private void readCSVData()
	{
	    // Use a specific data set.
	    if (!String.IsNullOrEmpty(CSVDataSet) && String.IsNullOrEmpty(CSVFileName))
		{
	        CSVFileName = "..\\..\\Data\\"+ CSVDataSet +"\\"+ fullName +".csv";
	    }

		// Read and populate from a specific data set.
		if (!String.IsNullOrEmpty(CSVFileName))
		{
		    print("Reading from "+ CSVFileName);
			mCSVStringValues = File.ReadAllLines(CSVFileName);
			populateCSValues();
		}

		// Read and populate from a general data set.
		else
		{
		    // Retrieve default data.
		    print("Reading from "+ mDefaultCSVFileName);
			mCSVStringValues = File.ReadAllLines(@mDefaultCSVFileName);
		}
	}

	/// <summary>
	/// Populates the CSV values.
	/// </summary>
	private void populateCSValues()
	{
	    if (overwriteMinMax)
	    {
		    maxValue = minValue = Convert.ToInt32(mCSVStringValues[0]);
	    }
		
		Int32 vCurrentValue = 0;
		mCSVDataSize = 0;
		
		//transform and find min and max
		foreach (string vValue in mCSVStringValues)
		{
			vCurrentValue = Convert.ToInt32(vValue); 
			mCSValues.Add(vCurrentValue);

			if(vCurrentValue > maxValue && overwriteMinMax)
				maxValue = vCurrentValue;
			if(vCurrentValue < minValue && overwriteMinMax) 
				minValue = vCurrentValue;

			mCSVDataSize++;
		}
	}

	public float getCurReading()
	{
	    float curReading = 0.0f;

		if (usingBLE)
		{
			//
		}
		else if (usingCOMPort)
		{
			curReading = getCurReadingFromCOM();
		}
		else if (usingCSVFile)
		{
			curReading = getCurReadingFromCSV();
		}

		return curReading;
	}

	//
	public float getCurReadingFromCSV()
	{
		float vSum = 0.0f;

		if (mStretchValBuffer.Length > 0)
		{
			for (int i = 0; i < filteringAvgHistory; i++) 
			{
				int vCurIdx = mCurCircularIdx - i;

				if (vCurIdx < 0) 
				{
					vCurIdx = (mStretchValBuffer.Length - 1 ) + vCurIdx;
				}

				if (vCurIdx < mStretchValBuffer.Length)
				{
					vSum += mStretchValBuffer[vCurIdx];
				}
			}
		}

		return vSum / filteringAvgHistory;
	}

	// 
	public float getCurReadingFromCOM()
	{
	    return (float) StretchContainer.moduleData[(int) dataChannel];
	}

	//
	// Map data to numbers that are easy to work with.
	//
	public float getMappedReading(int mapTo)
	{
		return mapTo * (getCurReading() - minValue) / (maxValue - minValue);
	}

	//
	// Try to catch mis-configurations before they throw errors.
	//
	public void performanceCheck()
	{
		// Make sure filteringAvgHistory <= mCircularBufferSize
		if (filteringAvgHistory > mCircularBufferSize) {
		    filteringAvgHistory = mCircularBufferSize;
		}

		// Make sure filteringAvgHistory > 0
		if (filteringAvgHistory < 1) {
			filteringAvgHistory = 1;
		}
	}
	
	/// <summary>
	/// Reset the sensors data to init status.
	/// </summary>
	public void Reset() 
	{
		mCSValues.Clear ();
		//mCSVStringValues.Initialize ();

		mStretchValBuffer = new int[mCircularBufferSize];
		for (int i=0; i < mCircularBufferSize; i++) 
		{
			mStretchValBuffer[i] = 0;
		}

		mCurCircularIdx = 0; 
		mCSVDataSize = 0; 
		mCurCircularIdx = 0; 
		mCurCSVDataIdx = 0;
	}
	
	/// <summary>
	/// Starts reading from designated source.
	/// </summary>
	public void StartReading()
	{
	    // Run some quick checks.
		performanceCheck();

		if (usingBLE)
		{
			StartReadingBLE();
		} 
		else if (usingCOMPort)
		{
			StartReadingCOM();
		} 
		else if (usingCSVFile)
		{
			StartReadingCSV();
		}
	}
	
	//
	public void StartReadingBLE()
	{
		//TODO
	}
	
	//
	public void StartReadingCOM()
	{
		// See StretchContainer
	}

	//
	public void StartReadingCSV()
	{
		readCSVData();
		populateCSValues();
	}

	/// <summary>
	/// Call this function to update current sensor values.
	/// </summary>
	public void UpdateSensor () 
	{
		if (usingBLE) 
		{
			//TODO
			if(mCurCircularIdx > mCircularBufferSize)
			{
				mCurCircularIdx = 0;
			}
		} 
		else if (usingCOMPort) 
		{
		    //
		} 
		else if (usingCSVFile) 
		{
			if (mCSValues.Count > 0)
			{
				mStretchValBuffer[mCurCircularIdx] = mCSValues[mCurCSVDataIdx];
				mCurCircularIdx++;
				mCurCSVDataIdx++;

				//Update indexes for next update
				if (mCurCSVDataIdx >= mCSValues.Count) 
				{
					mCurCSVDataIdx = 0;
				}

				if(mCurCircularIdx >= mCircularBufferSize)
				{
					mCurCircularIdx = 0;
				}
			}
		}
		else 
		{
			// NO DATA TO UPDATE !!
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
		mStretchValBuffer = new int[mCircularBufferSize];
	}
	
	/// <summary>
	/// Use this for initialization
	/// </summary>
	void Start() 
	{
		if(independentUpdate)
		{
			Reset();
			StartReading();
		}
	}
	
	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update()
	{
		if(independentUpdate)
		{
			UpdateSensor();
		}
	}

}

