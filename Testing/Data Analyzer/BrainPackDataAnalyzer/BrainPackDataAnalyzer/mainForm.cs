using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO; 
using System.Threading;
namespace BrainPackDataAnalyzer
{
    public partial class mainForm : Form
    {
        public Thread readThread;
        public FileStream dataFile;
        public DataTable analysisData;
        public DataTable sensorStats;
        public imu[] imuArray = new imu[9];
        private bool openSerialPort = false; 
        public mainForm()
        {
            InitializeComponent();
        }

        public void ReadThread()
        {
            while (true)
            {
                if (serialPort.IsOpen)
                {
                    try
                    {
                        string line = serialPort.ReadLine();
                        if (line.Length > 170)
                        {
                            processEntry(line);
                        }
                        else
                        {
                            this.Invoke((MethodInvoker)(() => tb_Console.AppendText(line + "\r\n")));
                        }
                    }
                    catch
                    {
                        //do nothing, this is alright
                    }
                    if(!openSerialPort)
                    {
                        this.Invoke((MethodInvoker)(() => tb_Console.AppendText("Serial Port Closed\r\n")));
                        serialPort.Close();
                        return;
                    }

                }
            }
        }
        public void processEntry(string entry)
        {            
            string[] entrySplit = entry.Split(',');
            //there should be 12 columns of data
            if (entrySplit.Length >= 12)
            {
                try
                {
                    UInt32 timeStamp = UInt32.Parse(entrySplit[0]);
                    UInt16 sensorMask = UInt16.Parse(entrySplit[1], System.Globalization.NumberStyles.HexNumber);
                    //sensorStats.Rows.Clear(); 
                    for(int i = 0; i < 9; i++)
                    {
                        //if the frame is valid for this sensor then process it. 
                        if ((sensorMask & (1<<i)) > 0)
                        {
                            imuArray[i].ProcessEntry(timeStamp, entrySplit[i + 2]);
                        }
                        //DataRow row = sensorStats.NewRow();
                        sensorStats.Rows[i]["Sensor ID"] = i.ToString();
                        sensorStats.Rows[i]["Roll"] = imuArray[i].GetCurrentEntry().Roll.ToString("F3");
                        sensorStats.Rows[i]["Pitch"] = imuArray[i].GetCurrentEntry().Pitch.ToString("F3");
                        sensorStats.Rows[i]["Yaw"] = imuArray[i].GetCurrentEntry().Yaw.ToString("F3");
                        sensorStats.Rows[i]["Frame Count"] = imuArray[i].GetTotalEntryCount().ToString();
                        sensorStats.Rows[i]["Interval"] = imuArray[i].GetLastInterval().ToString();
                        sensorStats.Rows[i]["Max Interval"] = imuArray[i].GetMaxInterval().ToString();
                        sensorStats.Rows[i]["Average Interval"] = imuArray[i].GetAverageInterval().ToString();
                        //sensorStats.Rows.Add(row);
                    }
                    //ImuEntry imu0Data = new ImuEntry(entrySplit[1]);
                    //this.Invoke((MethodInvoker)(() => tb_stretchData.Text = entrySplit[11]));
                    //this.Invoke((MethodInvoker)(() => tb_imu0_y.Text = imuArray[0].GetCurrentEntry().Yaw.ToString("F3")));
                    //this.Invoke((MethodInvoker)(() => tb_imu0_p.Text = imuArray[0].GetCurrentEntry().Pitch.ToString("F3")));
                    //bindingSource1.DataSource = sensorStats;
                    //dgv_SensorStats.DataSource = bindingSource1;
                    //this.Invoke((MethodInvoker)(() => dgv_SensorStats.Update()));
                }
                catch
                {
                    //error
                }
            }
        }
        private void mainForm_Load(object sender, EventArgs e)
        {

            cb_serialPorts.Items.AddRange(SerialPort.GetPortNames());
            serialPort.NewLine = "\r\n";
            //start read thread --> automatically put things in list box            
            tb_Console.AppendText("Brain Data Analyzer\r\n");
            sensorStats = new DataTable("Sensor Statistics");
            sensorStats.Columns.Add("Sensor ID", typeof(string));
            sensorStats.Columns.Add("Roll", typeof(string));
            sensorStats.Columns.Add("Pitch", typeof(string));
            sensorStats.Columns.Add("Yaw", typeof(string));
            sensorStats.Columns.Add("Frame Count", typeof(string));
            sensorStats.Columns.Add("Interval", typeof(string));
            sensorStats.Columns.Add("Max Interval", typeof(string));
            sensorStats.Columns.Add("Average Interval", typeof(string));
            for (int i = 0; i < 9; i++)
            {
                DataRow row = sensorStats.NewRow();
                row["Sensor ID"] = i.ToString();
                row["Roll"] = "0";
                row["Pitch"] = "0";
                row["Yaw"] = "0";
                row["Frame Count"] = "0";
                row["Interval"] = "0";
                row["Max Interval"] = "0";
                row["Average Interval"] = "0";
                sensorStats.Rows.Add(row);
            }
            for(int i = 0; i < imuArray.Length; i++)
            {
                imuArray[i] = new imu(); 
            }
            bindingSource1.DataSource = sensorStats;
            dgv_SensorStats.DataSource = bindingSource1; 
            dgv_SensorStats.Update();

        }

