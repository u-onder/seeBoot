namespace FProg
{
    partial class Form1
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
            this.cbPort = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.btnOpenPort = new System.Windows.Forms.Button();
            this.tbDebug = new System.Windows.Forms.TextBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.btnOpenFile = new System.Windows.Forms.Button();
            this.tbXmlFile = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnPIN = new System.Windows.Forms.Button();
            this.btnBoot = new System.Windows.Forms.Button();
            this.btnProg = new System.Windows.Forms.Button();
            this.btnVerify = new System.Windows.Forms.Button();
            this.btnErase = new System.Windows.Forms.Button();
            this.MenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.Separator1 = new System.Windows.Forms.ToolStripSeparator();
            this.MenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.MenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.MenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // cbPort
            // 
            this.cbPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbPort.FormattingEnabled = true;
            this.cbPort.Location = new System.Drawing.Point(45, 12);
            this.cbPort.Name = "cbPort";
            this.cbPort.Size = new System.Drawing.Size(87, 21);
            this.cbPort.TabIndex = 0;
            this.cbPort.Click += new System.EventHandler(this.cbPort_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Port";
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // btnOpenPort
            // 
            this.btnOpenPort.Location = new System.Drawing.Point(138, 11);
            this.btnOpenPort.Name = "btnOpenPort";
            this.btnOpenPort.Size = new System.Drawing.Size(75, 23);
            this.btnOpenPort.TabIndex = 2;
            this.btnOpenPort.Text = "Open";
            this.btnOpenPort.UseVisualStyleBackColor = true;
            this.btnOpenPort.Click += new System.EventHandler(this.btnOpenPort_Click);
            // 
            // tbDebug
            // 
            this.tbDebug.Location = new System.Drawing.Point(13, 118);
            this.tbDebug.Multiline = true;
            this.tbDebug.Name = "tbDebug";
            this.tbDebug.ReadOnly = true;
            this.tbDebug.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbDebug.Size = new System.Drawing.Size(623, 171);
            this.tbDebug.TabIndex = 3;
            this.tbDebug.TextChanged += new System.EventHandler(this.tbDebug_TextChanged);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // btnOpenFile
            // 
            this.btnOpenFile.Location = new System.Drawing.Point(561, 38);
            this.btnOpenFile.Name = "btnOpenFile";
            this.btnOpenFile.Size = new System.Drawing.Size(75, 23);
            this.btnOpenFile.TabIndex = 4;
            this.btnOpenFile.Text = "Open";
            this.btnOpenFile.UseVisualStyleBackColor = true;
            this.btnOpenFile.Click += new System.EventHandler(this.btnOpenFile_Click);
            // 
            // tbXmlFile
            // 
            this.tbXmlFile.Location = new System.Drawing.Point(45, 40);
            this.tbXmlFile.Name = "tbXmlFile";
            this.tbXmlFile.ReadOnly = true;
            this.tbXmlFile.Size = new System.Drawing.Size(510, 20);
            this.tbXmlFile.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "XML";
            // 
            // btnPIN
            // 
            this.btnPIN.Enabled = false;
            this.btnPIN.Location = new System.Drawing.Point(77, 76);
            this.btnPIN.Name = "btnPIN";
            this.btnPIN.Size = new System.Drawing.Size(75, 23);
            this.btnPIN.TabIndex = 8;
            this.btnPIN.Text = "PIN";
            this.btnPIN.UseVisualStyleBackColor = true;
            this.btnPIN.Click += new System.EventHandler(this.btnPIN_Click);
            // 
            // btnBoot
            // 
            this.btnBoot.Enabled = false;
            this.btnBoot.Location = new System.Drawing.Point(449, 76);
            this.btnBoot.Name = "btnBoot";
            this.btnBoot.Size = new System.Drawing.Size(75, 23);
            this.btnBoot.TabIndex = 9;
            this.btnBoot.Text = "Boot";
            this.btnBoot.UseVisualStyleBackColor = true;
            this.btnBoot.Click += new System.EventHandler(this.btnBoot_Click);
            // 
            // btnProg
            // 
            this.btnProg.Enabled = false;
            this.btnProg.Location = new System.Drawing.Point(263, 76);
            this.btnProg.Name = "btnProg";
            this.btnProg.Size = new System.Drawing.Size(75, 23);
            this.btnProg.TabIndex = 10;
            this.btnProg.Text = "Program";
            this.btnProg.UseVisualStyleBackColor = true;
            this.btnProg.Click += new System.EventHandler(this.btnProg_Click);
            // 
            // btnVerify
            // 
            this.btnVerify.Enabled = false;
            this.btnVerify.Location = new System.Drawing.Point(356, 76);
            this.btnVerify.Name = "btnVerify";
            this.btnVerify.Size = new System.Drawing.Size(75, 23);
            this.btnVerify.TabIndex = 11;
            this.btnVerify.Text = "Verify";
            this.btnVerify.UseVisualStyleBackColor = true;
            this.btnVerify.Click += new System.EventHandler(this.btnVerify_Click);
            // 
            // btnErase
            // 
            this.btnErase.Enabled = false;
            this.btnErase.Location = new System.Drawing.Point(170, 76);
            this.btnErase.Name = "btnErase";
            this.btnErase.Size = new System.Drawing.Size(75, 23);
            this.btnErase.TabIndex = 12;
            this.btnErase.Text = "Erase";
            this.btnErase.UseVisualStyleBackColor = true;
            this.btnErase.Click += new System.EventHandler(this.btnErase_Click);
            // 
            // MenuItem1
            // 
            this.MenuItem1.Name = "MenuItem1";
            this.MenuItem1.Size = new System.Drawing.Size(124, 22);
            this.MenuItem1.Text = "Save as ...";
            // 
            // Separator1
            // 
            this.Separator1.Name = "Separator1";
            this.Separator1.Size = new System.Drawing.Size(121, 6);
            // 
            // MenuItem2
            // 
            this.MenuItem2.Name = "MenuItem2";
            this.MenuItem2.Size = new System.Drawing.Size(124, 22);
            this.MenuItem2.Text = "Clear";
            // 
            // MenuStrip1
            // 
            this.MenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItem1,
            this.Separator1,
            this.MenuItem2});
            this.MenuStrip1.Name = "MenuStrip1";
            this.MenuStrip1.Size = new System.Drawing.Size(125, 54);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(648, 301);
            this.Controls.Add(this.btnErase);
            this.Controls.Add(this.btnVerify);
            this.Controls.Add(this.btnProg);
            this.Controls.Add(this.btnBoot);
            this.Controls.Add(this.btnPIN);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbXmlFile);
            this.Controls.Add(this.btnOpenFile);
            this.Controls.Add(this.tbDebug);
            this.Controls.Add(this.btnOpenPort);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cbPort);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Form1";
            this.Text = "FProg [seeBoot]";
            this.MenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cbPort;
        private System.Windows.Forms.Label label1;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button btnOpenPort;
        private System.Windows.Forms.TextBox tbDebug;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button btnOpenFile;
        private System.Windows.Forms.TextBox tbXmlFile;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnPIN;
        private System.Windows.Forms.Button btnBoot;
        private System.Windows.Forms.Button btnProg;
        private System.Windows.Forms.Button btnVerify;
        private System.Windows.Forms.Button btnErase;
        private System.Windows.Forms.ToolStripMenuItem MenuItem1;
        private System.Windows.Forms.ToolStripSeparator Separator1;
        private System.Windows.Forms.ToolStripMenuItem MenuItem2;
        private System.Windows.Forms.ContextMenuStrip MenuStrip1;
    }
}

