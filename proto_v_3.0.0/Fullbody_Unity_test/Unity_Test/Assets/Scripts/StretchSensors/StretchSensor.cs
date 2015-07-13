/**
 * @file StretchSensor.cs
 * @brief Describe what this script does.
 * @author Francis Amankrah (frank@heddoko.com)
 * @date June 2015
 */
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;

public class StretchSensor : MonoBehaviour 
{
    /**
     * Details about the sensor.
     */
    public enum PositionName {Axilla, Clavicle, Deltoid, Elbow, Forearm, Knee, Trapezius};
    public PositionName vPosition = PositionName.Axilla;
    public string       vFullName = "";

    /**
     * Thresholds for sensor data.
     */
    public Int32        vMinValue = 0;
    public Int32        vMaxValue = 9999;
    //	public enum dataSource {};

    /**
     * Smoothing settings.
     */
    public SmoothingAlgorithm vSmoothing = SmoothingAlgorithm.WeightedAverage;
    private int         mSmoothingHistory = 20;
    private int[]       maBuffer;

    public enum SmoothingAlgorithm {None, MovingAverage, WeightedAverage};

    /*
     * CSV details.
     */
    public bool         vUsingCSVFile   = false;
    public bool         vOverwriteThresholds = true;
    public string       vCSVFilename    = "";
    public string       mCSVDataSet     = "";
    private string      mDefaultCSVFilename = "../../Data/empty.csv";
    private string[]    maCSVStrings;
    private List<Int32> maCSVData       = new List<Int32>();
    private Int32       mCSVSize        = 0;
    private Int32       mCSVCounter     = 0;

    /*
     * COM port details.
     */
    public bool         vUsingCOMPort   = false;
    public DataChannel  vCOMChannel     = DataChannel.One;

    public enum DataChannel {One = 1, Two = 2, Three = 3, Four = 4, Five = 5};

    /*
    * TODO: BLE details.
    */
    public bool vUsingBLE = false;

    /*
    * When True the stretch sensor updates independently,
    * otherwise the sensor only updates when the ss_joint asks.
    */
    public bool vIndependentUpdate = false;

    // Current readings index
    private float mCurStretchAngle = 0.0f;
  
  
    /**
     * usage(const char *progName)
     * @brief ...
     * @param progName
     * @return -1 for failure
     */
    public float GetReading()
    {
        float vReading = 0.0f;

        switch (vSmoothing)
        {
            case SmoothingAlgorithm.MovingAverage:
                vReading = GetMovingAverage();
                break;

            case SmoothingAlgorithm.WeightedAverage:
                vReading = GetWeightedAverage();
                break;

            default:
                vReading = (float) maBuffer[0];
                break;
        }

        return vReading;
    }
  
    /**
     * GetMovingAverage()
     * @brief ...
     * @return float
     */
    public float GetMovingAverage()
    {
        // Make sure we have enough data to work with.
        if (!IsBufferReady())
        {
            return (float) maBuffer[0];
        }

        // Calculate the moving average, but leave the data maBuffer untouched.
        int vTotal = 0;
        foreach (int value in maBuffer)
        {
            vTotal += value;
        }

        return vTotal / maBuffer.Length;
    }
  
    /**
     * GetWeightedAverage()
     * @brief ...
     * @return float
     */
    public float GetWeightedAverage()
    {
        // Make sure we have enough data to work with.
        if (!IsBufferLongEnough(10))
        {
            return (float) maBuffer[0];
        }

        // Calculate weighted average and overwrite current data buffer value.
        maBuffer[0] = (int) (maBuffer[1] * 0.9f + maBuffer[0] * 0.1f);
        return maBuffer[1] * 0.9f + maBuffer[0] * 0.1f;
    }
  
    /**
     * GetMappedReading()
     * @brief Map data to numbers that are easy to work with.
     * @return float
     */
    public float GetMappedReading(int vMapTo)
    {
        return vMapTo * GetFractionalReading();
    }
  
    /**
     * GetFractionalReading()
     * @brief Map data to the range [0, 1].
     * @return float
     */
    public float GetFractionalReading()
    {
        return (GetReading() - vMinValue) / (vMaxValue - vMinValue);
    }

	/**
     * Try to catch mis-configurations before they throw errors.
     */
    public void RunChecks()
    {
        // Initialize maBuffer.
        maBuffer = new int[mSmoothingHistory];

        for (int i = 0; i < maBuffer.Length; i++)
        {
            maBuffer[i] = -1;
        }
    }

