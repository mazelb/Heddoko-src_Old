using System;
using System.IO;
using System.IO.Ports;
using Nod;
using System.Diagnostics;
using System.Threading;
using UnityEngine;

class Program
{
    /**
     * Interval between each frame, in miliseconds.
     */
    private static int smFrameInterval = 500;

    /**
     * Boolean indicating if debug should be verbose or not.
     */
    private static bool smVerboseDebug = false;

    /**
     * Port streams
     */
    private static SerialPort smEncoderPortStream = null;
    private static SerialPort smStretchSensePortStream = null;

    /**
     * COM ports.
     */
    private static string smEncoderCOMPort;
    private static string smStretchSenseCOMPort;

    /**
     * Nod-related objects.
     */
    private static NodController smNodController;                   // Regular Nod controller used in Unity code.
    private static NodControllerInterface smNodControllerInterface; // Controller interface used by Nod controller (second way of accessing API).
    private static int[] smaNodIDs = new int[1];
    private static NodRing[] smaNodSensors = new NodRing[1];

    /**
     * Collected data.
     */
    private static long smTimeStamp = 0;
    private static double smEncoderData = 0.0;
    private static string smFirstNodData = "";
    private static string smSecondNodData = "";
    private static int[] smaStretchSenseData = new int[6];

    /**
     * 
     */
    private static Stopwatch smStopwatch = null;

    /**
     * Filename to write to. The number will auto-increment if the file already exists.
     */
    private static string smFileName = "jig_test_data_1.csv";

    /**
     * Boolean indicating if we should keep reading data or not.
     */
    private static bool smReading = true;

    /**
     * Main.
     */
    static void Main(string[] args)
    {
        // Select the COM ports for the encoder and StretchSense.

        string[] vaPortNames = SerialPort.GetPortNames();

        Console.WriteLine("\n\tAvailable COM ports:");
        for (int idx = 0; idx < vaPortNames.Length; idx++)
        {
            Console.WriteLine("\t#" + idx + ": " + vaPortNames[idx]);
        }

        Console.WriteLine("\nSelect encoder/Arduino COM port # from list above (leave blank if not in use):");
        smEncoderCOMPort = GetPortName(vaPortNames);

        Console.WriteLine("\nSelect StretchSense COM port # from list above (leave blank if not in use):");
        smStretchSenseCOMPort = GetPortName(vaPortNames);

        // Open the encoder COM port.
        if (smEncoderCOMPort.Length > 0)
        {
            Console.WriteLine("... Connecting to encoder on port " + smEncoderCOMPort);
            smEncoderPortStream = new SerialPort(smEncoderCOMPort, 115200);
            smEncoderPortStream.ReadTimeout = 200;
            OpenCOMPort(smEncoderPortStream, smEncoderCOMPort);
        }

        // Open StretchSense COM port.
        if (smStretchSenseCOMPort.Length > 0)
        {
            Console.WriteLine("... Connecting to StretchSense module on port " + smStretchSenseCOMPort);
            smStretchSensePortStream = new SerialPort(smStretchSenseCOMPort, 115200, Parity.None, 8, StopBits.One);
            smStretchSensePortStream.ReadTimeout = 500;
            OpenCOMPort(smStretchSensePortStream, smStretchSenseCOMPort);

            // Send start command to StretchSense Bluetooth module.
            if (smStretchSensePortStream.IsOpen)
            {
                try
                {
                    smStretchSensePortStream.Write("#s\r\n");
                }
                catch (Exception)
                {
                    Console.WriteLine("Could not send start command to StretchSense module.");
                }
            }
        }

        // Connect to the Nods. The usual way is to use NodController.GetNodInterface(), although this
        // throws a System.Security.SecurityException. An alternative way is to access the NodControllerExternCImp
        // interface directly. All that is now left is to include the NodPlugin DLL.

        //smNodController = NodController.GetNodInterface();
        //int vNumNodsPaired = smNodController.getNumDevices();
        smNodControllerInterface = (NodControllerInterface) new NodControllerExternCImp();
        int vNumNodsPaired = smNodControllerInterface.GetNumDevices();
        if (vNumNodsPaired > 0)
        {
            ConnectNod(0);

            if (vNumNodsPaired > 1)
            {
                ConnectNod(1);
            }
        }

        // Open the file to write to.
        int vFileNameIncrement = 2;
        while (File.Exists(smFileName))
        {
            smFileName = "jig_test_data_" + (vFileNameIncrement++) + ".csv";
        }

        // Read user commands in a seperate thread.
        Thread vCommandsThread = new Thread(ReadUserCommands);
        vCommandsThread.Start();

        // Collect data and write to file.
        smStopwatch = Stopwatch.StartNew();
        using (StreamWriter file = new StreamWriter(smFileName))
        {
            while (smReading == true)
            {
                // Respect the indicated frame rate, so that we're not overloaded with data.
                // We'll also try to avoid duplicate lines by checking the timestamp.
                if (smStopwatch.ElapsedMilliseconds % smFrameInterval != 0 || smStopwatch.ElapsedMilliseconds == smTimeStamp)
                {
                    continue;
                }

                // Update the time stamp.
                smTimeStamp = smStopwatch.ElapsedMilliseconds;

                // Read data from our devices.
                ReadNods();
                ReadEncoder();
                ReadStretchSenseModule();

                // Write the data to file.
                RecordDataToFile(file);
            }
        }

        Console.WriteLine("\nData recorded in " + smFileName + ". Press return to exit.");
        Console.ReadLine();
    }

