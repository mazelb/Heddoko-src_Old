/**
 * @file stretch_sensor.cs
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
    public PositionName vPosition = PositionName.Axilla;
    public string       vFullName = "";

    public enum PositionName {Axilla, Clavicle, Deltoid, Elbow, Forearm, Knee, Trapezius};

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
    public string       vDataSet        = "";
    private string      mDefaultCSVFilename = "../../Data/empty.csv";
    private string[]    maCSVStrings;
    private List<Int32> maCSVData       = new List<Int32>();
    private Int32       mCSVSize        = 0;
    private Int32       mCSVCounter     = 0;

    /*
     * COM port details.
     */
    public bool vUsingCOMPort           = false;
    public DataChannel  vCOMChannel     = DataChannel.One;

    public enum DataChannel {One = 1, Two = 2, Three = 3, Four = 4, Five = 5};

    /*
     * TODO: BLE details.
     */
    public bool vUsingBLE = false;

    /*
     * When True the stretch sensor updates independently,
     * otherwise the sensor only updates when the SSJoint asks.
     */
    public bool vIndependentUpdate = false;

    /**
     * @brief           Gets the current reading for the sensor and applies the selected smoothing algorithm (if any).
     * @return float    Current reading for the sensor.
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
     * @brief           Applies a "moving average" smoothing algorithm to current sensor value and returns it.
     * @return float    Current sensor value.
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
     * @brief           Applies a "weighted average" smoothing algorithm to current sensor value and returns it.
     * @return float    Current sensor value.
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
     * @brief               Maps the current sensor value to numbers that are easy to work with.
     * @param int vMapTo    The value to map the data to.
     * @return float        Mapped value.
     */
    public float GetMappedReading(int vMapTo)
    {
        return vMapTo * GetFractionalReading();
    }
  
    /**
     * @brief           Maps the current sensor value to the range [0, 1].
     * @return float    Current sensor value, mapped to the range [0, 1].
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

        // Run other checks here.
        // ...
    }

    /**
     * @brief           Resets sensor.
     * @return void
     */
    public void Reset() 
    {
        maCSVData.Clear();
        //maCSVStrings.Initialize ();

        mCSVSize = 0;
        mCSVCounter = 0;
    }
  
    /**
     * @brief           Initializes the resources needed to read data from the sensor.
     * @return void
     */
    public void StartReading()
    {
        // Performance check.
        RunChecks();

        if (vUsingBLE)
        {
            StartReadingBLE();
        }

        else if (vUsingCSVFile)
        {
            StartReadingCSV();
        }
    }
  
    /**
     * @brief           TODO
     * @return void
     */
    public void StartReadingBLE() {}
  
    /**
     * @brief           Read CSV data and populate it to the data buffer.
     * @return void
     */
    public void StartReadingCSV()
    {
        ReadCSVData();
        PopulateCSVData();
    }
  
    /**
     * @brief         Reads the sensor data from a CSV file.
     * @return void
     */
    private void ReadCSVData()
    {
        // If a data set was specified in the container, use that.
        if (!String.IsNullOrEmpty(vDataSet) && String.IsNullOrEmpty(vCSVFilename))
        {
            vCSVFilename = "../../Data/"+ vDataSet +"/"+ vFullName +".csv";
            print("Reading from general data set: "+ vCSVFilename);

            // If we have thresholds for this sensor, load them as well.
            string vThresholdsFile = "../../Data/"+ vDataSet +"/"+ vFullName +".thresholds.csv";
            if (File.Exists(vThresholdsFile))
            {
                print("Loading thresholds for "+ vFullName);
                string[] vThresholds = File.ReadAllLines(vThresholdsFile);
                vMinValue = Convert.ToInt32(vThresholds[0]);
                vMaxValue = Convert.ToInt32(vThresholds[1]);

                // Keep these thresholds for remainder of animation.
                vOverwriteThresholds = false;
            }
        }

        // If a data set was specified in the sensor object, use that.
        else if (!String.IsNullOrEmpty(vCSVFilename) && File.Exists(vCSVFilename))
        {
            print("Reading from specific data set: "+ vCSVFilename);
        }

        // If no data set was specified, use generic data.
        else
        {
            vCSVFilename = mDefaultCSVFilename;
            print("Reading from default data set: "+ mDefaultCSVFilename);
        }

        // Read the CSV into an array.
        maCSVStrings = File.ReadAllLines(vCSVFilename);

        // Populate the CSV data.
        PopulateCSVData();
    }
  
    /**
     * @brief            Populate CSV data.
     * @return void
     */
    private void PopulateCSVData()
    {
        if (vOverwriteThresholds)
        {
            vMaxValue = vMinValue = Convert.ToInt32(maCSVStrings[0]);
        }

        Int32 vValue = 0;
        mCSVSize = 0;

        // Transform and find min and vMaxValue.
        foreach (string vString in maCSVStrings)
        {
            vValue = Convert.ToInt32(vString);
            maCSVData.Add(vValue);

            // Update upper threshold.
            if (vValue > vMaxValue && vOverwriteThresholds)
            {
                vMaxValue = vValue;
            }

            // Update lower threshold.
            if (vValue < vMinValue && vOverwriteThresholds)
            {
                vMinValue = vValue;
            }

            mCSVSize++;
        }
    }
  
    /**
    * @brief            Pushes new values into the buffer.
    * @return void
    */
    private void UpdateBuffer(int vValue)
    {
        // Push buffer data down by one position.
        for (int i = maBuffer.Length - 2; i > -1; i--)
        {
            maBuffer[i + 1] = maBuffer[i];
        }

        // Set current value in buffer.
        maBuffer[0] = vValue;
    }
  
    /**
     * @brief           Checks whether the buffer is filled or not.
     * @return bool     True if buffer is filled.
     */
    private bool IsBufferReady()
    {
        return IsBufferLongEnough(maBuffer.Length);
    }
  
  /**
   * @brief                 Checks whether buffer is filled to desired length.
   * @param int vLength     Required buffer length.
   * @return bool           True if the buffer is filled to desired length.
   */
    private bool IsBufferLongEnough(int vLength)
    {
        // Buffer should only have positive integers.
        for (int i = 0; i < vLength; i++)
        {
            if (maBuffer[i] < 0)
            {
                return false;
            }
        }

        return true;
    }
  
    /**
    * @brief            ...
    * @return void
    */
    public void UpdateSensor () 
    {
        // TODO
        if (vUsingBLE) {}

        // Update buffer with incoming data from the COM port.
        else if (vUsingCOMPort)
        {
            UpdateBuffer(SSContainer.svaModuleData[(int) vCOMChannel]);
        }

        // Update buffer with current value in CSV data.
        else if (vUsingCSVFile && maCSVData.Count > 0)
        {
            UpdateBuffer(maCSVData[mCSVCounter]);

            // Update our CSV index.
            mCSVCounter++;
            if (mCSVCounter >= maCSVData.Count)
            {
                mCSVCounter = 0;
            }
        }
    }



	/////////////////////////////////////////////////////////////////////////////////////
	/// UNITY GENERATED FUNCTIONS 
    //////////////////////////////////////////////////////////////////////////////////////

    
    
    /**
     * @brief           This method is called by Unity when the program is launched.
     * @return void
     */
    void Awake() {}
  
    /**
     * @brief           This method is called by Unity when the program is started.
     * @return void
     */
    void Start()
    {
        if (vIndependentUpdate)
        {
            Reset();
            StartReading();
        }
    }

    /**
     * @brief           This method is called by Unity once per frame.
     * @return void
     */
	void Update()
	{
		if (vIndependentUpdate)
        {
			UpdateSensor();
		}
	}
}

