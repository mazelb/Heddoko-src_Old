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
	public enum positionName {Axilla, Clavicle, Deltoid, Elbow, Forearm, Knee, Trapezius};
    public positionName bodyPosition = positionName.Axilla;
	public string fullName = "";

	//
	// Thresholds for sensor data.
	//
	public Int32 minValue = 0;
	public Int32 maxValue = 9999;
//	public enum dataSource {};

	//
	// Filtering
	//
	public enum filtering {None, MovingAverage, WeightedAverage};
	public filtering filteringAlgorithm = filtering.WeightedAverage;
	private int mFilteringHistory = 10;
	private int[] mDataBuffer;

	//
	// CSV details.
	//
	public bool usingCSVFile = false;
	public bool overwriteMinMax = true;
	public string CSVFileName = "";
	public string CSVDataSet = "";
    private string mDefaultCSVFileName = "../../Data/empty.csv";
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

	public float getCurReading()
	{
	    float curReading = 0.0f;

		switch (filteringAlgorithm)
		{
		    case filtering.MovingAverage:
		        curReading = getReadingFromMovingAverage();
		        break;

		    case filtering.WeightedAverage:
		        curReading = getReadingFromWeightedAverage();
		        break;

		    default:
		        curReading = (float) mDataBuffer[0];
		        break;
		}

	    return curReading;
	}

	public float getReadingFromMovingAverage()
	{
	    // Make sure we have enough data to work with.
	    if (!isBufferReady()) {
	        return (float) mDataBuffer[0];
	    }

	    // Calculate the moving average, but leave the data buffer untouched.
	    int total = 0;
	    foreach (int value in mDataBuffer) {
	        total += value;
	    }

	    return total / mDataBuffer.Length;
	}

	public float getReadingFromWeightedAverage()
	{
	    // Make sure we have enough data to work with.
	    if (!isBufferLongEnough(1)) {
	        return (float) mDataBuffer[0];
	    }

        // Calculate weighted average and overwrite current data buffer value
		// and return floating point result instead of integer.
		mDataBuffer[0] = (int) (mDataBuffer[1] * 0.9f + mDataBuffer[0] * 0.1f);
		return mDataBuffer[1] * 0.9f + mDataBuffer[0] * 0.1f;
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
	    // Initialize mDataBuffer.
		mDataBuffer = new int[mFilteringHistory];
		for (int i = 0; i < mDataBuffer.Length; i++) {
		    mDataBuffer[i] = -1;
		}

		// Make sure filteringAvgHistory <= mCircularBufferSize
//		if (filteringAvgHistory > mCircularBufferSize) {
//		    filteringAvgHistory = mCircularBufferSize;
//		}
//
//		// Make sure filteringAvgHistory > 0
//		if (filteringAvgHistory < 1) {
//			filteringAvgHistory = 1;
//		}
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
	/// Reads the CSV data.
	/// </summary>
	private void readCSVData()
	{
	    // Use a data set specified by the container.
	    if (!String.IsNullOrEmpty(CSVDataSet) && String.IsNullOrEmpty(CSVFileName))
		{
	        CSVFileName = "../../Data/"+ CSVDataSet +"/"+ fullName +".csv";
	    }

		// Read from a specific data set.
		if (!String.IsNullOrEmpty(CSVFileName) && File.Exists(CSVFileName))
		{
		    print("Reading from "+ CSVFileName);
			mCSVStringValues = File.ReadAllLines(CSVFileName);
		}

		// Read from the default data set.
		else
		{
		    print("Reading from "+ mDefaultCSVFileName);
			mCSVStringValues = File.ReadAllLines(mDefaultCSVFileName);
		}

		populateCSValues();
	}

	/// <summary>
	/// Populates the CSV values.
	/// </summary>
	private void populateCSValues()
	{
	    if (overwriteMinMax) {
		    maxValue = minValue = Convert.ToInt32(mCSVStringValues[0]);
	    }

		Int32 vCurrentValue = 0;
		mCSVDataSize = 0;

		// Transform and find min and max.
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

	private void updateDataBuffer(int value)
	{
	    // Push buffer data down by one position.
        for (int i = mDataBuffer.Length - 2; i > -1; i--) {
            mDataBuffer[i + 1] = mDataBuffer[i];
        }

	    // Set current value in buffer.
	    mDataBuffer[0] = value;
	}

	private bool isBufferReady() {
	    return isBufferLongEnough(mDataBuffer.Length);
	}

	private bool isBufferLongEnough(int length)
	{
	    // Buffer should only have positive integers.
	    for (int i = 0; i < length; i++)
	    {
	        if (mDataBuffer[i] < 0) {
	            return false;
	        }
	    }

	    return true;
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
		    // Update buffer with incoming data from the COM port.
		    updateDataBuffer(StretchContainer.moduleData[(int) dataChannel]);
		} 
		else if (usingCSVFile && mCSValues.Count > 0)
		{
		    // Update buffer with current value in CSV data.
		    updateDataBuffer(mCSValues[mCurCSVDataIdx]);

		    // Update our CSV index.
            mCurCSVDataIdx++;
            if (mCurCSVDataIdx >= mCSValues.Count) {
                mCurCSVDataIdx = 0;
            }
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
		if (independentUpdate) {
			Reset();
			StartReading();
		}
	}
	
	/// <summary>
	/// Update is called once per frame
	/// </summary>
	void Update()
	{
		if (independentUpdate) {
			UpdateSensor();
		}
	}

}

