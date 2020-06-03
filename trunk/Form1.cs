using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Web.Script.Serialization;
using System.IO;
using odin.model;

namespace odin
{

    public partial class Form1 : Form
    {
        Dictionary<String,Brain> brains = new Dictionary<string,Brain>();
        Brain brain;
        odin.model.Monitor monitor;
        Stream braindump = null;
        
        private Thread listenThread;

        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private System.Windows.Forms.ContextMenu contextMenu1;
        private System.Windows.Forms.MenuItem menuItem1;
        private System.ComponentModel.IContainer comp;

        private bool allowshowdisplay = false;
        protected override void SetVisibleCore(bool value)
        {
            base.SetVisibleCore(allowshowdisplay ? value : allowshowdisplay);
        }

        public Form1()
        {
            
            InitializeComponent();
            listBox1.DrawMode = DrawMode.OwnerDrawFixed;
            createIcon();
            brain = new Brain();
            brains.Add("ide", brain);

            this.listenThread = new Thread(new ThreadStart(ListenForClients));
            this.listenThread.Start();
            listenThread.IsBackground = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
  
        }
        private void notifyIcon1_MouseClick(object sender, EventArgs e )
        {
            
            this.allowshowdisplay = true;
            this.Visible = !this.Visible;

        }
        private void createIcon()
        {
            this.comp = new System.ComponentModel.Container();
            this.contextMenu1 = new System.Windows.Forms.ContextMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();

            this.menuItem1.Index = 0;
            this.menuItem1.Text = "E&xit";
            this.menuItem1.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            
            this.contextMenu1.MenuItems.AddRange(
                        new System.Windows.Forms.MenuItem[] { this.menuItem1 });

            
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);

            notifyIcon1.Icon = new Icon("ML.ico");

            notifyIcon1.ContextMenu = this.contextMenu1;

            notifyIcon1.Text = "Vanillatech ML";
            notifyIcon1.Visible = true;

