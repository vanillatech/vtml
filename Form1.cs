using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using odin.model;

namespace odin
{
    public partial class Form1 : Form
    {
        Brain brain;
        Monitor monitor;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            brain = new Brain();
            monitor = brain.addMonitor();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            brain.input(textBox1.Text);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            brain.think();
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
    }
}
