using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace VTMLInstaller.cs
{
    [RunInstaller(true)]
    public partial class VTMLInstallerClass : System.Configuration.Install.Installer
    {
        public VTMLInstallerClass()
        {
            InitializeComponent();
        }
        protected override void OnAfterInstall(IDictionary savedState)
        {
            base.OnAfterInstall(savedState);

            var proc = Process.Start(
                new ProcessStartInfo
                {
                    FileName = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.System),"netsh.exe"),
                    Arguments = "http add urlacl url=http://+:81/ sddl=D:(A;;GX;;;S-1-1-0) listen=yes",
                    WindowStyle = ProcessWindowStyle.Hidden
                });
            proc.WaitForExit();
        }
        protected override void OnAfterUninstall(IDictionary savedState)
        {
            base.OnAfterUninstall(savedState);

            var proc = Process.Start(
                new ProcessStartInfo
                {
                    FileName = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.System), "netsh.exe"),
                    Arguments = "http delete urlacl url=http://+:81/",
                    WindowStyle = ProcessWindowStyle.Hidden
                });
            proc.WaitForExit();
        }
       
    }
}
