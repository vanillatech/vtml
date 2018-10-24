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

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            brain = new Brain();
            brains.Add("ide", brain);
            /* debug */
            
            /* --debug */


            this.listenThread = new Thread(new ThreadStart(ListenForClients));
            this.listenThread.Start();
            listenThread.IsBackground = true;
            



            
            
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
                                if (inp.desiredOutput != 0)
                                {
                                    brains[inp.token].desiredOutput = inp.desiredOutput;
                                }
                                else if (inp.input != null)
                                {
                                    brains[inp.token].desiredOutput = inp.input[0];
                                }
                                datarec = brains[inp.token].query(inp.input, inp.context, inp.learnmode);
                                brains[inp.token].think(inp.outputLMT);
                                datarec = brains[inp.token].getOutput();
                            }
                            else datarec = "token required.";
                        }
                        catch (Exception e) { datarec = "Input Format error."; }
                        if (datarec != null)
                        {
                            byte[] sendBytes = Encoding.ASCII.GetBytes(datarec + "\n");
                            clientStream.Write(sendBytes, 0, sendBytes.Length);
                        }


                        dataFromClient = "";
                    }
                }
                
            }

            tcpClient.Close();
        }
        private void onOutputChanged (String output) {
            try { 
            textBox3.Text = output;
            }
            catch (System.InvalidOperationException) { }
        }
        private void onNewLogEntry(String logEntry)
        {
            try
            {
                listBox1.Items.Insert(0, logEntry);
            }
            catch (System.InvalidOperationException) { }
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

        private void button4_Click(object sender, EventArgs e)
        {
            for (int n = 0; n < 30; n++)
            {
                int m = 0;
                Random rnd = new Random();
                
                for (; m < rnd.Next(12); m++)
                {
                    brain.query(rnd.Next(50),true); 

                }
                brain.query(1, true);
                brain.query(2, true);
                brain.query(3, true);
                brain.query(4, true);
                brain.query(5, true);
                brain.query(6, true);
                brain.query(7, true);
                brain.query(8, true);
                brain.query(9, true); 
                
                for (m++; m < 20; m++)
                {
                    brain.query(rnd.Next(50), true); 

                }
                
                //brain.think(20);
                //textBox4.Text = string.Join(",", nums);
            }
            for (int n = 0; n < 20; n++)
            {
                int m = 0;
                Random rnd = new Random();

                for (; m < rnd.Next(12); m++)
                {
                    brain.query(rnd.Next(50), true);

                }
                brain.query(1, true);
                brain.query(3, true);
                brain.query(5, true);
                brain.query(7, true);
                brain.query(9, true);
                brain.query(11, true);
                brain.query(13, true);

                for (m++; m < 20; m++)
                {
                    brain.query(rnd.Next(50), true);

                }

                //brain.think(20);
                //textBox4.Text = string.Join(",", nums);
            }
            for (int n = 0; n < 30; n++)
            {
                int m = 0;
                Random rnd = new Random();

                for (; m < rnd.Next(12); m++)
                {
                    brain.query(rnd.Next(50), true);

                }
                brain.query(1, true);
                brain.query(2, true);
                brain.query(3, true);
                
                
                brain.query(6, true);
                brain.query(9, true);
                brain.query(12, true);
                brain.query(15, true);

                for (m++; m < 20; m++)
                {
                    brain.query(rnd.Next(50), true);

                }

                brain.think(20);
                //textBox4.Text = string.Join(",", nums);
            }
        }

        private void debugToolStripMenuItem_Click(object sender, EventArgs e)
        {
            monitor = brain.addMonitor();

            monitor.attachLog(onNewLogEntry);
            monitor.attachOutput(onOutputChanged);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            for (int n = brains.Count() - 1; n >= 0; n--)
            {
                if (brains.ElementAt(n).Value.inactiveSince.AddMinutes(60) < DateTime.Now && brains.ElementAt(n).Key != "ide")
                {
                    if (!brains.ElementAt(n).Value.temporary)
                    {
                        this.saveBrain(brains.ElementAt(n).Key);
                    }
                    brains.Remove(brains.ElementAt(n).Key);
                }
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
