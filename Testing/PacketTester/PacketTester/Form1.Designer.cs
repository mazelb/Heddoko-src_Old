namespace PacketTester
{
    partial class mainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.btn_disconnect = new System.Windows.Forms.Button();
            this.tb_Console = new System.Windows.Forms.TextBox();
            this.tb_cmd = new System.Windows.Forms.TextBox();
            this.btn_SendCmd = new System.Windows.Forms.Button();
            this.bnt_Connect = new System.Windows.Forms.Button();
            this.cb_serialPorts = new System.Windows.Forms.ComboBox();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.cb_BaudRate = new System.Windows.Forms.ComboBox();
            this.btn_getFrame = new System.Windows.Forms.Button();
            this.btn_SetupMode = new System.Windows.Forms.Button();
            this.cb_SetupModeEn = new System.Windows.Forms.CheckBox();
            this.btn_sendUpdateCmd = new System.Windows.Forms.Button();
            this.cb_enableStream = new System.Windows.Forms.CheckBox();
            this.chrt_dataChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.tb_y_max = new System.Windows.Forms.TextBox();
            this.tb_y_min = new System.Windows.Forms.TextBox();
            this.lb_y_max = new System.Windows.Forms.Label();
            this.lb_y_min = new System.Windows.Forms.Label();
            this.btn_setAxis = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.chrt_dataChart)).BeginInit();
            this.SuspendLayout();
            // 
            // btn_disconnect
            // 
            this.btn_disconnect.Location = new System.Drawing.Point(225, 297);
            this.btn_disconnect.Name = "btn_disconnect";
            this.btn_disconnect.Size = new System.Drawing.Size(73, 23);
            this.btn_disconnect.TabIndex = 21;
            this.btn_disconnect.Text = "Disconnect";
            this.btn_disconnect.UseVisualStyleBackColor = true;
            this.btn_disconnect.Click += new System.EventHandler(this.btn_disconnect_Click);
            // 
            // tb_Console
            // 
            this.tb_Console.Location = new System.Drawing.Point(40, 30);
            this.tb_Console.Multiline = true;
            this.tb_Console.Name = "tb_Console";
            this.tb_Console.ReadOnly = true;
            this.tb_Console.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tb_Console.Size = new System.Drawing.Size(455, 218);
            this.tb_Console.TabIndex = 20;
            // 
            // tb_cmd
            // 
            this.tb_cmd.Location = new System.Drawing.Point(40, 342);
            this.tb_cmd.Name = "tb_cmd";
            this.tb_cmd.Size = new System.Drawing.Size(161, 20);
            this.tb_cmd.TabIndex = 19;
            this.tb_cmd.Text = "?";
            // 
            // btn_SendCmd
            // 
            this.btn_SendCmd.Location = new System.Drawing.Point(230, 339);
            this.btn_SendCmd.Name = "btn_SendCmd";
            this.btn_SendCmd.Size = new System.Drawing.Size(117, 23);
            this.btn_SendCmd.TabIndex = 18;
            this.btn_SendCmd.Text = "Send Command";
            this.btn_SendCmd.UseVisualStyleBackColor = true;
            this.btn_SendCmd.Click += new System.EventHandler(this.btn_SendCmd_Click);
            // 
            // bnt_Connect
            // 
            this.bnt_Connect.Location = new System.Drawing.Point(225, 268);
            this.bnt_Connect.Name = "bnt_Connect";
            this.bnt_Connect.Size = new System.Drawing.Size(73, 23);
            this.bnt_Connect.TabIndex = 17;
            this.bnt_Connect.Text = "Connect";
            this.bnt_Connect.UseVisualStyleBackColor = true;
            this.bnt_Connect.Click += new System.EventHandler(this.bnt_Connect_Click);
            // 
            // cb_serialPorts
            // 
            this.cb_serialPorts.FormattingEnabled = true;
            this.cb_serialPorts.Location = new System.Drawing.Point(40, 268);
            this.cb_serialPorts.Name = "cb_serialPorts";
            this.cb_serialPorts.Size = new System.Drawing.Size(161, 21);
            this.cb_serialPorts.TabIndex = 16;
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            this.serialPort.ReadTimeout = 500;
            this.serialPort.WriteTimeout = 500;
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            // 
            // cb_BaudRate
            // 
            this.cb_BaudRate.FormattingEnabled = true;
            this.cb_BaudRate.Location = new System.Drawing.Point(339, 268);
            this.cb_BaudRate.Name = "cb_BaudRate";
            this.cb_BaudRate.Size = new System.Drawing.Size(156, 21);
            this.cb_BaudRate.TabIndex = 22;
            // 
            // btn_getFrame
            // 
            this.btn_getFrame.Location = new System.Drawing.Point(40, 413);
            this.btn_getFrame.Name = "btn_getFrame";
            this.btn_getFrame.Size = new System.Drawing.Size(75, 23);
            this.btn_getFrame.TabIndex = 23;
            this.btn_getFrame.Text = "Get Frame";
            this.btn_getFrame.UseVisualStyleBackColor = true;
            this.btn_getFrame.Click += new System.EventHandler(this.btn_getFrame_Click);
            // 
            // btn_SetupMode
            // 
            this.btn_SetupMode.Location = new System.Drawing.Point(40, 442);
            this.btn_SetupMode.Name = "btn_SetupMode";
            this.btn_SetupMode.Size = new System.Drawing.Size(103, 23);
            this.btn_SetupMode.TabIndex = 24;
            this.btn_SetupMode.Text = "Send Setup Mode";
            this.btn_SetupMode.UseVisualStyleBackColor = true;
            this.btn_SetupMode.Click += new System.EventHandler(this.btn_SetupMode_Click);
            // 
            // cb_SetupModeEn
            // 
            this.cb_SetupModeEn.AutoSize = true;
            this.cb_SetupModeEn.Location = new System.Drawing.Point(156, 446);
            this.cb_SetupModeEn.Name = "cb_SetupModeEn";
            this.cb_SetupModeEn.Size = new System.Drawing.Size(65, 17);
            this.cb_SetupModeEn.TabIndex = 25;
            this.cb_SetupModeEn.Text = "Enabled";
            this.cb_SetupModeEn.UseVisualStyleBackColor = true;
            // 
            // btn_sendUpdateCmd
            // 
            this.btn_sendUpdateCmd.Location = new System.Drawing.Point(40, 472);
            this.btn_sendUpdateCmd.Name = "btn_sendUpdateCmd";
            this.btn_sendUpdateCmd.Size = new System.Drawing.Size(75, 23);
            this.btn_sendUpdateCmd.TabIndex = 26;
            this.btn_sendUpdateCmd.Text = "Update";
            this.btn_sendUpdateCmd.UseVisualStyleBackColor = true;
            this.btn_sendUpdateCmd.Click += new System.EventHandler(this.btn_sendUpdateCmd_Click);
            // 
            // cb_enableStream
            // 
            this.cb_enableStream.AutoSize = true;
            this.cb_enableStream.Location = new System.Drawing.Point(156, 477);
            this.cb_enableStream.Name = "cb_enableStream";
            this.cb_enableStream.Size = new System.Drawing.Size(59, 17);
            this.cb_enableStream.TabIndex = 27;
            this.cb_enableStream.Text = "Stream";
            this.cb_enableStream.UseVisualStyleBackColor = true;
            this.cb_enableStream.CheckedChanged += new System.EventHandler(this.cb_enableStream_CheckedChanged);
            // 
            // chrt_dataChart
            // 
            chartArea1.AxisY.Maximum = 1.1D;
            chartArea1.AxisY.Minimum = -1.1D;
            chartArea1.Name = "ChartArea1";
            this.chrt_dataChart.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chrt_dataChart.Legends.Add(legend1);
            this.chrt_dataChart.Location = new System.Drawing.Point(541, 30);
            this.chrt_dataChart.Name = "chrt_dataChart";
            this.chrt_dataChart.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.Bright;
            series1.BorderWidth = 5;
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.Legend = "Legend1";
            series1.Name = "Qx";
            series1.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Single;
            series2.BorderWidth = 5;
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series2.Legend = "Legend1";
            series2.Name = "Qy";
            series3.BorderWidth = 5;
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series3.Legend = "Legend1";
            series3.Name = "Qz";
            series4.BorderWidth = 5;
            series4.ChartArea = "ChartArea1";
            series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series4.Legend = "Legend1";
            series4.Name = "Qw";
            this.chrt_dataChart.Series.Add(series1);
            this.chrt_dataChart.Series.Add(series2);
            this.chrt_dataChart.Series.Add(series3);
            this.chrt_dataChart.Series.Add(series4);
            this.chrt_dataChart.Size = new System.Drawing.Size(589, 487);
            this.chrt_dataChart.TabIndex = 28;
            this.chrt_dataChart.Text = "chart1";
            this.chrt_dataChart.Click += new System.EventHandler(this.chrt_dataChart_Click);
            // 
            // tb_y_max
            // 
            this.tb_y_max.Location = new System.Drawing.Point(351, 444);
            this.tb_y_max.Name = "tb_y_max";
            this.tb_y_max.Size = new System.Drawing.Size(100, 20);
            this.tb_y_max.TabIndex = 29;
            this.tb_y_max.Text = "1.1";
            this.tb_y_max.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tb_y_max.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb_y_max_KeyPress);
            // 
            // tb_y_min
            // 
            this.tb_y_min.Location = new System.Drawing.Point(351, 474);
            this.tb_y_min.Name = "tb_y_min";
            this.tb_y_min.Size = new System.Drawing.Size(100, 20);
            this.tb_y_min.TabIndex = 30;
            this.tb_y_min.Text = "-1.1";
            this.tb_y_min.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tb_y_min.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb_y_min_KeyPress);
            // 
            // lb_y_max
            // 
            this.lb_y_max.AutoSize = true;
            this.lb_y_max.Location = new System.Drawing.Point(310, 451);
            this.lb_y_max.Name = "lb_y_max";
            this.lb_y_max.Size = new System.Drawing.Size(37, 13);
            this.lb_y_max.TabIndex = 31;
            this.lb_y_max.Text = "Y-Max";
            // 
            // lb_y_min
            // 
            this.lb_y_min.AutoSize = true;
            this.lb_y_min.Location = new System.Drawing.Point(310, 481);
            this.lb_y_min.Name = "lb_y_min";
            this.lb_y_min.Size = new System.Drawing.Size(34, 13);
            this.lb_y_min.TabIndex = 32;
            this.lb_y_min.Text = "Y-Min";
            // 
            // btn_setAxis
            // 
            this.btn_setAxis.Location = new System.Drawing.Point(351, 510);
            this.btn_setAxis.Name = "btn_setAxis";
            this.btn_setAxis.Size = new System.Drawing.Size(100, 23);
            this.btn_setAxis.TabIndex = 33;
            this.btn_setAxis.Text = "Set Y Axis";
            this.btn_setAxis.UseVisualStyleBackColor = true;
            this.btn_setAxis.Click += new System.EventHandler(this.btn_setAxis_Click);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1142, 564);
            this.Controls.Add(this.btn_setAxis);
            this.Controls.Add(this.lb_y_min);
            this.Controls.Add(this.lb_y_max);
            this.Controls.Add(this.tb_y_min);
            this.Controls.Add(this.tb_y_max);
            this.Controls.Add(this.chrt_dataChart);
            this.Controls.Add(this.cb_enableStream);
            this.Controls.Add(this.btn_sendUpdateCmd);
            this.Controls.Add(this.cb_SetupModeEn);
            this.Controls.Add(this.btn_SetupMode);
            this.Controls.Add(this.btn_getFrame);
            this.Controls.Add(this.cb_BaudRate);
            this.Controls.Add(this.btn_disconnect);
            this.Controls.Add(this.tb_Console);
            this.Controls.Add(this.tb_cmd);
            this.Controls.Add(this.btn_SendCmd);
            this.Controls.Add(this.bnt_Connect);
            this.Controls.Add(this.cb_serialPorts);
            this.Name = "mainForm";
            this.Text = "Sean\'s Packet Tester";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainForm_FormClosing);
            this.Load += new System.EventHandler(this.mainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chrt_dataChart)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_disconnect;
        private System.Windows.Forms.TextBox tb_Console;
        private System.Windows.Forms.TextBox tb_cmd;
        private System.Windows.Forms.Button btn_SendCmd;
        private System.Windows.Forms.Button bnt_Connect;
        private System.Windows.Forms.ComboBox cb_serialPorts;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.ComboBox cb_BaudRate;
        private System.Windows.Forms.Button btn_getFrame;
        private System.Windows.Forms.Button btn_SetupMode;
        private System.Windows.Forms.CheckBox cb_SetupModeEn;
        private System.Windows.Forms.Button btn_sendUpdateCmd;
        private System.Windows.Forms.CheckBox cb_enableStream;
        private System.Windows.Forms.DataVisualization.Charting.Chart chrt_dataChart;
        private System.Windows.Forms.TextBox tb_y_max;
        private System.Windows.Forms.TextBox tb_y_min;
        private System.Windows.Forms.Label lb_y_max;
        private System.Windows.Forms.Label lb_y_min;
        private System.Windows.Forms.Button btn_setAxis;
    }
}

