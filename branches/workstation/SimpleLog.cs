using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin
{
    public class SimpleLog
    {
        
        public SimpleLog() { }
        public bool attached = false;
        internal void log(String s)
        {
            if (attached) NewLogEntry(s);
        }
        private Action<string> NewLogEntry;
        internal void attachLog(Action<string> onNewLogEntry)
        {
            NewLogEntry = onNewLogEntry;
            attached = true;

        }
        internal void detachLog()
        {
            NewLogEntry = null;
            attached = false;
        }
    }
}
