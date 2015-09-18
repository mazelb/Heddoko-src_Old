using System;
using System.IO;
using System.IO.Ports;
using Nod;

class Program
{
    /**
     * Port streams
     */
    private static SerialPort mEncoderPortStream = null;
    private static SerialPort mStretchSensePortStream = null;

    /**
     * COM ports.
     */
    private static string mEncoderCOMPort;
    private static string mStretchSenseCOMPort;

    /**
     * Nod-related objects.
     */
    private static NodController mNodController;
    private static NodRing mFirstNodSensor = null;
    private static NodRing mSecondNodSensor = null;

    /**
     * Collected data.
     */
    private static string mEncoderData = "";
    private static string mFirstNodData = "";
    private static string mSecondNodData = "";
    private static int[] maStretchSenseData = new int[6];

    /**
     * Filename to write to. The number will auto-increment if the file already exists.
     */
    private static string mFileName = "jig_test_data_1.csv";

    /**
     * Boolean indicating if debug should be verbose or not.
     */
    private static bool mVerboseDebug = true;

    /**
     * Boolean indicating if we should keep reading data or not.
     */
    private static bool mReading = true;

    /**
     * Main.
     */
    static void Main(string[] args)
    {
        // Select the COM ports for the encoder and StretchSense.

        string[] vaPortNames = SerialPort.GetPortNames();

        Console.WriteLine("\n\tAvailable COM ports:");

        int idx = 0;
        for (; idx < vaPortNames.Length; idx++)
        {
            Console.WriteLine("\t#" + idx + ": " + vaPortNames[idx]);
        }

        Console.WriteLine("\nSelect the COM port for encoder/Arduino (leave blank if not in use):");
        mEncoderCOMPort = GetPortName(vaPortNames);

        Console.WriteLine("\nSelect the COM port for the StretchSense module (leave blank if not in use):");
        mStretchSenseCOMPort = GetPortName(vaPortNames);

        // Open the encoder COM port.
        if (mEncoderCOMPort.Length > 0)
        {
            Console.WriteLine("Connecting to encoder on port " + mEncoderCOMPort);
            mEncoderPortStream = new SerialPort(mEncoderCOMPort, 115200);
            mEncoderPortStream.ReadTimeout = 200;
            OpenCOMPort(mEncoderPortStream, mEncoderCOMPort);
        }

        // Open StretchSense COM port.
        if (mStretchSenseCOMPort.Length > 0)
        {
            Console.WriteLine("Connecting to StretchSense module on port " + mStretchSenseCOMPort);
            mStretchSensePortStream = new SerialPort(mStretchSenseCOMPort, 115200, Parity.None, 8, StopBits.One);
            mStretchSensePortStream.ReadTimeout = 500;
            OpenCOMPort(mStretchSensePortStream, mStretchSenseCOMPort);

            // Send start command to StretchSense Bluetooth module.
            if (mStretchSensePortStream.IsOpen)
            {
                try
                {
                    mStretchSensePortStream.Write("#s\r\n");
                }
                catch (Exception e)
                {
                    Console.WriteLine("Could not send start command to StretchSense module.");
                }
            }
        }

        // Connect to the Nods.
        //mNodController = NodController.GetNodInterface();

        // Open the file to write to.
        int vFileNameIncrement = 2;
        while (File.Exists(mFileName))
        {
            mFileName = "jig_test_data_" + (vFileNameIncrement++) + ".csv";
        }

        // Collect data and write to file.
        using (StreamWriter file = new StreamWriter(mFileName))
        {
            string command = "";
            StringComparer str = StringComparer.OrdinalIgnoreCase;
            Console.WriteLine("Type \"quit\" to stop the program.");
            while (mReading)
            {
                // Check for "quit" command.
                command = Console.ReadLine();
                if (str.Equals("quit", command))
                {
                    mReading = false;
                    break;
                }

                // Read data from our devices.
                ReadNods();
                ReadEncoder();
                ReadStretchSenseModule();

                // Write the data to file.
                RecordDataToFile(file);
            }
        }

        Console.WriteLine($"\nThe test data was recorded in \"{mFileName}\". Press return to exit.");
        Console.ReadLine();
    }