    /**
     * Retrieves a valid COM port name for a device.
     */
    public static string GetPortName(string[] vaPortNames)
    {
        // Retrieve the port index from the console.
        Console.Write("> #");
        string vPortIndexStr = Console.ReadLine();

        // If a blank line was returned, assume we don't want to use this device.
        if (vPortIndexStr.Length == 0)
        {
            return "";
        }

        // Make sure we have a valid COM port.
        int vPortIndex = Convert.ToInt32(vPortIndexStr);
        if (vPortIndex >= vaPortNames.Length)
        {
            return GetPortName(vaPortNames);
        }

        return vaPortNames[vPortIndex];
    }

    /**
     * Tries to open a COM port so we can read from it.
     */
    public static void OpenCOMPort(SerialPort vPortStream, string vPortName)
    {
        // If port is already open, close it.
        if (vPortStream.IsOpen)
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Closing port: " + vPortName);
            }

            vPortStream.Close();
        }

        // Try to open COM port.
        try
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Opening port: " + vPortName);
            }
            vPortStream.Open();
        }
        catch (Exception e)
        {
            Console.WriteLine("Couldn't open port " + vPortName + " (exception: " + e.Message + ")");
        }
    }

    public static void ConnectNod(int vNodID)
    {
        Console.WriteLine("... Connecting Nod with ID: " + vNodID);

        // Find requested Nod ring.
        //smaNodSensors[vNodID] = smNodController.getRing(vNodID);
        smaNodSensors[vNodID] = smNodControllerInterface.GetRing(vNodID);

        if (smaNodSensors[vNodID] == null)
        {
            Console.WriteLine("Could not find Nod with ID: " + vNodID);
            return;
        }

        // Subscribe the Nod ring to the relevant services.
        if (smaNodSensors[vNodID].Subscribe(NodSubscriptionType.Orientation)
            && smaNodSensors[vNodID].Subscribe(NodSubscriptionType.Button))
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Successfully connected Nod wth ID: " + vNodID);
            }
        }
    }

    /**
     * Retrieves encoder data.
     *
     * See: Heddoko-src/Testing/stretchSense_encoder/dual_csv.py
     */
    public static void ReadEncoder()
    {
        // Performance check.
        if (smEncoderCOMPort.Length == 0 || smEncoderPortStream == null || !smEncoderPortStream.IsOpen)
        {
            return;
        }

        // Retrieve data.
        string vRawData = "";
        try
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Reading from encoder...");
            }

            vRawData = smEncoderPortStream.ReadLine();
        }
        catch (Exception e)
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Could not read from encoder: " + e.Message);
            }
        }

        // Update encoder values.
        if (vRawData.Length > 0)
        {
            smEncoderData = Convert.ToDouble(vRawData);
        }

        if (smVerboseDebug)
        {
            Console.WriteLine("Encoder raw data: " + vRawData);
        }
    }

    /**
     * Retrieves Nod data.
     *
     * See: Heddoko-src/proto_v_3.0.0/Fullbody_Unity_test/Unity_Test/Assets/Scripts/NodSensors/NodSensor.cs
     */
    public static void ReadNods()
    {
        // Performance check.
        if (false)
        {
            return;
        }
        
        for (int idx = 0; idx < smaNodSensors.Length; idx++)
        {
            smaNodSensors[idx].CheckForUpdate();

            //Quaternion vRingRotation = smaNodSensors[idx].ringRotation;
            //Vector3 vEulerAngles = vRingRotation.eulerAngles;
            //Vector3 vRawEulerAngles = smaNodSensors[idx].ringEulerRotation;
        }
    }

    /**
     * Retrieves StretchSense data.
     *
     * See: Heddoko-src/proto_v_3.0.0/Fullbody_Unity_test/Unity_Test/Assets/Scripts/StretchSensors/SSContainer.cs
     */
    public static void ReadStretchSenseModule()
    {
        // Performance check.
        if (smStretchSenseCOMPort.Length == 0 || smStretchSensePortStream == null || !smStretchSensePortStream.IsOpen)
        {
            return;
        }

        // Retrieve data.
        string vRawData = "";
        try
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Reading from StretchSense module...");
            }

            vRawData = smStretchSensePortStream.ReadLine();
        }
        catch (Exception e)
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Could not read from StretchSense module: " + e.Message);
            }
        }

        // Read a data line.
        if (vRawData.Length >= 21 && vRawData[0] == '!')
        {
            if (smVerboseDebug)
            {
                Console.WriteLine("Received SS data.");
            }

            smaStretchSenseData[1] = Convert.ToInt32(vRawData.Substring(1, 4));
            smaStretchSenseData[2] = Convert.ToInt32(vRawData.Substring(5, 4));
            smaStretchSenseData[3] = Convert.ToInt32(vRawData.Substring(9, 4));
            smaStretchSenseData[4] = Convert.ToInt32(vRawData.Substring(13, 4));
            smaStretchSenseData[5] = Convert.ToInt32(vRawData.Substring(17, 4));
        }

        // Read a comment line.
        else if (vRawData[0] == '@' && smVerboseDebug)
        {
            Console.WriteLine("StretchSense module comment: " + vRawData);
        }

        // Anything else will be unusable.
        else { }
    }

    /**
     * Writes the collected data to file.
     *
     * @param StreamWriter file
     */
    public static void RecordDataToFile(StreamWriter file)
    {
        String vDataLine = "";

        // Start with a timestamp.
        vDataLine += smTimeStamp + ",";

        // Append the encoder value.
        if (smEncoderPortStream != null && smEncoderPortStream.IsOpen)
        {
            vDataLine += smEncoderData;
        }
        vDataLine += ",";

        // Append Nod euler angles.
        vDataLine += ",,,,,,";

        // Append StretchSense data.
        if (smStretchSensePortStream != null && smStretchSensePortStream.IsOpen)
        {
            vDataLine +=
                smaStretchSenseData[1] + "," +
                smaStretchSenseData[2] + "," +
                smaStretchSenseData[3] + "," +
                smaStretchSenseData[4] + "," +
                smaStretchSenseData[5];
        }
        else
        {
            vDataLine += ",,,,";
        }

        file.WriteLine(vDataLine);
    }

    /**
     * Reads commands from the console and acts upon them.
     */
    public static void ReadUserCommands()
    {
        string vCommand = "";
        StringComparer vComparer = StringComparer.OrdinalIgnoreCase;
        Console.WriteLine("\nType \"quit\" to stop the program.");

        while (smReading)
        {
            Console.Write("> ");

            // Check for "quit" command.
            vCommand = Console.ReadLine();
            if (vComparer.Equals("quit", vCommand))
            {
                smReading = false;
            }
        }
    }

    /**
     * Cleans up before exiting program.
     */
    public static void Finish()
    {
        // Close COM ports.
        if (smEncoderCOMPort.Length > 0 && smEncoderPortStream.IsOpen)
        {
            smEncoderPortStream.Close();
        }
        if (smStretchSenseCOMPort.Length > 0 && smStretchSensePortStream.IsOpen)
        {
            smStretchSensePortStream.Close();
        }

        // Unsubscribe from Nod services.
        for (int idx = 0; idx < smaNodSensors.Length; idx++)
        {
            if (smaNodSensors[idx] != null)
            {
                smaNodSensors[idx].Unsubscribe(NodSubscriptionType.Button);
                smaNodSensors[idx].Unsubscribe(NodSubscriptionType.Orientation);
            }
        }
    }
}