    /**
     * usage(const char *progName)
     * @brief ...
     * @param progName
     * @return -1 for failure
     */
    public void reset() 
    {
        maCSVData.Clear();
        //maCSVStrings.Initialize ();

        mCSVSize = 0;
        mCSVCounter = 0;
    }
  
    /**
     * usage(const char *progName)
     * @brief ...
     * @param progName
     * @return -1 for failure
     */
    public void start_reading()
    {
        // Run some quick checks.
        RunChecks();

        if (vUsingBLE)
        {
            StartReadingBLE();
        }

        else if (vUsingCOMPort)
        {
            StartReadingCOM();
        }

        else if (vUsingCSVFile)
        {
            StartReadingCSV();
        }
    }
  
    /**
     * usage(const char *progName)
     * @brief TODO
     * @param progName
     * @return -1 for failure
     */
    public void StartReadingBLE() {}
  
    /**
     * usage(const char *progName)
     * @brief ...
     * @param progName
     * @return -1 for failure
     */
    public void StartReadingCOM()
    {
    	// See StretchContainer
    }
  
    /**
     * usage(const char *progName)
     * @brief ...
     * @param progName
     * @return -1 for failure
     */
    public void StartReadingCSV()
    {
        read_csv_data();
        populate_maCSVData();
    }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return void
   */
	private void read_csv_data()
	{
    // Use a data set specified by the container.
    if (!String.IsNullOrEmpty(mCSVDataSet) && String.IsNullOrEmpty(vCSVFilename)) {
      vCSVFilename = "../../Data/"+ mCSVDataSet +"/"+ vFullName +".csv";
    }

		// Read from a specific data set.
		if (!String.IsNullOrEmpty(vCSVFilename) && File.Exists(vCSVFilename)) {
      print("Reading from "+ vCSVFilename);
      maCSVStrings = File.ReadAllLines(vCSVFilename);
    }

		// Read from the default data set.
		else {
      print("Reading from "+ mDefaultCSVFilename);
      maCSVStrings = File.ReadAllLines(mDefaultCSVFilename);
		}

		populate_maCSVData();
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	private void populate_maCSVData()
	{
    if (vOverwriteThresholds) {
	    vMaxValue = vMinValue = Convert.ToInt32(maCSVStrings[0]);
    }

		Int32 val = 0;
		mCSVSize = 0;

		// Transform and find min and vMaxValue.
		foreach (string str in maCSVStrings)
		{
			val = Convert.ToInt32(str);
			maCSVData.Add(val);

			if (val > vMaxValue && vOverwriteThresholds) {
				vMaxValue = val;
      }

			if(val < vMinValue && vOverwriteThresholds) {
				vMinValue = val;
      }

			mCSVSize++;
		}
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	private void update_buffer(int value)
	{
    // Push buffer data down by one position.
    for (int i = maBuffer.Length - 2; i > -1; i--) {
      maBuffer[i + 1] = maBuffer[i];
    }

    // Set current value in buffer.
    maBuffer[0] = value;
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	private bool IsBufferReady() {
    return IsBufferLongEnough(maBuffer.Length);
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	private bool IsBufferLongEnough(int length)
	{
    // Buffer should only have positive integers.
    for (int i = 0; i < length; i++) {
      if (maBuffer[i] < 0) {
        return false;
      }
    }

    return true;
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	public void update_sensor () 
	{
		if (vUsingBLE) {
			// TODO
		}

    else if (vUsingCOMPort) {

      // Update buffer with incoming data from the COM port.
            update_buffer(StretchContainer.svaModuleData[(int) vCOMChannel]);
		}

    else if (vUsingCSVFile && maCSVData.Count > 0) {
      
      // Update buffer with current value in CSV data.
	    update_buffer(maCSVData[mCSVCounter]);

	    // Update our CSV index.
      mCSVCounter++;
      if (mCSVCounter >= maCSVData.Count) {
        mCSVCounter = 0;
      }
		}
	}



	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
  //////////////////////////////////////////////////////////////////////////////////////



  /**
   * Awake()
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	void Awake() {}
  
  /**
   * Start()
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	void Start()
	{
		if (vIndependentUpdate) {
			reset();
			start_reading();
		}
  }

  /**
   * Update()
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	void Update()
	{
		if (vIndependentUpdate) {
			update_sensor();
		}
	}
}