        private void bnt_Connect_Click(object sender, EventArgs e)
        {
            //set the serial port to the selected item. 
            Thread serialThread = new Thread(ReadThread);
            if (serialPort.IsOpen)
            {
                //do nothing
                return; 
            }
            
            serialPort.PortName = cb_serialPorts.Items[cb_serialPorts.SelectedIndex].ToString();
            try
            {
                openSerialPort = true;
                serialPort.Open();
                if (serialPort.IsOpen)
                {
                    serialThread.Start();
                }
                tb_Console.AppendText("Port: " + serialPort.PortName + "Open\r\n");
            }
            catch
            {
                tb_Console.AppendText("Failed to open Port: " + serialPort.PortName + "Open\r\n");
                openSerialPort = false;
            }
        }
        private void btn_disconnect_Click(object sender, EventArgs e)
        {
            openSerialPort = false;
        }
        private void btn_SendCmd_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                if (tb_cmd.Text.Length > 0)
                {
                    serialPort.Write(tb_cmd.Text + "\r\n");
                }
            }
        }
        private void btn_getState_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write("GetState\r\n");                
            }
        }
        private void btn_record_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write("Record\r\n");
            }
        }
        private void btn_reset_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write("Reset\r\n");
            }
        }
        private void btn_setTime_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                DateTime time = DateTime.Now;
                int dayOfWeekNumber = (int)time.DayOfWeek;
                string timeCommand = "setTime" + time.Year.ToString() + "-" + time.Month.ToString() + "-" + time.Day.ToString() + "-" +
                 dayOfWeekNumber.ToString() + "-" + time.Hour.ToString() + ":" + time.Minute.ToString() + ":" + time.Second.ToString() + "\r\n";
                serialPort.Write(timeCommand);
            }
        }
        private void btn_Analyze_Click(object sender, EventArgs e)
        {
            if (ofd_AnalyzeFile.ShowDialog() == DialogResult.OK)
            {
                analysisData = CSVDataAdapter.Fill(ofd_AnalyzeFile.FileName, false);
                tb_Console.AppendText("Loaded " + analysisData.Rows.Count.ToString() + " Rows \r\n");
                tb_Console.AppendText("From File: " + ofd_AnalyzeFile.FileName + " \r\n");
                Int32 maxTime = 0;

                DataTable convertedData = new DataTable("Converted_Data");
                convertedData.Columns.Add("TimeStamp", typeof(float));
                ImuEntry defaultValue = new ImuEntry(0.0, 0.0, 0.0); 
                for (int i = 0; i < 10; i++)
                {
                    //Add column with the IMU index. 
                    convertedData.Columns.Add(i.ToString(), typeof(string));
                    convertedData.Columns[i.ToString()].DefaultValue = (i+1).ToString();
                    string columnName = "IMU" + i.ToString();
                    convertedData.Columns.Add(columnName, typeof(ImuEntry));
                    convertedData.Columns[columnName].DefaultValue = defaultValue; //set default to all zeros 
                }

                Int32 startTime = Int32.Parse(analysisData.Rows[0][0].ToString());
                for (int i = 1; i < analysisData.Rows.Count; i++)
                {
                    Int32 val1 = Int32.Parse(analysisData.Rows[i][0].ToString());
                    Int32 val2 = Int32.Parse(analysisData.Rows[i - 1][0].ToString());
                    Int32 interval = val1 - val2;
                    if (interval > maxTime)
                    {
                        maxTime = interval;
                    }
                    DataRow row = convertedData.NewRow();
                    row[0] = (float)(val1 - startTime) / 1000; //convert to float
                    
                    for (int j = 1, k =1; j < 18; j += 2,k++)
                    {
                        row[j+1] = new ImuEntry(analysisData.Rows[i][k].ToString());
                    }
                    convertedData.Rows.Add(row);

                }
                tb_Console.AppendText("Max Interval" + maxTime.ToString() + "ms \r\n");
                if (sfd_ConvertedFile.ShowDialog() == DialogResult.OK)
                {
                    //have to create header for the file before writting it in. 
                    string line1 = Guid.NewGuid().ToString() + "\r\n";
                    string line2 = Guid.NewGuid().ToString() + "\r\n";
                    string line3 = Guid.NewGuid().ToString() + "\r\n";
                    StreamWriter writer = File.CreateText(sfd_ConvertedFile.FileName);
                    writer.Write(line1);
                    writer.Write(line2);
                    writer.Write(line3);
                    writer.Close(); 
                    CSVDataAdapter.Write(convertedData, false, sfd_ConvertedFile.FileName, true);
                }


            }
            //FileStream dataFile = new FileStream(ofd_AnalyzeFile.FileName, FileMode.Open);
        }
        //experimental encryption functions
        private byte rotlByte(byte value, int count)
        {
            const int CHAR_BIT = 8;
            const int mask = (CHAR_BIT * sizeof(byte) - 1);
            count &= mask;

            return (byte)((value << count) | (value >> ((-count) & mask)));
        }

        private byte rotrByte(byte value, int count)
        {
            const int CHAR_BIT = 8;
            const int mask = (CHAR_BIT * sizeof(byte) - 1);
            count &= mask;
            return (byte)((value >> count) | (value << ((-count) & mask)));
        }
        private void encrypt(ref byte[] data, int size)
        {
            int shift = 0;
            for (int i=0; i < size; i++)
            {
                shift = i % 7;
                if (shift == 0)
                {
                    shift = 3;
                }
                data[i] = rotlByte(data[i], shift);
            }
        }

        private void decrypt(ref byte[] data, int size)
        {
            int shift = 0;
            for (int i = 0; i < size; i++)
            {
                shift = i % 7;
                if (shift == 0)
                {
                    shift = 3;
                }
                data[i] = rotrByte(data[i], shift);
            }
        }

        private void btn_EncryptSettings_Click(object sender, EventArgs e)
        {
            if (ofd_AnalyzeFile.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    FileStream settingsFs = File.Open(ofd_AnalyzeFile.FileName, FileMode.Open);
                    byte[] data = new byte[500];
                    int totalBytesRead = settingsFs.Read(data, 0, 500);
                    settingsFs.Close(); 
                    encrypt(ref data, totalBytesRead);

                    //decrypt(ref data, totalBytesRead);  

                    if(sfd_ConvertedFile.ShowDialog() == DialogResult.OK)
                    {
                        FileStream encryptedSettings = File.Open(sfd_ConvertedFile.FileName, FileMode.Create);
                        byte[] header = new byte[2];
                        header[0] = (byte)'e';
                        header[1] = (byte)'e';
                        encryptedSettings.Write(header, 0, 2);
                        encryptedSettings.Write(data, 0, totalBytesRead);
                        encryptedSettings.Close(); 
                    }
                }
                catch
                {

                }
            }
        }
        private byte reverse(byte c)
        {
            int shift;
            int result = 0;
            for (shift = 0; shift < 8; shift++)
            {
                if ((c & (0x01 << shift)) > 0)
                    result |= ((byte)0x80 >> shift);
            }
            return (byte)result;
        }
        private byte[] bitReverseAllBytes(byte[] data)
        {
            byte[] reversedBytes = new byte[data.Length];

            for (int i = 0, j=0;j < reversedBytes.Length; i++,j++)
            {
                reversedBytes[j] = reverse(data[i]); 
            }
            return reversedBytes; 

        }
        private void btn_CreateFwBin_Click(object sender, EventArgs e)
        {
            if (ofd_AnalyzeFile.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    FileStream fwStream = File.Open(ofd_AnalyzeFile.FileName, FileMode.Open);
                    byte[] data = new byte[fwStream.Length];
                    int totalBytesRead = fwStream.Read(data, 0, data.Length);
                    fwStream.Close();

                    CRC_Calculator crcCal = new CRC_Calculator(InitialCrcValue.NonZero1);
                    CRCTool crcTool = new CRCTool();
                    crcTool.Init(CRCTool.CRCCode.CRC32);
                    ulong crcValue1 = crcTool.crcbitbybitfast(data); 
                    ushort crcValue2 = crcCal.ComputeChecksum(data);
                    ulong crcValue3 = crcTool.crcbitbybitfast(bitReverseAllBytes(data)); 
                    tb_Console.AppendText("CRC method 1 Calculated: " + crcValue1.ToString() + " \r\n");
                    tb_Console.AppendText("CRC method 2 Calculated: " + crcValue2.ToString() + " \r\n");
                    tb_Console.AppendText("CRC method 3 Calculated: " + crcValue3.ToString() + " \r\n");
                    //header 0x55AA55AA CRC(16bit) CRC(16bit), Length(32bit)
                    byte[] header = { 0x55, 0xAA, 0x55, 0xAA, 0, 0, 0, 0, 0, 0, 0, 0};

                    header[4] = (byte)(crcValue1 & 0x00FF);
                    header[5] = (byte)((crcValue1 >> 8) & 0x00FF);
                    header[6] = (byte)((crcValue1 >> 16) & 0x00FF);
                    header[7] = (byte)((crcValue1 >> 24) & 0x00FF);


                    //decrypt(ref data, totalBytesRead);  

                    if (sfd_ConvertedFile.ShowDialog() == DialogResult.OK)
                    {
                        FileStream outputFw = File.Open(sfd_ConvertedFile.FileName, FileMode.Create);
                        outputFw.Write(header, 0, header.Length);
                        outputFw.Write(data, 0, totalBytesRead);
                        outputFw.Close();
                    }
                }
                catch
                {

                }
            }
        }

        private void dgv_SensorStats_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            Console.WriteLine("data Error"); 
        }

        private void btn_clearStats_Click(object sender, EventArgs e)
        {
            for(int i = 0; i < imuArray.Length; i++)
            {
                imuArray[i].clearStats(); 
            }
        }


    }
}



