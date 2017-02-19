using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Xml;

namespace FProg
{
    public partial class Form1 : Form
    {
        private String portLine;
        private String portLastLine;
        private XmlTextReader doc;

        static AutoResetEvent portNewLineEvent;

        public Form1()
        {
            InitializeComponent();

            TextBox.CheckForIllegalCrossThreadCalls = false;
            portNewLineEvent = new AutoResetEvent(false);

            tbDebug.ContextMenu = new ContextMenu();
            tbDebug.ContextMenu.MenuItems.Add(new MenuItem("Save as...", tbDebugSaveAsClick));
            tbDebug.ContextMenu.MenuItems.Add(new MenuItem("-"));
            tbDebug.ContextMenu.MenuItems.Add(new MenuItem("Clear", tbDebugClearClick));
        }

        private void cbPort_Click(object sender, EventArgs e)
        {
            cbPort.Items.Clear();
            cbPort.Items.AddRange(SerialPort.GetPortNames());
        }

        private void btnOpenPort_Click(object sender, EventArgs e)
        {
            if (btnOpenPort.Text == "Open")
            {
                try
                {
                    serialPort1.Parity = Parity.None;
                    serialPort1.BaudRate = 9600;
                    serialPort1.DataBits = 8;
                    serialPort1.Handshake = Handshake.None;
                    serialPort1.PortName = cbPort.SelectedItem.ToString();
                    serialPort1.Open();
                    tbDebug.Text += "> Port Open SUCCESS\r\n";
                    btnPIN.Enabled = true;
                    btnErase.Enabled = true;
                    btnProg.Enabled = true;
                    btnVerify.Enabled = true;
                    btnBoot.Enabled = true;

                    btnOpenPort.Text = "Close";
                }
                catch (Exception ex)
                {
                    tbDebug.Text += "> ERROR: Port Open FAILURE\r\n";
                }
            }
            else
            {
                try
                {
                    serialPort1.Close();
                    btnPIN.Enabled = false;
                    btnErase.Enabled = false;
                    btnProg.Enabled = false;
                    btnVerify.Enabled = false;
                    btnBoot.Enabled = false;
                    btnOpenPort.Text = "Open";
                }
                catch (Exception ex)
                {
                }
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            String str = serialPort1.ReadExisting();
            foreach (char c in str)
            {
                if (c == 'c')
                    portLine = "";

                if ((c != '\r') && (c != '\n'))
                {
                    portLine += c;
                }
                else
                {
                    if (portLine != "")
                    {
                        tbDebug.Text += "< " + portLine + "\r\n";
                        portLastLine = portLine;
                        portNewLineEvent.Set();
                        portLine = "";
                    }
                }
            }
        }

        private void btnOpenFile_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "image files (*.img)|*.img";
            openFileDialog1.FileName = "";
            openFileDialog1.Title = "Please select an image file";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                tbXmlFile.Text = "";
                try
                {
                    doc.Dispose();
                }
                catch (Exception ex)
                {
                }
                finally
                {
                    if (openFileDialog1.CheckFileExists == true)
                        if (Path.GetExtension(openFileDialog1.FileName) == ".img")
                        {
                            tbXmlFile.Text = openFileDialog1.FileName;
                            doc = new XmlTextReader(tbXmlFile.Text);

                            while (doc.Read())
                            {
                                if (doc.NodeType == XmlNodeType.Element)
                                {
                                    if (doc.Name == "FIRMWARE")
                                    {
                                        doc.MoveToFirstAttribute();
                                        tbDebug.Text += "> Found Firmware \"" + doc.Value + "\"\r\n";
                                        break;
                                    }
                                }
                            }
                        }
                }
            }
        }

        private void commSend(String str)
        {
            try
            {
                byte[] CRC = { 0 };

                CRC[0] = 0;
                foreach (byte c in str)
                    CRC[0] ^= c;
                String text = "c" + str + "." + BitConverter.ToString(CRC) + "\r\n";

                tbDebug.Text += "> " + text;
                
                serialPort1.Write(text);
            }
            catch (Exception ex)
            {
            }
        }

        private void tbDebug_TextChanged(object sender, EventArgs e)
        {
            tbDebug.SelectionStart = tbDebug.Text.Length;
            tbDebug.ScrollToCaret();
        }

        private void tbDebugSaveAsClick(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "Text File (*.txt)|*.txt";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                File.WriteAllText(dialog.FileName, tbDebug.Text);
            }
        }

        private void tbDebugClearClick(object sender, EventArgs e)
        {
            tbDebug.Text = "";
        }

        private void btnPIN_Click(object sender, EventArgs e)
        {
            commSend("i987654321");
        }

        private void btnErase_Click(object sender, EventArgs e)
        {
            commSend("e");
        }

        private void btnProg_Click(object sender, EventArgs e)
        {
            byte[] cnt = { 0 };
            bool success = true;
            try
            {
                doc.Dispose();
            }
            catch
            {
            }
            finally
            {
                try
                {
                    doc = new XmlTextReader(tbXmlFile.Text);
                    while (doc.Read())
                    {
                        if (doc.NodeType == XmlNodeType.Text)
                        {
                            commSend("p" + doc.Value);
                            cnt[0]++;

                            if (portNewLineEvent.WaitOne(10000) == true)
                            {
                                //signaled
                                if (portLine == "a.61")
                                    continue;
                                else if (portLine == "n.6E")
                                {
                                    success = false;
                                    break;
                                }
                            }
                            else
                            {
                                tbDebug.Text += "> Program Command Timeout\r\n";
                                tbDebug.Text += "> Terminating Programming Sequence...\r\n";
                                success = false;
                                break;
                            }
                        }
                    }
                }
                catch
                {
                    tbDebug.Text += "> Program Command Undefined Error\r\n";
                    tbDebug.Text += "> Terminating Programming Sequence...\r\n"; 
                    success = false;
                }

                if (success == true)
                    tbDebug.Text += "> Programming SUCCESS...\r\n";
                else
                    tbDebug.Text += "> Porgramming ERROR...\r\n";
            }
        }

        private void btnVerify_Click(object sender, EventArgs e)
        {
            tbDebug.Text += "Verify not supported\r\n";
        }

        private void btnBoot_Click(object sender, EventArgs e)
        {
            commSend("b");
        }
    }
}