            //--show form: only for development/debugging
            //notifyIcon1.Click += new System.EventHandler(this.notifyIcon1_MouseClick);


        }
        private void ListenForClients()
        {
            IPAddress hostIP = Dns.GetHostEntry(Dns.GetHostName()).AddressList[1];
            TcpListener serverSocket = new TcpListener(hostIP, 11202);
            serverSocket.Start();
            while (true)
            {
                //blocks until a client has connected to the server
                TcpClient client = serverSocket.AcceptTcpClient();

                //create a thread to handle communication 
                //with connected client
                Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
                clientThread.Start(client);
            }
            
        }
        private void HandleClientComm(object client)
        {
            TcpClient tcpClient = (TcpClient)client;
            NetworkStream clientStream = tcpClient.GetStream();

            byte[] message = new byte[4096];
            int bytesRead;
            string dataFromClient = "";

            while (true)
            {
                bytesRead = 0;

                try
                {
                    //blocks until a client sends a message
                    bytesRead = clientStream.Read(message, 0, 4096);
                }
                catch
                {
                    //a socket error has occured
                    break;
                }

                if (bytesRead == 0)
                {
                    //the client has disconnected from the server
                    break;
                }

                //message has successfully been received
                ASCIIEncoding encoder = new ASCIIEncoding();
                dataFromClient += encoder.GetString(message, 0, bytesRead);
                if (dataFromClient.IndexOf("\n") > -1)
                {
                    dataFromClient = dataFromClient.Substring(0, dataFromClient.IndexOf("\n")-1);
                    if (dataFromClient != null)
                    {

                        string datarec = "";
                        try
                        {
                            templateQueryData inp = new JavaScriptSerializer().Deserialize<templateQueryData>(dataFromClient);


                            if (inp.token != null)
                            {
                                if (!brains.ContainsKey(inp.token))
                                {
                                    if (File.Exists(inp.token + ".dump"))
                                    {
                                        try
                                        {
                                            FileStream bd = File.OpenRead(inp.token + ".dump");
                                            var binaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                                            brains.Add(inp.token, (Brain)binaryFormatter.Deserialize(bd));
                                            bd.Close();

                                        }
                                        catch (Exception ex)
                                        {
                                            datarec = "Error: Could not read file from disk. Original error: " + ex.Message;
                                        }
                                        brains[inp.token].init();
                                    }
                                    else
                                    {
                                        brains.Add(inp.token, new Brain());
                                    }
                                }
                                if (inp.activationThreshold > 0)
                                {
                                    brains[inp.token].activationThreshold = inp.activationThreshold;
                                }
                                if (inp.activateNewNeurons  == true)
                                {
                                    brains[inp.token].activateNewNeurons = inp.activateNewNeurons;
                                }
                                if (inp.noSeparateFeatures)
                                {
                                    brains[inp.token].separateFeatures = false;
                                }
                                if (inp.maxlayer > 0)
                                {
                                    brains[inp.token].maxLayer = inp.maxlayer;
                                }
                                if (inp.temporalPatternLength > 0)
                                {
                                    brains[inp.token].temporalPatternLength = inp.temporalPatternLength;
                                }
                                if (inp.leakageFactor > 0)
                                {
                                    brains[inp.token].leakageFactor = inp.leakageFactor;
                                }
                                if (inp.token.Contains("READONLY"))
                                {
                                    inp.learnmode = false;
                                }
                                if (inp.temporary == true)
                                {
                                    brains[inp.token].temporary = inp.temporary;
                                }
                                //* translate features to int representation
                                if (inp.featureMatrix != null)
                                {
                                    inp.context = new int[inp.featureMatrix.Count()];
                                    for(int n=0;n<inp.featureMatrix.Count();n++)
                                    {
                                        inp.context[n] = brains[inp.token].getFeatureAsInt(inp.featureMatrix[n]);
                                    }
                                }
                                if (inp.inputFeature != null)
                                {
                                    inp.input = new int[inp.inputFeature.Count()];
                                    for (int n = 0; n < inp.inputFeature.Count(); n++)
                                    {
                                        inp.input[n] = brains[inp.token].getFeatureAsInt(inp.inputFeature[n]);
                                    }
                                }
                                if (inp.outputFeature != null)
                                {
                                    inp.desiredOutput = brains[inp.token].getFeatureAsInt(inp.outputFeature);
                                    
                                }
                                //*
                                if (inp.desiredOutput != 0)
                                {
                                    brains[inp.token].desiredOutput = inp.desiredOutput;
                                }
                                else if (inp.input != null)
                                {
                                    brains[inp.token].desiredOutput = inp.input[0];
                                }
                                else
                                {
                                    brains[inp.token].desiredOutput = 0;
                                }
                                datarec = brains[inp.token].query(inp.input, inp.context, inp.learnmode);
                                brains[inp.token].think(inp.outputLMT);
                                datarec = brains[inp.token].getOutput();
                            }
                            else datarec = "token required.";
                        }
                        catch (Exception e) {
                            datarec = e.Message; //  "Input Format error."; 
                        }
                        if (datarec != null)
                        {
                            try
                            {
                                byte[] sendBytes = Encoding.ASCII.GetBytes(datarec + "\n");
                                clientStream.Write(sendBytes, 0, sendBytes.Length);
                            }
                            catch (Exception e) {  }

                        }


                        dataFromClient = "";
                    }
                }
                
            }

            tcpClient.Close();
        }
        private void onOutputChanged (String output) {
            try
            {
                textBox3.Text = output;
            }
            catch (System.InvalidOperationException) { }
        }
        private List<Brush> logColors = new List<Brush>();
        public void onNewLogEntry(String logEntry, Brush color)
        {
            try
            {
                logColors.Insert(0, color);
                BeginInvoke(new Action(() =>
   {
       listBox1.Items.Insert(0, logEntry);
   }));
                
            }
            catch (System.InvalidOperationException) { }
        }
        private void listBox1_DrawItem(object sender, DrawItemEventArgs e)
        {
            if (e.Index != -1)
            {
                e.DrawBackground();
                Brush brush = logColors[e.Index];
                e.Graphics.DrawString(listBox1.Items[e.Index].ToString(), new Font("Arial", 10), brush, e.Bounds);
                e.DrawFocusRectangle();
            }
        }
        

        
        private void Query_Click(object sender, EventArgs e)
        {
            
            Neuron neuron = monitor.getInputNode(textBox2.Text);
            if (neuron != null)
            {
                TreeNode n = new TreeNode();
                n.Text = textBox2.Text + "(" + neuron.id + ")";
                n.Name = neuron.id.ToString();
                n.Tag = neuron;
                treeView1.Nodes.Add(n);

            }
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private void treeView1_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            TreeNode n = e.Node;
            n.Nodes.Clear();
            List<Neuron> subnodes = monitor.getSubNodes((Neuron)n.Tag);
            foreach (Neuron subnode in subnodes)
            {
                TreeNode m = new TreeNode();
                m.Text = subnode.id.ToString();
                m.Name = subnode.id.ToString();
                m.Tag = subnode;
                n.Nodes.Add(m);
                

            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)(13)) {
            button1_Click(this,new EventArgs());
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //monitor.clearOutput();
            String[] splitted = textBox1.Text.Split(',');

            int[] nums = new int[splitted.Length];

            for (int i = 0; i < splitted.Length; i++)
            {
                nums[i] = int.Parse(splitted[i]);
            }
            String[] splitted2 = textBox5.Text.Split(',');

            int[] nums2 = new int[splitted2.Length];

            for (int i = 0; i < splitted2.Length; i++)
            {
                if (splitted2[i] != "")
                {
                    nums2[i] = int.Parse(splitted2[i]);
                }
            }
            textBox4.Text = brain.query(nums,nums2);
            //textBox1.Text = "";
        }
        private void button2_Click(object sender, EventArgs e)
        {
            String[] splitted = textBox1.Text.Split(',');
            
            int[] nums = new int[splitted.Length];

            for(int i = 0 ; i < splitted.Length ; i++)
            {
                nums[i] = int.Parse(splitted[i]);
            }
            int[] nums2 = null;
            if (textBox5.Text != "")
            {
                String[] splitted2 = textBox5.Text.Split(',');

                nums2 = new int[splitted2.Length];

                for (int i = 0; i < splitted2.Length; i++)
                {
                    if (splitted2[i] != "")
                    {
                        nums2[i] = int.Parse(splitted2[i]);
                    }
                }
            }
            textBox4.Text = brain.query(nums, nums2, true);
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
            brain.think (20);
            textBox4.Text = brain.getOutput();

            /*using (Stream stream = File.Open("brain.dump", true ? FileMode.Append : FileMode.Create))
            {
                var binaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                binaryFormatter.Serialize(stream, brain);
            }*/
            


        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            //openFileDialog1.InitialDirectory = "c:\\";
            openFileDialog1.Filter = "brain dump (*.dump)|*.dump|All files (*.*)|*.*";
            //openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    if ((braindump = openFileDialog1.OpenFile()) != null)
                    {
                        using (braindump)
                        {
                            var binaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                            brain = (Brain)binaryFormatter.Deserialize(braindump);
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
                }
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveAsDump();
        }

        private void saveAsDump()
        {
            SaveFileDialog openFileDialog1 = new SaveFileDialog ();

            //openFileDialog1.InitialDirectory = "c:\\";
            openFileDialog1.Filter = "brain dump (*.dump)|*.dump|All files (*.*)|*.*";
            //openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                if ((braindump = openFileDialog1.OpenFile()) != null)
                {
                    saveDump();
                }
            }
        }

