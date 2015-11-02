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
                    string line = serialPort.ReadLine();
                    processEntry(line);

                }
            }
        }
        public void processEntry(string entry)
        {
            string[] entrySplit = entry.Split(',');
            if (entrySplit.Length > 1)
            {
                try
                {
                    UInt32 timeStamp = UInt32.Parse(entrySplit[0]);
                    ImuEntry imu0Data = new ImuEntry(entrySplit[1]);
                    this.Invoke((MethodInvoker)(() => tb_imu0_y.Text = imu0Data.Yaw.ToString("F3")));
                    this.Invoke((MethodInvoker)(() => tb_imu0_p.Text = imu0Data.Pitch.ToString("F3")));
                    this.Invoke((MethodInvoker)(() => tb_imu0_r.Text = imu0Data.Roll.ToString("F3")));
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
            readThread = new Thread(ReadThread);
            tb_Console.AppendText("Brain Data Analyzer\r\n");
        }

        private void bnt_Connect_Click(object sender, EventArgs e)
        {
            //set the serial port to the selected item. 
            serialPort.PortName = cb_serialPorts.Items[cb_serialPorts.SelectedIndex].ToString();
            serialPort.Open();
            readThread.Start();
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
                    string line1 = "DA920FFD-D8D0-436D-979D-48C73031F9F2\r\n";
                    string line2 = "FBBE8B37-151D-4D8A-8138-CC2BAAFA5BF7\r\n";
                    string line3 = "91D69812-3B9A-4831-9225-E86C6D6227FB\r\n";
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
    }
}