    /**
     * Retrieves a valid COM port name for a device.
     */
    public static string GetPortName(string[] vaPortNames)
    {
        // Retrieve the port index from the console.
        Console.Write("> # ");
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
            if (mVerboseDebug)
            {
                Console.WriteLine("Closing port: " + vPortName);
            }

            vPortStream.Close();
        }

        // Try to open COM port.
        try
        {
            if (mVerboseDebug)
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

    /**
     * Retrieves encoder data.
     *
     * See: Heddoko-src/Testing/stretchSense_encoder/dual_csv.py
     */
    public static void ReadEncoder()
    {
        // Performance check.
        if (mEncoderCOMPort.Length == 0 || mEncoderPortStream == null || !mEncoderPortStream.IsOpen)
        {
            return;
        }

        // Retrieve data.
        string vRawData = "";
        try
        {
            if (mVerboseDebug)
            {
                Console.WriteLine("Reading from encoder...");
            }

            vRawData = mEncoderPortStream.ReadLine();
            //vRawData = mEncoderPortStream.ReadTo("\r");
        }
        catch (Exception e)
        {
            if (mVerboseDebug)
            {
                Console.WriteLine("Could not read from encoder: " + e.Message);
            }
        }

        // Update encoder values.
        if (vRawData.Length == 8)
        {
            mEncoderData = vRawData;
        }

        if (mVerboseDebug)
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
        int vNumRingsPaired = mNodController.getNumDevices();
    }

    /**
     * Retrieves StretchSense data.
     *
     * See: Heddoko-src/proto_v_3.0.0/Fullbody_Unity_test/Unity_Test/Assets/Scripts/StretchSensors/SSContainer.cs
     */
    public static void ReadStretchSenseModule()
    {
        // Performance check.
        if (mStretchSenseCOMPort.Length == 0 || mStretchSensePortStream == null || !mStretchSensePortStream.IsOpen)
        {
            return;
        }

        // Retrieve data.
        string vRawData = "";
        try
        {
            if (mVerboseDebug)
            {
                Console.WriteLine("Reading from StretchSense module...");
            }

            vRawData = mStretchSensePortStream.ReadLine();
            //vRawData = mStretchSensePortStream.ReadTo("\r");
        }
        catch (Exception e)
        {
            if (mVerboseDebug)
            {
                Console.WriteLine("Could not read from StretchSense module: " + e.Message);
            }
        }

        // Read a data line.
        if (vRawData.Length >= 21 && vRawData[0] == '!')
        {
            maStretchSenseData[1] = Convert.ToInt32(vRawData.Substring(1, 4));
            maStretchSenseData[2] = Convert.ToInt32(vRawData.Substring(5, 4));
            maStretchSenseData[3] = Convert.ToInt32(vRawData.Substring(9, 4));
            maStretchSenseData[4] = Convert.ToInt32(vRawData.Substring(13, 4));
            maStretchSenseData[5] = Convert.ToInt32(vRawData.Substring(17, 4));
        }

        // Read a comment line.
        else if (vRawData[0] == '@' && mVerboseDebug)
        {
            Console.WriteLine("StretchSense module comment: " + vRawData);
        }

        // Anything else will be unusable.
        else { }
    }

    /**
     * TODO
     */
    public static void RecordDataToFile(StreamWriter file)
    {

    }

    public static void Finish()
    {
        // Close COM ports.
        if (mEncoderCOMPort.Length > 0 && mEncoderPortStream.IsOpen)
        {
            mEncoderPortStream.Close();
        }
        if (mStretchSenseCOMPort.Length > 0 && mStretchSensePortStream.IsOpen)
        {
            mStretchSensePortStream.Close();
        }

        // Unsubscribe from Nod services.
        if (mFirstNodSensor != null)
        {
            mFirstNodSensor.Unsubscribe(NodSubscriptionType.Button);
            mFirstNodSensor.Unsubscribe(NodSubscriptionType.Orientation);
        }
        if (mSecondNodSensor != null)
        {
            mSecondNodSensor.Unsubscribe(NodSubscriptionType.Button);
            mSecondNodSensor.Unsubscribe(NodSubscriptionType.Orientation);
        }
    }
}
