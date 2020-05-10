using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Web.Http;
using System.Web.Http.SelfHost;
using odin.model;

namespace odin
{
    static class Program
    {
        /// <summary>
        /// Der Haupteinstiegspunkt für die Anwendung.
        /// </summary>
        [STAThread]
        static void Main()
        {
             
           var db = DictionaryHandler.Instance;
             
            var config = new HttpSelfHostConfiguration("http://localhost:81");

            config.Routes.MapHttpRoute(
                "API Default", "api/{controller}/{id}",
                new { id = RouteParameter.Optional });

            using (HttpSelfHostServer server = new HttpSelfHostServer(config))
            {
                server.OpenAsync().Wait();
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Form1());
            }
        }
    }
}
