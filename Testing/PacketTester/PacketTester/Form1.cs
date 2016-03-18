using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace PacketTester
{
    public partial class mainForm : Form
    {
        private bool openSerialPort = false;
        private bool processDebugThreadEnabled = false;
        public ConcurrentQueue<string> debugMessageQueue;
        private bool processPacketQueueEnabled = false; 
        public ConcurrentQueue<RawPacket> packetQueue;

        private bool streamDataEnabled = false;
        private long startTime = 0;
        private UInt16 graphIndex = 0;
        private const UInt16 graphMaxSize = 100; 

        public mainForm()
        {
            InitializeComponent();
        }

        public void processDebugMessagesThread()
        {
            while (processDebugThreadEnabled)
            {
                string line;
                if (debugMessageQueue.Count > 0)
                {
                    if (debugMessageQueue.TryDequeue(out line))
                    {
                        this.BeginInvoke((MethodInvoker)(() => tb_Console.AppendText(line)));
                    }
                }
            }
        }
        public void streamDataThread()
        {
            startTime = (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond);
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qx"].Points.Clear()));
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qy"].Points.Clear()));
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qz"].Points.Clear()));
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qw"].Points.Clear()));
            
            graphIndex = 0;
            while (streamDataEnabled)
            {
                sendUpdateCommand();
                Thread.Sleep(5);
                sendGetFrameCommand(0);
                Thread.Sleep(5); 
            }
        }
        
        enum CommandIds {update=0x11,getFrame,getFrameResp,setupMode,buttonPress,setImuId,setImuIdResp};
        string processSensorSerialNumber(RawPacket packet)
        {
            StringBuilder strBuilder = new StringBuilder();
            if(packet.PayloadSize != 18)
            {
                return "Failed to parse serial Number";
            }
            strBuilder.Append("0x");
            for(int i = 0; i < 16; i++)
            {
                strBuilder.Append(string.Format("{0}", packet.Payload[i + 2]));
            }
            return strBuilder.ToString();
        }
        private void updateChart(ImuFrame frame)
        {
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qx"].Points.AddY((double)frame.Quaternion_x)));
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qy"].Points.AddY((double)frame.Quaternion_y)));
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qz"].Points.AddY((double)frame.Quaternion_z)));
            this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qw"].Points.AddY((double)frame.Quaternion_w)));
            if (chrt_dataChart.Series["Qx"].Points.Count > graphMaxSize)
            {
                this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qx"].Points.RemoveAt(0)));
                this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qy"].Points.RemoveAt(0)));
                this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qz"].Points.RemoveAt(0)));
                this.BeginInvoke((MethodInvoker)(() => chrt_dataChart.Series["Qw"].Points.RemoveAt(0)));

            }
        }
        private void processPacket(RawPacket packet)
        {
            //check that the packet comes from an IMU sensor
            if (packet.Payload[0] == 0x03)
            {
                CommandIds commandId = (CommandIds)packet.Payload[1];
                switch(commandId)
                {
                    case CommandIds.buttonPress:
                        debugMessageQueue.Enqueue(String.Format("{0}:Received Button Press for serial#{1}\r\n", (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond), processSensorSerialNumber(packet)));
                        break;
                    case CommandIds.getFrameResp:
                        //debugMessageQueue.Enqueue(String.Format("{0}:Received Frame\r\n", (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond)));

                        ImuFrame frame = new ImuFrame(packet);
                        updateChart(frame);
                        //debugMessageQueue.Enqueue(frame.ToString());
                        break;
                    case CommandIds.setImuIdResp:
                        debugMessageQueue.Enqueue(String.Format("{0}:Received set Imu Resp\r\n", (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond)));
                        break;
                    default:
                        break;
                }
            }
            
        }

        public void processPacketThread()
        {
            while(processPacketQueueEnabled)
            {
                RawPacket packet;
                if(packetQueue.Count > 0)
                {
                    if(packetQueue.TryDequeue(out packet))
                    {
                        processPacket(packet);
                    }
                }
            }
        }
        private void mainForm_Load(object sender, EventArgs e)
        {
            cb_serialPorts.Items.AddRange(SerialPort.GetPortNames());
            string[] baudrates = { "110", "150", "300", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "230400"
                    , "460800","500000", "921600","1000000"};
            cb_BaudRate.Items.AddRange(baudrates);
            cb_BaudRate.SelectedIndex = 12;
            //initialize the message queue
            debugMessageQueue = new ConcurrentQueue<string>();
            //start the debug message thread
            processDebugThreadEnabled = true;
            Thread debugMessageThread = new Thread(processDebugMessagesThread);
            debugMessageThread.Start();

            packetQueue = new ConcurrentQueue<RawPacket>();
            processPacketQueueEnabled = true; 
            Thread packetProcessorThread = new Thread(processPacketThread);
            packetProcessorThread.Start();


        }

        private void bnt_Connect_Click(object sender, EventArgs e)
        {
            //set the serial port to the selected item. 
            //Thread serialThread = new Thread(ReadThread);
            if (serialPort.IsOpen)
            {
                //do nothing
                return;
            }

            serialPort.PortName = cb_serialPorts.Items[cb_serialPorts.SelectedIndex].ToString();
            serialPort.BaudRate = int.Parse(cb_BaudRate.Items[cb_BaudRate.SelectedIndex].ToString()); 
            try
            {
                openSerialPort = true;
                serialPort.Open();

                tb_Console.AppendText("Port: " + serialPort.PortName + " Open\r\n");
            }
            catch (Exception ex)
            {
                tb_Console.AppendText("Failed to open Port: " + serialPort.PortName + " \r\n");
                tb_Console.AppendText("Exception " + ex.Message + " \r\n");
                openSerialPort = false;
            }
        }

        private void btn_disconnect_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                try
                {                    
                    serialPort.Close();
                    openSerialPort = false;
                    tb_Console.AppendText("Port: " + serialPort.PortName + " Closed\r\n");
                }
                catch
                {
                    tb_Console.AppendText("Failed to close Port: " + serialPort.PortName + "\r\n");
                }
            }
        }




        RawPacket packet = new RawPacket();
        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            
            while(serialPort.BytesToRead > 0)
            {
                int receivedByte = serialPort.ReadByte();
                if (receivedByte != -1)
                {                    
                    //process the byte
                    byte newByte = (byte)receivedByte;
                    int bytesReceived = packet.BytesReceived + 1;
                    PacketStatus status = packet.processByte((byte)receivedByte);
                    switch(status)
                    {
                        case PacketStatus.PacketComplete:                        
                            //debugMessageQueue.Enqueue(String.Format("{0} Packet Received {1} bytes\r\n", (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond), packet.PayloadSize));
                            RawPacket packetCopy = new RawPacket(packet);  
                            packetQueue.Enqueue(packetCopy); 
                            packet.resetPacket(); 
                            break;
                        case PacketStatus.PacketError:
                            debugMessageQueue.Enqueue(String.Format("{0} Packet ERROR! {1} bytes received\r\n", (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond), bytesReceived));
                            packet.resetPacket(); 
                            break;
                        case PacketStatus.Processing:
                        case PacketStatus.newPacketDetected:
                            break;
                    }
                }
            }

        }
        private void sendPacket(byte[] payload, UInt16 size)
        {
            RawPacket packetToSend = new RawPacket(payload, size);
            UInt16 rawPacketSize = 0;
            byte[] rawPacketBytes = packetToSend.createRawPacket(ref rawPacketSize);
            serialPort.Write(rawPacketBytes, 0, rawPacketSize); 
        }

        private void btn_SendCmd_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                if (tb_cmd.Text.Length > 0)
                {
                    byte[] payload = Encoding.ASCII.GetBytes(tb_cmd.Text);
                    sendPacket(payload, (UInt16)payload.Length); 
                }
            }
        }

        private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //stop debug thread
            processDebugThreadEnabled = false;
            processPacketQueueEnabled = false; 
            //close the serial port
            if (serialPort.IsOpen)
            {
                serialPort.Close(); 
            }
        }
        private void sendGetFrameCommand(byte sensorId)
        {
            if (serialPort.IsOpen)
            {
                byte[] getFrameBytes = new byte[3];
                getFrameBytes[0] = 0x01;
                getFrameBytes[1] = 0x12;
                getFrameBytes[2] = sensorId;
                sendPacket(getFrameBytes, 3);
            }
        }
        private void btn_getFrame_Click(object sender, EventArgs e)
        {
            sendGetFrameCommand(0x00);
        }

        private void btn_SetupMode_Click(object sender, EventArgs e)
        {
            byte[] setupModeBytes = new byte[3];
            setupModeBytes[0] = 0x01;
            setupModeBytes[1] = 0x14;
            if (cb_SetupModeEn.Checked)
            {
                setupModeBytes[2] = 0x01;
            }
            else
            {
                setupModeBytes[2] = 0x00;
            }
            if (serialPort.IsOpen)
            {
                sendPacket(setupModeBytes, 3);
            }

        }
        private void sendUpdateCommand()
        {
            if (serialPort.IsOpen)
            {
                byte[] getFrameBytes = new byte[3];
                getFrameBytes[0] = 0x01;
                getFrameBytes[1] = 0x11;
                sendPacket(getFrameBytes, 2);
            }
        }
        private void btn_sendUpdateCmd_Click(object sender, EventArgs e)
        {
            sendUpdateCommand();
        }

        private void cb_enableStream_CheckedChanged(object sender, EventArgs e)
        {
            if(cb_enableStream.Checked)
            {
                Thread streamThread = new Thread(streamDataThread);
                streamDataEnabled = true; 
                streamThread.Start(); 
            }
            else
            {
                streamDataEnabled = false; 
            }
        }

        private void tb_y_max_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = !(char.IsDigit(e.KeyChar) || (e.KeyChar == '.') ||(e.KeyChar == '-' ) || (e.KeyChar == '\b'));
        }

        private void tb_y_min_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = !(char.IsDigit(e.KeyChar) || (e.KeyChar == '.') || (e.KeyChar == '-') || (e.KeyChar == '\b'));
        }

        private void btn_setAxis_Click(object sender, EventArgs e)
        {
            try
            {
                float yaxis_max = 0.0F, yaxis_min = 0.0F;
                if(float.TryParse(tb_y_max.Text,out yaxis_max))
                {
                    chrt_dataChart.ChartAreas[0].AxisY.Maximum = yaxis_max;
                }
                if (float.TryParse(tb_y_min.Text, out yaxis_min))
                {
                    chrt_dataChart.ChartAreas[0].AxisY.Minimum = yaxis_min;
                }
                chrt_dataChart.ChartAreas[0].AxisY.RoundAxisValues();
            }
            catch
            {
                debugMessageQueue.Enqueue(String.Format("{0} Failed to parse Axis values (must be decimal number)\r\n", (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond)));
            }
        }

        private void chrt_dataChart_Click(object sender, EventArgs e)
        {

        }
    }
}
