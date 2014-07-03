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
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            brain = new Brain();

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
            Monitor monitor = brain.addMonitor();
            int val = monitor.getInputNode(textBox2.Text);
            if (val != 0)
            {
                TreeNode n = new TreeNode();
                n.Text = textBox2.Text + "(" + val + ")";
                treeView1.Nodes.Add(n);

            }
        }
    }
}