        private void saveDump()
        {
 	        try
                {
                    if ((braindump) != null)
                    {
                        using (braindump)
                        {
                            var binaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                            binaryFormatter.Serialize(braindump, brain);
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not write file to disk. Original error: " + ex.Message);
                }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {

            if (braindump == null) saveAsDump();
            else
            {
                
                saveAsDump();
            }

        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            Application.Exit();
        }

        
        private void debug_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)(13))
            {
                debugToolStripMenuItem_Click();
            }
        }
        private void debugToolStripMenuItem_Click()
        {
            String bid = brainidToolStripMenuItem.Text;
            if (!brains.ContainsKey(bid))
            {
                if (File.Exists(bid + ".dump"))
                {
                    try
                    {
                        FileStream bd = File.OpenRead(bid + ".dump");
                        var binaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                        brains.Add(bid, (Brain)binaryFormatter.Deserialize(bd));
                        bd.Close();

                    }
                    catch (Exception ex)
                    {
                        
                    }
                }
                else
                {
                    brains.Add(bid, new Brain());
                }
            }
                monitor = brains[bid].addMonitor();

                monitor.attachLog(onNewLogEntry);
                monitor.attachOutput(onOutputChanged);
            
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            String removenext = "";
            DateTime longestinactive = DateTime.Now;
            Boolean unloadBrain = false;
            for (int n = brains.Count() - 1; n >= 0; n--)
            {
                if (brains.ElementAt(n).Value.inactiveSince.AddMinutes(60) < DateTime.Now && brains.ElementAt(n).Key != "ide")
                {
                    if (!brains.ElementAt(n).Value.temporary)
                    {
                        if (brains.ElementAt(n).Value.inactiveSince > brains.ElementAt(n).Value.lastSaved)
                        {
                            brains.ElementAt(n).Value.lastSaved = DateTime.Now;
                            this.saveBrain(brains.ElementAt(n).Key);
                        }
                    }
                    if (brains.ElementAt(n).Value.temporary)
                    {
                        brains.Remove(brains.ElementAt(n).Key);
                    } else if (brains.ElementAt(n).Value.inactiveSince < longestinactive)
                    {
                        removenext = brains.ElementAt(n).Key;
                        longestinactive = brains.ElementAt(n).Value.inactiveSince;
                        unloadBrain = true;
                    }
                }
            }
            if (brains.Count() > 5 && unloadBrain)
            {
                brains.Remove(removenext);
            }
        }
        private void saveBrain(String b)
        {
            try
            {
                if (File.Exists(b + ".dump"))
                {
                    File.Delete(b + ".dump");
                }
                FileStream bd = File.OpenWrite(b + ".dump");
                if (bd != null)
                {
                    using (bd)
                    {


                        var binaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                        binaryFormatter.Serialize(bd, brains[b]);
                        bd.Close();
                    }
                }
            }
            catch (Exception ex)
            {
                
                listBox1.Items.Insert(0,"Error: Could not write file to disk. Original error: " + ex.Message);
            }
        }

        

        
        

        
    }
}
