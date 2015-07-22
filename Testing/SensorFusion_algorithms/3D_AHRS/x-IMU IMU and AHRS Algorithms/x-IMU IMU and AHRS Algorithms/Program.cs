using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Threading;
using System.Windows.Forms;
using System.ComponentModel;
using System.IO;
using System.IO.Pipes;

namespace x_IMU_IMU_and_AHRS_Algorithms
{
    class Program
    {



        /// <summary>
        /// Algorithm object.
        /// </summary>
        static AHRS.MadgwickAHRS AHRS = new AHRS.MadgwickAHRS(1f / 256f, 0.1f);
        //static AHRS.MahonyAHRS AHRS = new AHRS.MahonyAHRS(1f / 256f, 5f);

        /// <summary>
        /// Main method.
        /// </summary>
        /// <param name="args">
        /// Unused.
        /// </param>
        static void Main(string[] args)
        {
   
             
    
            Console.WriteLine(Assembly.GetExecutingAssembly().GetName().Name + " " + Assembly.GetExecutingAssembly().GetName().Version.Major.ToString() + "." + Assembly.GetExecutingAssembly().GetName().Version.Minor.ToString());
           try
           {
                // Connect to x-IMU
                //Console.WriteLine("Searching for x-IMU...");
                //x_IMU_API.PortAssignment[] portAssignment = (new x_IMU_API.PortScanner(true, true)).Scan();
                //x_IMU_API.xIMUserial xIMUserial = new x_IMU_API.xIMUserial(portAssignment[0].PortName);
                //xIMUserial.Open();
                //Console.WriteLine("Connected to x-IMU " + portAssignment[0].DeviceID + " on " + portAssignment[0].PortName + ".");

                // Show 3D cuboid forms
                Console.WriteLine("Showing 3D Cuboid forms...");
                Form_3Dcuboid form_3DcuboidA = new Form_3Dcuboid();
                Form_3Dcuboid form_3DcuboidB = new Form_3Dcuboid(new string[] { "Form_3Dcuboid/RightInv.png", "Form_3Dcuboid/LeftInv.png", "Form_3Dcuboid/BackInv.png", "Form_3Dcuboid/FrontInv.png", "Form_3Dcuboid/TopInv.png", "Form_3Dcuboid/BottomInv.png" });
                             
                form_3DcuboidA.Text += " A";
                form_3DcuboidB.Text += " B";


                BackgroundWorker backgroundWorkerA = new BackgroundWorker();
                BackgroundWorker backgroundWorkerB = new BackgroundWorker();


                backgroundWorkerA.DoWork += new DoWorkEventHandler(delegate { form_3DcuboidA.ShowDialog(); });
                backgroundWorkerB.DoWork += new DoWorkEventHandler(delegate { form_3DcuboidB.ShowDialog(); });

                backgroundWorkerA.RunWorkerAsync();
                backgroundWorkerB.RunWorkerAsync();

               // stores orientation of each cube 
               float[] hi = { 1, 0, 0, 0, 1, 0, 0, 0,1 };
               float[] hi2 = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

                form_3DcuboidA.RotationMatrix = hi;
                form_3DcuboidB.RotationMatrix = hi2;
                
               
           

                //pipe istalation //       
                using (NamedPipeClientStream pipeClient =
            new NamedPipeClientStream(".", "testpipe", PipeDirection.In))
                {

                    // Connect to the pipe or wait until the pipe is available.
                    Console.Write("Attempting to connect to pipe...");
                    pipeClient.Connect();

                    Console.WriteLine("Connected to pipe.");
                    Console.WriteLine("There are currently {0} pipe server instances open.",
                    pipeClient.NumberOfServerInstances);
             
      
                using (StreamReader sr = new StreamReader(pipeClient))
            {
    
                // Display the read text to the console 
                string temp;
                while (true)
                {
                    for (int i=0; i<=8  ;i++ )
                    { temp = sr.ReadLine();
                    hi[i] = float.Parse(temp);}
                    form_3DcuboidA.RotationMatrix = hi;



                    for (int i = 0; i <= 8; i++)
                    {
                        temp = sr.ReadLine();
                        hi2[i] = float.Parse(temp);
                    }
                    form_3DcuboidB.RotationMatrix = hi2;



                }
            }
        }


            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
            }
            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }

        /// <summary>
        /// xIMUserial CalInertialAndMagneticDataReceived event to update algorithm in IMU mode.
        /// </summary>
        static void xIMUserial_CalInertialAndMagneticDataReceived_updateIMU(object sender, x_IMU_API.CalInertialAndMagneticData e)
        {
            AHRS.Update(deg2rad(e.Gyroscope[0]), deg2rad(e.Gyroscope[1]), deg2rad(e.Gyroscope[2]), e.Accelerometer[0], e.Accelerometer[1], e.Accelerometer[2]);
        }

        /// <summary>
        /// xIMUserial CalInertialAndMagneticDataReceived event to update algorithm in AHRS mode.
        /// </summary>
        static void xIMUserial_CalInertialAndMagneticDataReceived_updateAHRS(object sender, x_IMU_API.CalInertialAndMagneticData e)
        {
            AHRS.Update(deg2rad(e.Gyroscope[0]), deg2rad(e.Gyroscope[1]), deg2rad(e.Gyroscope[2]), e.Accelerometer[0], e.Accelerometer[1], e.Accelerometer[2], e.Magnetometer[0], e.Magnetometer[1], e.Magnetometer[2]);
        }

        /// <summary>
        /// Converts degrees to radians.
        /// </summary>
        /// <param name="degrees">
        /// Angular quantity in degrees.
        /// </param>
        /// <returns>
        /// Angular quantity in radians.
        /// </returns>
        static float deg2rad(float degrees)
        {
            return (float)(Math.PI / 180) * degrees;
        }
    }
}