//*	@ This program calculates number of tun its start and end by getting an input file of "angular velocity*time" named angle integration
//	
//Author: Fatemeh Aghazadeh
//	
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace TurnForOfflineData2
{


        
    class Program
    {
        static void Main(string[] args)
        {


            StreamReader inputFile;

            //Open the file and get a StreamReader object.
            inputFile = File.OpenText("AngleIntegratation.txt");

            // Declare new List.
            List<double> AngleIntegration = new List<double>();
       

            //Save numbers in the file to the list.
            foreach (var number in File.ReadLines("AngleIntegratation.txt").Select(double.Parse))
            AngleIntegration.Add(number);
            inputFile.Close();

            //Define time threshold and angle threshold- definition of turn
            int TimeThresh = 200;
            double AngleThresh = 360; 
            int n = 0;
            int vNumberOfTurns = 0;
            double vAngleIntegrationTurns = 0;
            
            //To save turn data
            List<double>[] Turn = new List<double>[3];
            Turn[0] = new List<double>(); //turn number
            Turn[1] = new List<double>(); //turn start
            Turn[2] = new List<double>(); //turn end

          
            bool doloop = true;
            while (doloop)
            {
                for (int i = n; i < n + TimeThresh; i++)
                {
                                       
                    vAngleIntegrationTurns += AngleIntegration[i];
                    
                    if (Math.Abs(vAngleIntegrationTurns) > AngleThresh) //turn occurs
                    {

                       // for sensor erorr accumulation- if turn sometimes occur at 330 and sometimes at 360 - it reduces erorr
                        //while (Math.Abs(vAngleIntegrationTurns) >= AngleThresh && Math.Abs(vAngleIntegrationTurns) < 360)
                        //{
                        //    i = i + 1;
                        //    vAngleIntegrationTurns += AngleIntegration[i];
                        //    if (Math.Abs(vAngleIntegrationTurns) < AngleThresh || Math.Abs(vAngleIntegrationTurns) > 360)
                        //    {
                        //        i = i - 1;
                        //    }

                        //}

                        
                        vNumberOfTurns++;
                        vAngleIntegrationTurns = 0;
                        
                        //To obtain turn start
                        int j;
                        for ( j = i; j >= n; j--)
                        {
                            vAngleIntegrationTurns += AngleIntegration[j];
                            if (Math.Abs(vAngleIntegrationTurns) > AngleThresh)
                            {
                                
                                using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"turns.txt", true))
                                {
                                    file.WriteLine("Turn Number:     " + vNumberOfTurns + "   Turn Start:      " + j + "  Turn End:     " + i);
                                }


                                Console.WriteLine("Turn Number:     "+ vNumberOfTurns + "   Turn Start:      "+ j + "  Turn End:     " + i);
                                break;
                            }

                        }
                        
                        Turn[0].Add(vNumberOfTurns);
                        Turn[1].Add(j);
                        Turn[2].Add(i);

                        vAngleIntegrationTurns = 0;
                        n = i; //we have n++ later **
                        break;
                    }

                   
                    if (i >= AngleIntegration.Count - 1)
                    {
                        if (n >= AngleIntegration.Count - 1) 
                        { 
                           
                        doloop = false;
                        
                        }
                        break;
                    }
                }
                n++; //**
                vAngleIntegrationTurns = 0;
            }

             
                Console.WriteLine("Number of turns   " + vNumberOfTurns);
                Console.ReadLine();
        }
    }
}
