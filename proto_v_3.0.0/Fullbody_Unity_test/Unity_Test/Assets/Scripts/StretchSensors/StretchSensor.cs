using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class StretchSensor : MonoBehaviour 
{
	public int stretchID = 0;
	public String stretchName = "Sensor999";

	//When True the stretch sensor updates independently
	//otherwise the sensor only updates when the stretchJoint asks
	public Boolean independentUpdate = false; 

	/*
	 * Input source: CSV
	 */
	public Boolean usingCSVFile = false;
	public Boolean overwriteMinMax = true;
	public String CSVFileName = "";
	public String mCSVDataSet = "";
    private String defaultCSVFileName = "../../Data/default/default.csv";
	private string[] mCSVStringValues;
	private List<Int32> mCSValues = new List<Int32>();
	private Int32 mCSVDataSize = 0;
	private Int32 mCurCSVDataIdx = 0;

	/*
	 * Input source: COM port
	 */
	public Boolean usingCOMPort = false;
	public enum Channel {One = 1, Two = 2, Three = 3, Four = 4, Five = 5};
	public Channel stretchSenseDataChannel = Channel.One;

	/*
	 * TODO: Input source: BLE
	 */
	public Boolean usingBLE = false;

	/*
	 * Min - Max
	 */
	public Int32 maxStretchVal = Int32.MaxValue;
	public Int32 minStretchVal = Int32.MinValue;
	public float minAngleVal = 0; 
	public float maxAngleVal = 360;

	//data smoothing
	public Int32 filteringAvgHistory = 5;
	
	//circular buffer for data captured and filtering
	private Int32[] mStretchValBuffer;
	private Int32 mCurCircularIdx = 0;
	private Int32 mCircularBufferSize = 20;

	//Current readings index
	private float mCurStretchAngle = 0.0f;

	/// <summary>
	/// Reads the CSV data.
	/// </summary>
	private void readCSVData()
	{
	    // Use a specific data set
	    if (!String.IsNullOrEmpty(mCSVDataSet) && String.IsNullOrEmpty(CSVFileName))
	    {
	        CSVFileName = "../../Data/"+ mCSVDataSet +"/"+ stretchName +".csv";
	    }

		if (!String.IsNullOrEmpty(CSVFileName))
		{
		    print("Reading from "+ CSVFileName);
			mCSVStringValues = File.ReadAllLines(CSVFileName);
			populateCSValues();
		} 
		else 
		{
		    print("Reading from "+ defaultCSVFileName);
			mCSVStringValues = File.ReadAllLines(@defaultCSVFileName);
		}
	}

	/// <summary>
	/// Populates the CSV values.
	/// </summary>
	private void populateCSValues()
	{
	    if (overwriteMinMax)
	    {
		    maxStretchVal = minStretchVal = Convert.ToInt32(mCSVStringValues[0]);
	    }
		
		Int32 vCurrentValue = 0;
		mCSVDataSize = 0;
		
		//transform and find min and max
		foreach (string vValue in mCSVStringValues)
		{
			vCurrentValue = Convert.ToInt32(vValue); 
			mCSValues.Add(vCurrentValue);

			if(vCurrentValue > maxStretchVal && overwriteMinMax)
				maxStretchVal = vCurrentValue;
			if(vCurrentValue < minStretchVal && overwriteMinMax) 
				minStretchVal = vCurrentValue;

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
		// TODO: This condition should always be TRUE: mCircularBufferSize >= mFilteringAvgHistory
		// So put a limit on the data entered in the Editor 1

		float vSum = 0.0f;

		if(mStretchValBuffer.Length > 0)
		{
			for(int i=0; i < filteringAvgHistory; i++) 
			{
				int vCurIdx = mCurCircularIdx - i;

				if(vCurIdx < 0) 
				{
					vCurIdx = (mStretchValBuffer.Length - 1 ) + vCurIdx;
				}

				if(vCurIdx < mStretchValBuffer.Length)
				{
					vSum += mStretchValBuffer[vCurIdx];
				}
			}
		}

		return vSum / filteringAvgHistory;
	}

	/// <summary>
	/// Gets the current raw Unfiltered reading.
	/// </summary>
	/// <returns>The current raw reading.</returns>
	public Int32 getCurRawReadingFromCSV()
	{
		return mStretchValBuffer [mCurCircularIdx];
	}

	public float getCurReadingFromCOM()
	{
	    return (float) StretchContainer.mData[(int) stretchSenseDataChannel];
	}

	/// <summary>
	/// Gets the current filtered resulting angle of the sensor reading.
	/// </summary>
	/// <returns>The current angle reading.</returns>
	public float getCurAngleReading()
	{
		mCurStretchAngle = mapRange(minStretchVal, maxStretchVal, minAngleVal, maxAngleVal, getCurReading());
		return mCurStretchAngle;
	}

	/// <summary>
	/// UTIL map values to a range.
	/// </summary>
	private float mapRange(float a1,float a2,float b1,float b2,float s)
	{
		return b1 + (s-a1)*(b2-b1)/(a2-a1);
	}

	/// <summary>
	/// Sets the min and max values for angle conversion.
	/// </summary>
	/// <param name="vMin">minimum raw value.</param>
	/// <param name="vMax">max raw value.</param>
	/// <param name="vAngleMin">angle minimum.</param>
	/// <param name="vAngleMax">angle max.</param>
//	public void SetupAngleConversion(Int32 vMin, Int32 vMax, float vAngleMin, float vAngleMax)
//	{
//		maxStretchVal = vMax;
//		minStretchVal = vMin;
//		maxAngleVal = vAngleMax;
//		minAngleVal = vAngleMin;
//	}
	
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

