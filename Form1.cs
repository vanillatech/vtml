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
            monitor = brain.addMonitor();
           
            monitor.attachLog(onNewLogEntry);
            monitor.attachOutput(onOutputChanged);
           


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
                        String datarec = brain.query(dataFromClient,true);
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
        private void button1_Click(object sender, EventArgs e)
        {
            //monitor.clearOutput();
            textBox4.Text = brain.query(textBox1.Text);
            textBox1.Text = "";
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

        private void button2_Click(object sender, EventArgs e)
        {
            textBox4.Text = brain.query(textBox1.Text, true);
        }

        
        

        
    }
}
