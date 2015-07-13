/**
 * @file StretchContainer.cs
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

public class StretchContainer : MonoBehaviour 
{
    /**
     * Information to setup COM port.
     */
    public bool vUsingCOMPort = false;
    public bool vPrintChannelData = true;
    public string vCOMPort;
    public int vBaudRate = 115200;
    public int vReadTimeout = 200;
    private SerialPort mPortStream = null;

    /**
    * Shortcut to apply CSV data sets to all sensors at the same time.
    */
    public string vCSVDataSet = "default";

    /**
     * Array of joints in container.
     */
    private StretchJoint[] maJoints;

    /**
     * Data from StretchSense module.
     * This is accessible from other scripts.
     */
    public static int[] svaModuleData = new int[6];

    /**
     * usage(const char *progName)
     * @brief Update() is called once per frame.
     * @param progName
     * @return -1 for failure
     */
    public void StartJoints() 
    {
        // Set up the port stream if we're going to need it.
        if (vUsingCOMPort && !String.IsNullOrEmpty(vCOMPort))
        {
            start_COM_port();
        }

        // Loop through joint scripts to initialize them.
        for (int i = 0; i < maJoints.Length; i++)
        {
            if (!maJoints[i].vIndependentUpdate)
            {
                // vCSVDataSet allows us to set specific CSV files (by folder) to all sensors.
                maJoints[i].StartJoint(vCSVDataSet);
            }
        }
    }

    /**
     * TODO: describe this.
     */
    public void UpdateJoints() 
    {
        for (int i = 0; i < maJoints.Length; i++) 
        {
            if(!maJoints[i].vIndependentUpdate)
            {
                maJoints[i].update_joint();
            }
        }
    }

    /**
     * usage(const char *progName)
     * @brief Loops through the joint scripts and calls their reset method.
     * @return void
     */
    public void ResetJoints()
    {
        for (int i = 0; i < maJoints.Length; i++) 
        {
            if(!maJoints[i].vIndependentUpdate)
            {
                maJoints[i].reset_joint();
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
        print("Using COM port: "+ vCOMPort);
        mPortStream = new SerialPort(vCOMPort, vBaudRate, Parity.None, 8, StopBits.One);

        if (mPortStream.IsOpen)
        {
            mPortStream.Close();
        }

        // Reduce the read timeout.
        mPortStream.ReadTimeout = vReadTimeout;

        // Try to open COM port and send start command.
        try
        {
            mPortStream.Open();
            mPortStream.Write("#s\r\n");
        }
        catch (Exception e)
        {
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
        // Performance check.
        if (mPortStream == null || !mPortStream.IsOpen)
        {
            return;
        }

        // Update channel data
        while (true)
        {
            string rawData = mPortStream.ReadLine();

            // Read a data line.
            if (rawData.Length >= 21 && rawData[0] == '!')
            {
                svaModuleData[1] = Convert.ToInt32(rawData.Substring(1, 4));
                svaModuleData[2] = Convert.ToInt32(rawData.Substring(5, 4));
                svaModuleData[3] = Convert.ToInt32(rawData.Substring(9, 4));
                svaModuleData[4] = Convert.ToInt32(rawData.Substring(13, 4));
                svaModuleData[5] = Convert.ToInt32(rawData.Substring(17, 4));

                if (vPrintChannelData)
                {
                    print(
                        " #1: "+ svaModuleData[1] +
                        " #2: "+ svaModuleData[2] +
                        " #3: "+ svaModuleData[3] +
                        " #4: "+ svaModuleData[4] +
                        " #5: "+ svaModuleData[5]
                    );
                }
            }

            // Read a comment line.
            else if (rawData[0] == '@')
            {
                print(rawData);
            }

            // Anything else will be unusable.
            else
            {
                print("Invalid incoming data.");
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
        if (mPortStream != null)
        {
            mPortStream.Close();
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
        maJoints = GetComponentsInChildren<StretchJoint>();
    }
  
    /**
     * Start()
     * @brief ...
     * @return void
     */
    void Start() 
    {
        ResetJoints();
        StartJoints();
    }
  
    /**
     * Update()
     * @brief Update() is called once per frame.
     * @return void
     */
    void Update() 
    {
        UpdateJoints();
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
            ResetJoints();
            StartJoints();
        }

        if (GUI.Button (new Rect (20, 52, 150, 30), "Reset Sensors "))
        {
            ResetJoints();        
        }
    }
}

