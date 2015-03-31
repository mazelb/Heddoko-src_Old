using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class StretchSensor
{
	//Input source: CSV file
	public Boolean mUsingCSVFile = false;
	public String mCSVFileName = "";
	private string[] mCSVStringValues;
	private List<Int32> mCSValues = new List<Int32>(); 
	private Int32 mCSVDataSize = 0;
	private Int32 mCurCSVDataIdx = 0;

	//TODO: Input source: COM port
	public Boolean mUsingCOMPort = false;
	public String mCOMport = ""; 
	public Int32 mBaudeRate = 9600;
	private SerialPort mPortStream = null; 

	//TODO: Input source: BLE
	public Boolean mUsingBLE = false;

	//Min - Max
	public Int32 mMaxVal;
	public Int32 mMinVal;
	public float mMinAngleVal; 
	public float mMaxAngleVal;

	//data smoothing
	private Int32 mFilteringAvgHistory = 5;
	
	//circular buffer for data captured and filtering
	private List<Int32> mStretchValBuffer = new List<Int32>(); 
	private Int32 mCurDataIdx = 0;
	private Int32 mCurCircularIdx = 0; 
	private Int32 mCircularBufferSize = 20;

	//Current readings index
	private float mCurStretchAngle = 0.0f;	 

	/// <summary>
	/// UTIL map values to a range.
	/// </summary>
	private float mapRange(float a1,float a2,float b1,float b2,float s)
	{
		return b1 + (s-a1)*(b2-b1)/(a2-a1);
	}

	/// <summary>
	/// Reads the CSV data.
	/// </summary>
	private void readCSVData()
	{
		if (!String.IsNullOrEmpty (mCSVFileName)) 
		{	
			mCSVStringValues = File.ReadAllLines(mCSVFileName);
			populateCSVValues();
		} 
		else 
		{
			mCSVStringValues = File.ReadAllLines(@"..\..\..\..\..\Data\data_stretch_default.csv");
		}
	}

	/// <summary>
	/// Populates the CSV values.
	/// </summary>
	private void populateCSVValues()
	{
		mMaxVal = mMinVal = Convert.ToInt32(mCSVStringValues[0]);
		
		Int32 vCurrentValue = 0;
		mCSVDataSize = 0;
		
		//transform and find min and max
		foreach (string vValue in mCSVStringValues)
		{
			vCurrentValue = Convert.ToInt32(vValue); 
			mCSValues.Add(vCurrentValue);

			if(vCurrentValue > mMaxVal)
				mMaxVal = vCurrentValue;
			if(vCurrentValue < mMinVal) 
				mMinVal = vCurrentValue;

			mCSVDataSize++;
		}
	}

	/// <summary>
	/// Inits the COM stream.
	/// </summary>
	private void initCOMStream()
	{
		if (!String.IsNullOrEmpty(mCOMport)) 
		{
			mPortStream = new SerialPort(mCOMport, mBaudeRate);
		}
	}

	/// <summary>
	/// Gets the current raw Unfiltered reading.
	/// </summary>
	/// <returns>The current raw reading.</returns>
	public Int32 getCurRawReading()
	{
		return mStretchValBuffer [mCurCircularIdx];
	}

	/// <summary>
	/// Gets the current filtered reading.
	/// </summary>
	/// <returns>The current reading.</returns>
	public float getCurReading()
	{
		// TODO: This condition should always be TRUE: mCircularBufferSize >= mFilteringAvgHistory
		// So put a limit on the data entered in the Editor 1

		float vSum = 0.0f;

		for(int i=0; i < mFilteringAvgHistory; i++) 
		{
			int vCurIdx = mCurCircularIdx - i;

			if(vCurIdx < 0) 
			{
				vCurIdx = mStretchValBuffer.Count + vCurIdx;
			}

			if(vCurIdx < mStretchValBuffer.Count)
			{
				vSum += mStretchValBuffer[mCurCircularIdx - i];
			}
		}

		return vSum / mFilteringAvgHistory;
	}

	/// <summary>
	/// Gets the current filtered resulting angle of the sensor reading.
	/// </summary>
	/// <returns>The current angle reading.</returns>
	public float getCurAngleReading()
	{
		return mapRange(mMinVal, mMaxVal, mMinAngleVal, mMaxAngleVal, getCurReading());
	}

	/// <summary>
	/// Sets the min and max values for angle conversion.
	/// </summary>
	/// <param name="vMin">minimum raw value.</param>
	/// <param name="vMax">max raw value.</param>
	/// <param name="vAngleMin">angle minimum.</param>
	/// <param name="vAngleMax">angle max.</param>
	public void SetupAngleConversion(Int32 vMin, Int32 vMax, float vAngleMin, float vAngleMax)
	{
		mMaxVal = vMax;
		mMinVal = vMin;
		mMaxAngleVal = vAngleMax;
		mMinAngleVal = vAngleMin;
	}
	
	/// <summary>
	/// Reset this instance.
	/// </summary>
	public void Reset() 
	{
		mCurDataIdx = 0;
		mCurCircularIdx = 0; 
		mCSVDataSize = 0; 
		mCurCircularIdx = 0; 
		mUsingCSVFile = false;
		mUsingCOMPort = false;
		mUsingBLE = false;
	}

	public void StartReadingCSV(String vFileName)
	{
		readCSVData();
		populateCSVValues();
	}

	public void StartReadingCOM(String vCOMPort, Int32 vBaudeRate)
	{
		initCOMStream();
		//TODO
	}

	public void StartReadingBLE()
	{
		//TODO
	}

	/// <summary>
	/// Call this function to update current sensor values.
	/// </summary>
	public void UpdateSensor () 
	{
		if (mUsingBLE) 
		{
			//TODO
			if(mCurCircularIdx > mCircularBufferSize)
			{
				mCurCircularIdx = 0;
			}
		} 
		else if (mUsingCOMPort) 
		{
			//TODO
			if(mCurCircularIdx > mCircularBufferSize)
			{
				mCurCircularIdx = 0;
			}
		} 
		else if (mUsingCSVFile) 
		{
			mStretchValBuffer.Add(mCSValues[mCurCSVDataIdx]);
			mCurCircularIdx++;
			mCurCSVDataIdx++;

			//Update indexes for next update
			if (mCurCSVDataIdx > mCSValues.Count) 
			{
				mCurCSVDataIdx = 0;
			}

			if(mCurCircularIdx > mCircularBufferSize)
			{
				mCurCircularIdx = 0;
			}
		}
		else 
		{
			// NO DATA TO UPDATE !!
		}
	}

}

