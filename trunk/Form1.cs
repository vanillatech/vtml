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
using odin.model;

namespace odin
{

    public partial class Form1 : Form
    {
        Brain brain;
        odin.model.Monitor monitor;
        
        
        private Thread listenThread;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            brain = new Brain();
            /* debug */
            //monitor = brain.addMonitor();
           
            //monitor.attachLog(onNewLogEntry);
            //monitor.attachOutput(onOutputChanged);
            /* --debug */


            this.listenThread = new Thread(new ThreadStart(ListenForClients));
            this.listenThread.Start();

            



            
            
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
                        
                        templateQueryData inp = new JavaScriptSerializer().Deserialize<templateQueryData>(dataFromClient);

                        
                        string datarec = brain.query(inp.input,inp.context,inp.learnmode);
                        brain.think(inp.outputLMT );
                        datarec = brain.getOutput();
                        if (datarec != null)
                        {
                            byte[] sendBytes = Encoding.ASCII.GetBytes(datarec + "\n");
                            clientStream.Write(sendBytes, 0, sendBytes.Length);
                        }

                    }
                    dataFromClient = "";
                }
                
            }

            tcpClient.Close();
        }
        private void onOutputChanged (String output) {
            textBox3.Text = output;
        }
        private void onNewLogEntry(String logEntry)
        {
            listBox1.Items.Insert(0, logEntry);
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
                nums2[i] = int.Parse(splitted2[i]);
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
            String[] splitted2 = textBox5.Text.Split(',');

            int[] nums2 = new int[splitted2.Length];

            for (int i = 0; i < splitted2.Length; i++)
            {
                nums2[i] = int.Parse(splitted2[i]);
            }
            textBox4.Text = brain.query(nums,nums2,true);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            brain.thinkToEnd();
            textBox4.Text = brain.getOutput();
        }

        
        

        
    }
}
