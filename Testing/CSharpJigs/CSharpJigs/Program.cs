using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;

namespace c_sharp_jigs
{
    class Program
    {
        /**
         * 
         */
        private static SerialPort msStretchSensePortStream = null;

        /**
         * 
         */
        private static string msStretchSenseCOMPort;

        /**
         * 
         */
        private static int[] maStretchSenseModuleData = new int[6];

        /**
         * 
         */
        static void Main(string[] args)
        {
            // Get a list of serial port names.
            string[] vaPortNames = SerialPort.GetPortNames();
            
            Console.WriteLine("Select the StretchSense COM port:");
            foreach (string vPortName in vaPortNames)
            {
                Console.WriteLine(vPortName);
            }

            Console.ReadLine();

            return;

            // Start reading StretchSense COM port.
            msStretchSensePortStream = new SerialPort(msStretchSenseCOMPort, 115200, Parity.None, 8, StopBits.One);

            if (msStretchSensePortStream.IsOpen)
            {
                msStretchSensePortStream.Close();
            }

            // Try to open COM port and send start command.
            try
            {
                msStretchSensePortStream.Open();
                msStretchSensePortStream.Write("#s\r\n");
            }
            catch (Exception e)
            {
            }

            // Use threading to read data.
            Thread vReadThread = new Thread(ReadStretchSenseCOMPort);
            vReadThread.Start();
        }

        /**
         * 
         */
        public static void ReadStretchSenseCOMPort()
        {
            // Performance check.
            if (msStretchSensePortStream == null || !msStretchSensePortStream.IsOpen)
            {
                return;
            }

            // Update channel data
            while (true)
            {
                string rawData = msStretchSensePortStream.ReadLine();

                // Read a data line.
                if (rawData.Length >= 21 && rawData[0] == '!')
                {
                    maStretchSenseModuleData[1] = Convert.ToInt32(rawData.Substring(1, 4));
                    maStretchSenseModuleData[2] = Convert.ToInt32(rawData.Substring(5, 4));
                    maStretchSenseModuleData[3] = Convert.ToInt32(rawData.Substring(9, 4));
                    maStretchSenseModuleData[4] = Convert.ToInt32(rawData.Substring(13, 4));
                    maStretchSenseModuleData[5] = Convert.ToInt32(rawData.Substring(17, 4));

                    print(
                       " #1: " + maStretchSenseModuleData[1] +
                       " #2: " + maStretchSenseModuleData[2] +
                       " #3: " + maStretchSenseModuleData[3] +
                       " #4: " + maStretchSenseModuleData[4] +
                       " #5: " + maStretchSenseModuleData[5]
                   );
                }

                // Read a comment line.
                else if (rawData[0] == '@') {}

                // Anything else will be unusable.
                else {}

            }
        }

        private static void print(string v)
        {
            throw new NotImplementedException();
        }
    }
}
