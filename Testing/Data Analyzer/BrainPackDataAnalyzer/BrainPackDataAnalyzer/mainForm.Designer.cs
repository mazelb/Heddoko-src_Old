namespace BrainPackDataAnalyzer
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
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.cb_serialPorts = new System.Windows.Forms.ComboBox();
            this.l_COM_Port = new System.Windows.Forms.Label();
            this.bnt_Connect = new System.Windows.Forms.Button();
            this.btn_SendCmd = new System.Windows.Forms.Button();
            this.tb_cmd = new System.Windows.Forms.TextBox();
            this.btn_Analyze = new System.Windows.Forms.Button();
            this.ofd_AnalyzeFile = new System.Windows.Forms.OpenFileDialog();
            this.bindingSource1 = new System.Windows.Forms.BindingSource(this.components);
            this.tb_Console = new System.Windows.Forms.TextBox();
            this.sfd_ConvertedFile = new System.Windows.Forms.SaveFileDialog();
            this.tb_imu0_y = new System.Windows.Forms.TextBox();
            this.l_imu0 = new System.Windows.Forms.Label();
            this.tb_imu0_p = new System.Windows.Forms.TextBox();
            this.tb_imu0_r = new System.Windows.Forms.TextBox();
            this.btn_EncryptSettings = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).BeginInit();
            this.SuspendLayout();
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            // 
            // cb_serialPorts
            // 
            this.cb_serialPorts.FormattingEnabled = true;
            this.cb_serialPorts.Location = new System.Drawing.Point(222, 280);
            this.cb_serialPorts.Name = "cb_serialPorts";
            this.cb_serialPorts.Size = new System.Drawing.Size(161, 21);
            this.cb_serialPorts.TabIndex = 1;
            // 
            // l_COM_Port
            // 
            this.l_COM_Port.AutoSize = true;
            this.l_COM_Port.Location = new System.Drawing.Point(148, 287);
            this.l_COM_Port.Name = "l_COM_Port";
            this.l_COM_Port.Size = new System.Drawing.Size(53, 13);
            this.l_COM_Port.TabIndex = 2;
            this.l_COM_Port.Text = "COM Port";
            // 
            // bnt_Connect
            // 
            this.bnt_Connect.Location = new System.Drawing.Point(403, 277);
            this.bnt_Connect.Name = "bnt_Connect";
            this.bnt_Connect.Size = new System.Drawing.Size(117, 23);
            this.bnt_Connect.TabIndex = 3;
            this.bnt_Connect.Text = "Connect";
            this.bnt_Connect.UseVisualStyleBackColor = true;
            this.bnt_Connect.Click += new System.EventHandler(this.bnt_Connect_Click);
            // 
            // btn_SendCmd
            // 
            this.btn_SendCmd.Location = new System.Drawing.Point(403, 326);
            this.btn_SendCmd.Name = "btn_SendCmd";
            this.btn_SendCmd.Size = new System.Drawing.Size(117, 23);
            this.btn_SendCmd.TabIndex = 4;
            this.btn_SendCmd.Text = "Send Command";
            this.btn_SendCmd.UseVisualStyleBackColor = true;
            this.btn_SendCmd.Click += new System.EventHandler(this.btn_SendCmd_Click);
            // 
            // tb_cmd
            // 
            this.tb_cmd.Location = new System.Drawing.Point(222, 326);
            this.tb_cmd.Name = "tb_cmd";
            this.tb_cmd.Size = new System.Drawing.Size(161, 20);
            this.tb_cmd.TabIndex = 5;
            // 
            // btn_Analyze
            // 
            this.btn_Analyze.Location = new System.Drawing.Point(403, 380);
            this.btn_Analyze.Name = "btn_Analyze";
            this.btn_Analyze.Size = new System.Drawing.Size(117, 23);
            this.btn_Analyze.TabIndex = 6;
            this.btn_Analyze.Text = "Analyze File";
            this.btn_Analyze.UseVisualStyleBackColor = true;
            this.btn_Analyze.Click += new System.EventHandler(this.btn_Analyze_Click);
            // 
            // ofd_AnalyzeFile
            // 
            this.ofd_AnalyzeFile.DefaultExt = "csv";
            this.ofd_AnalyzeFile.FileName = "movementData.csv";
            // 
            // tb_Console
            // 
            this.tb_Console.Location = new System.Drawing.Point(65, 29);
            this.tb_Console.Multiline = true;
            this.tb_Console.Name = "tb_Console";
            this.tb_Console.Size = new System.Drawing.Size(455, 218);
            this.tb_Console.TabIndex = 7;
            // 
            // tb_imu0_y
            // 
            this.tb_imu0_y.Location = new System.Drawing.Point(85, 383);
            this.tb_imu0_y.Name = "tb_imu0_y";
            this.tb_imu0_y.Size = new System.Drawing.Size(71, 20);
            this.tb_imu0_y.TabIndex = 8;
            // 
            // l_imu0
            // 
            this.l_imu0.AutoSize = true;
            this.l_imu0.Location = new System.Drawing.Point(28, 390);
            this.l_imu0.Name = "l_imu0";
            this.l_imu0.Size = new System.Drawing.Size(29, 13);
            this.l_imu0.TabIndex = 9;
            this.l_imu0.Text = "imu0";
            // 
            // tb_imu0_p
            // 
            this.tb_imu0_p.Location = new System.Drawing.Point(85, 419);
            this.tb_imu0_p.Name = "tb_imu0_p";
            this.tb_imu0_p.Size = new System.Drawing.Size(71, 20);
            this.tb_imu0_p.TabIndex = 10;
            // 
            // tb_imu0_r
            // 
            this.tb_imu0_r.Location = new System.Drawing.Point(85, 456);
            this.tb_imu0_r.Name = "tb_imu0_r";
            this.tb_imu0_r.Size = new System.Drawing.Size(71, 20);
            this.tb_imu0_r.TabIndex = 11;
            // 
            // btn_EncryptSettings
            // 
            this.btn_EncryptSettings.Location = new System.Drawing.Point(403, 434);
            this.btn_EncryptSettings.Name = "btn_EncryptSettings";
            this.btn_EncryptSettings.Size = new System.Drawing.Size(117, 23);
            this.btn_EncryptSettings.TabIndex = 12;
            this.btn_EncryptSettings.Text = "Encrypt Settings";
            this.btn_EncryptSettings.UseVisualStyleBackColor = true;
            this.btn_EncryptSettings.Click += new System.EventHandler(this.btn_EncryptSettings_Click);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(588, 528);
            this.Controls.Add(this.btn_EncryptSettings);
            this.Controls.Add(this.tb_imu0_r);
            this.Controls.Add(this.tb_imu0_p);
            this.Controls.Add(this.l_imu0);
            this.Controls.Add(this.tb_imu0_y);
            this.Controls.Add(this.tb_Console);
            this.Controls.Add(this.btn_Analyze);
            this.Controls.Add(this.tb_cmd);
            this.Controls.Add(this.btn_SendCmd);
            this.Controls.Add(this.bnt_Connect);
            this.Controls.Add(this.l_COM_Port);
            this.Controls.Add(this.cb_serialPorts);
            this.Name = "mainForm";
            this.Text = "Brain Data Analyzer";
            this.Load += new System.EventHandler(this.mainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.ComboBox cb_serialPorts;
        private System.Windows.Forms.Label l_COM_Port;
        private System.Windows.Forms.Button bnt_Connect;
        private System.Windows.Forms.Button btn_SendCmd;
        private System.Windows.Forms.TextBox tb_cmd;
        private System.Windows.Forms.Button btn_Analyze;
        private System.Windows.Forms.OpenFileDialog ofd_AnalyzeFile;
        private System.Windows.Forms.BindingSource bindingSource1;
        private System.Windows.Forms.TextBox tb_Console;
        private System.Windows.Forms.SaveFileDialog sfd_ConvertedFile;
        private System.Windows.Forms.TextBox tb_imu0_y;
        private System.Windows.Forms.Label l_imu0;
        private System.Windows.Forms.TextBox tb_imu0_p;
        private System.Windows.Forms.TextBox tb_imu0_r;
        private System.Windows.Forms.Button btn_EncryptSettings;
    }
}

