using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Deployment.Application;

namespace odin
{
    public partial class InfoWindow : Form
    {
        public InfoWindow()
        {
            InitializeComponent();
            if (ApplicationDeployment.IsNetworkDeployed)
            {
                versionlabel.Text = "Version " + ApplicationDeployment.CurrentDeployment.CurrentVersion.ToString(4);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
