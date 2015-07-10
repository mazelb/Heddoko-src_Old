/**
 * @file ss_container.cs
 * @brief This script acts as the parent to all joint scripts for a given Unity game object. The game object
 * must be a person animated exclusively with StretchSense sensors (or any stretchable sensor).
 * @note Add notes here.
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
using System.Threading;

public class ss_container : MonoBehaviour 
{
  /**
   * Information to setup COM port.
   */
  public bool using_COM_port = false;
  public bool print_channel_data = true;
  public string COM_port;
  public int COM_baud_rate = 115200;
  public int COM_read_timeout = 200;
  private SerialPort m_port_stream = null;
  
  /**
   * Shortcut to apply CSV data sets to all sensors at the same time.
   */
  public string csv_data_set = "default";
  
  /**
   * Array of joints in container.
   */
  private ss_joint[] m_joints;
  
  /**
   * Data from StretchSense module.
   * This is accessible from other scripts.
   */
  public static int[] module_data = new int[6];
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
  public void start_joints () 
  {
    // Set up the port stream if we're going to need it.
    if (using_COM_port && !String.IsNullOrEmpty(COM_port)) {
      
      start_COM_port();
    }
    
    // Loop through joint scripts to initialize them.
    for (int i = 0; i < m_joints.Length; i++) {
      if (!m_joints[i].independent_update) {
        
        // csv_data_set allows us to set specific CSV files (by folder) to all sensors.
        m_joints[i].start_joint(csv_data_set);
      }
    }
  }
  
  /**
   * TODO: describe this.
   */
  public void update_joints () 
  {
    for (int i = 0; i < m_joints.Length; i++) 
    {
      if(!m_joints[i].independent_update)
      {
        m_joints[i].update_joint();
      }
    }
  }
  
  /**
   * usage(const char *progName)
   * @brief Loops through the joint scripts and calls their reset method.
   * @return void
   */
  public void reset_joints()
  {
    for (int i = 0; i < m_joints.Length; i++) 
    {
      if(!m_joints[i].independent_update)
      {
        m_joints[i].reset_joint();
      }
    }
  }
  
  /**
   * usage(const char *progName)
   * @brief Update() is called once per frame.
   * @param progName
   * @return -1 for failure
   */
  private void start_COM_port()
  {
    print("Using COM port: "+ COM_port);
    m_port_stream = new SerialPort(COM_port, COM_baud_rate, Parity.None, 8, StopBits.One);
    if (m_port_stream.IsOpen) {
      m_port_stream.Close();
    }
    
    // Reduce the read timeout.
    m_port_stream.ReadTimeout = COM_read_timeout;
    
    // Try to open COM port and send start command.
    try {
      m_port_stream.Open();
      m_port_stream.Write("#s\r\n");
    }
    catch (Exception e) {
      print("Could not open COM port: "+ e.Message);
    }
    
    // Use threading to read data.
    Thread readThread = new Thread(ReadCOMPort);
    readThread.Start();
  }
  
  /**
   * usage(const char *progName)
   * @brief Reads data from the assigned COM port.
   * @return void
   */
  public void ReadCOMPort()
  {
    // Update channel data
    if (m_port_stream != null && m_port_stream.IsOpen)
    {
      while (true)
      {
        string rawData = m_port_stream.ReadLine();
        
        // Read a data line.
        if (rawData.Length >= 21 && rawData[0] == '!')
        {
          module_data[1] = Convert.ToInt32(rawData.Substring(1, 4));
          module_data[2] = Convert.ToInt32(rawData.Substring(5, 4));
          module_data[3] = Convert.ToInt32(rawData.Substring(9, 4));
          module_data[4] = Convert.ToInt32(rawData.Substring(13, 4));
          module_data[5] = Convert.ToInt32(rawData.Substring(17, 4));
          
          if (print_channel_data)
          {
            print(
              " #1: "+ module_data[1] +
              " #2: "+ module_data[2] +
              " #3: "+ module_data[3] +
              " #4: "+ module_data[4] +
              " #5: "+ module_data[5]
              );
          }
        }
        
        // Read a comment line.
        else if (rawData[0] == '@')
        {
          print(rawData);
        }
        
        // Anything else will be unusable.
        else {
          print("Invalid incoming data.");
        }
      }
    }
  }
  
  /**
   * OnApplicationQuit()
   * @brief Called once when the application quits.
   * @return void
   */
  public void OnApplicationQuit()
  {
    if (m_port_stream != null)
    {
      m_port_stream.Close();
    }
  }
  
  
  
  /////////////////////////////////////////////////////////////////////////////////////
  /// UNITY GENERATED FUNCTIONS 
  //////////////////////////////////////////////////////////////////////////////////////
  
  
  
  /**
   * Awake()
   * @brief Awake() is called once on startup.
   * @return void
   */
  void Awake ()
  {
    Application.targetFrameRate = 300;
    m_joints = GetComponentsInChildren<ss_joint>();
  }
  
  /**
   * Start()
   * @brief ...
   * @return void
   */
  void Start() 
  {
    reset_joints();
    start_joints();
  }
  
  /**
   * Update()
   * @brief Update() is called once per frame.
   * @return void
   */
  void Update() 
  {
    update_joints();
  }
  
  /**
   * OnGUI()
   * @brief ...
   * @return void
   */
  void OnGUI()
  {
    if (GUI.Button (new Rect (20, 20, 150, 30), "Start Sensors"))
    {
      reset_joints();
      start_joints();
    }
    
    if (GUI.Button (new Rect (20, 52, 150, 30), "Reset Sensors "))
    {
      reset_joints();        
    }
  }
}
