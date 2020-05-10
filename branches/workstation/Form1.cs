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
       // Dictionary<String,Brain> brains = new Dictionary<string,Brain>();

         
        Brain brain;
        odin.model.Monitor monitor;
        Stream braindump = null;
        

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
            var db = DictionaryHandler.Instance;
            Dictionary<String, Brain> brains = db.getBrainDictionary();
            brain = new Brain();
            brains.Add("ide", brain);
            Dictionary<String, SimpleLog> simpleLog = db.getLogs();
            simpleLog.Add("debug", new SimpleLog());
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
            this.menuItem1.Text = "&Open";
            this.menuItem1.Click += new System.EventHandler(this.notifyIcon1_MouseClick);

            this.contextMenu1.MenuItems.AddRange(
                        new System.Windows.Forms.MenuItem[] { this.menuItem1 });

            this.menuItem1 = new System.Windows.Forms.MenuItem();

            this.menuItem1.Index = 1;
            this.menuItem1.Text = "E&xit";
            this.menuItem1.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            
            this.contextMenu1.MenuItems.AddRange(
                        new System.Windows.Forms.MenuItem[] { this.menuItem1 });

            
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);

            notifyIcon1.Icon = new Icon("ML.ico");

            notifyIcon1.ContextMenu = this.contextMenu1;

            notifyIcon1.Text = "Vanillatech ML";
            notifyIcon1.Visible = true;

            //--show form: 
            notifyIcon1.DoubleClick += new System.EventHandler(this.notifyIcon1_MouseClick);


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
        public void onNewSimpleLogEntry(String logEntry)
        {
            try
            {
               
                BeginInvoke(new Action(() =>
                {
                    listBox2.Items.Insert(0, logEntry);
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
                            var db = DictionaryHandler.Instance;
                            Dictionary<String, Brain> brains = db.getBrainDictionary();
                            brains["ide"] = brain;
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
                            var db = DictionaryHandler.Instance;
                            Dictionary<String, Brain> brains = db.getBrainDictionary();
                            binaryFormatter.Serialize(braindump, brains["ide"]);
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
                
                saveDump();
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
        private void debugToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /* monitor = brain.addMonitor();

                monitor.attachLog(onNewLogEntry);
                monitor.attachOutput(onOutputChanged);*/
            var db = DictionaryHandler.Instance;
            Dictionary<String, SimpleLog> simpleLog = db.getLogs();
            if (!simpleLog["debug"].attached)
            {
               
                simpleLog["debug"].attachLog(onNewSimpleLogEntry);
            }
            else
            {
                simpleLog["debug"].detachLog();
            }
            
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            var db = DictionaryHandler.Instance;
            Dictionary<String, Brain> brains = db.getBrainDictionary();
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
            var db = DictionaryHandler.Instance;
            Dictionary<String, Brain> brains = db.getBrainDictionary();
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

        private void debugToolStripMenuItem_Click()
        {

        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            brain = new Brain();
            var db = DictionaryHandler.Instance;
            Dictionary<String, Brain> brains = db.getBrainDictionary();
            brains["ide"] = brain;
            braindump = null;
            listBox2.Items.Clear();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            listBox2.Items.Clear();
        } 
    }
}
