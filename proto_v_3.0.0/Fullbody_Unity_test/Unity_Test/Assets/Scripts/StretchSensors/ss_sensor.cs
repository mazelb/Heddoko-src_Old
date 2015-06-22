/**
 * @file ss_sensor.cs
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

public class ss_sensor : MonoBehaviour 
{
  /**
   * Details about the sensor.
   */
	public enum positionName {Axilla, Clavicle, Deltoid, Elbow, Forearm, Knee, Trapezius};
  public positionName position = positionName.Axilla;
	public string full_name = "";

	/**
   * Thresholds for sensor data.
   */
	public Int32 min_value = 0;
	public Int32 max_value = 9999;
  //	public enum dataSource {};

	/**
   * Smoothing settings.
   */
	public enum smoothingAlgorithm {None, MovingAverage, WeightedAverage};
	public smoothingAlgorithm smoothing = smoothingAlgorithm.WeightedAverage;
	private int smoothing_history = 20;
	private int[] buffer;

	/*
   * CSV details.
   */
	public bool using_csv_file = false;
	public bool overwrite_thresholds = true;
	public string csv_filename = "";
	public string csv_data_set = "";
  private string default_csv_filename = "../../Data/empty.csv";
	private string[] csv_strings;
	private List<Int32> csv_values = new List<Int32>();
	private Int32 csv_size = 0;
	private Int32 csv_i = 0;

	/*
   * COM port details.
   */
	public bool using_com_port = false;
	public enum dataChannel {One = 1, Two = 2, Three = 3, Four = 4, Five = 5};
	public dataChannel com_channel = dataChannel.One;

	/*
   * TODO: BLE details.
   */
	public bool using_ble = false;

	/*
   * When True the stretch sensor updates independently,
   * otherwise the sensor only updates when the ss_joint asks.
   */
	public bool independent_update = false;
	
	// Circular buffer for data captured and filtering.
	private Int32[] ss_buffer;
	private Int32 buffer_i = 0;
	private Int32 buffer_size = 20;

	// Current readings index
	private float mCurStretchAngle = 0.0f;
  
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	public float get_reading()
	{
    float reading = 0.0f;

		switch (smoothing) {

	    case smoothingAlgorithm.MovingAverage:
        reading = get_moving_average();
        break;

	    case smoothingAlgorithm.WeightedAverage:
        reading = get_weighted_average();
        break;

	    default:
        reading = (float) buffer[0];
        break;
		}

	    return reading;
  }
  
  /**
   * get_moving_average()
   * @brief ...
   * @return float
   */
	public float get_moving_average()
	{
	    // Make sure we have enough data to work with.
	    if (!is_buffer_ready()) {
	        return (float) buffer[0];
	    }

	    // Calculate the moving average, but leave the data buffer untouched.
	    int total = 0;
	    foreach (int value in buffer) {
	        total += value;
	    }

	    return total / buffer.Length;
  }
  
  /**
   * get_weighted_average()
   * @brief ...
   * @return float
   */
	public float get_weighted_average()
	{
    // Make sure we have enough data to work with.
    if (!is_buffer_long_enough(10)) {
      return (float) buffer[0];
    }

    // Calculate weighted average and overwrite current data buffer value.
		buffer[0] = (int) (buffer[1] * 0.9f + buffer[0] * 0.1f);
		return buffer[1] * 0.9f + buffer[0] * 0.1f;
  }
  
  /**
   * get_mapped_reading()
   * @brief Map data to numbers that are easy to work with.
   * @return float
   */
  public float get_mapped_reading(int map_to) {
    return map_to * get_fractional_reading();
  }
  
  /**
   * get_fractional_reading()
   * @brief Map data to the range [0, 1].
   * @return float
   */
  public float get_fractional_reading() {
    return (get_reading() - min_value) / (max_value - min_value);
  }

	//
	// Try to catch mis-configurations before they throw errors.
	//
	public void run_checks()
	{
    // Initialize buffer.
		buffer = new int[smoothing_history];
		for (int i = 0; i < buffer.Length; i++) {
	    buffer[i] = -1;
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
		csv_values.Clear ();
		//csv_strings.Initialize ();

		ss_buffer = new int[buffer_size];
		for (int i=0; i < buffer_size; i++) {
			ss_buffer[i] = 0;
		}

		buffer_i = 0;
		csv_size = 0;
		csv_i = 0;
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
		run_checks();

		if (using_ble) {
			start_reading_ble();
		}

		else if (using_com_port) {
			start_reading_com();
		}

		else if (using_csv_file) {
			start_reading_csv();
		}
  }
  
  /**
   * usage(const char *progName)
   * @brief TODO
   * @param progName
   * @return -1 for failure
   */
	public void start_reading_ble() {}
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	public void start_reading_com()
	{
		// See ss_container
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	public void start_reading_csv()
	{
		read_csv_data();
		populate_csv_values();
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
    if (!String.IsNullOrEmpty(csv_data_set) && String.IsNullOrEmpty(csv_filename)) {
      csv_filename = "../../Data/"+ csv_data_set +"/"+ full_name +".csv";
    }

		// Read from a specific data set.
		if (!String.IsNullOrEmpty(csv_filename) && File.Exists(csv_filename)) {
      print("Reading from "+ csv_filename);
      csv_strings = File.ReadAllLines(csv_filename);
    }

		// Read from the default data set.
		else {
      print("Reading from "+ default_csv_filename);
      csv_strings = File.ReadAllLines(default_csv_filename);
		}

		populate_csv_values();
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	private void populate_csv_values()
	{
    if (overwrite_thresholds) {
	    max_value = min_value = Convert.ToInt32(csv_strings[0]);
    }

		Int32 val = 0;
		csv_size = 0;

		// Transform and find min and max_value.
		foreach (string str in csv_strings)
		{
			val = Convert.ToInt32(str);
			csv_values.Add(val);

			if (val > max_value && overwrite_thresholds) {
				max_value = val;
      }

			if(val < min_value && overwrite_thresholds) {
				min_value = val;
      }

			csv_size++;
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
    for (int i = buffer.Length - 2; i > -1; i--) {
      buffer[i + 1] = buffer[i];
    }

    // Set current value in buffer.
    buffer[0] = value;
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	private bool is_buffer_ready() {
    return is_buffer_long_enough(buffer.Length);
  }
  
  /**
   * usage(const char *progName)
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	private bool is_buffer_long_enough(int length)
	{
    // Buffer should only have positive integers.
    for (int i = 0; i < length; i++) {
      if (buffer[i] < 0) {
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
		if (using_ble) {
			// TODO
		}

    else if (using_com_port) {

      // Update buffer with incoming data from the COM port.
	    update_buffer(ss_container.module_data[(int) com_channel]);
		}

    else if (using_csv_file && csv_values.Count > 0) {
      
      // Update buffer with current value in CSV data.
	    update_buffer(csv_values[csv_i]);

	    // Update our CSV index.
      csv_i++;
      if (csv_i >= csv_values.Count) {
        csv_i = 0;
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
	void Awake() {
		ss_buffer = new int[buffer_size];
	}
  
  /**
   * Start()
   * @brief ...
   * @param progName
   * @return -1 for failure
   */
	void Start()
	{
		if (independent_update) {
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
		if (independent_update) {
			update_sensor();
		}
	}
}

