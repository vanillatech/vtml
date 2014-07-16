using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace odin.model
{
    class Monitor
    {
        private Brain brain;
        private Sense sense;
        private BindingList<String> logEntries;
        private String outputString;
        
        /*public String OutputString { get { return outputString; } set { outputString = value; } }
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, e);
            }
        }*/
        public Monitor(Brain mybrain, Sense senseToMonitor)
        {
            
            this.brain = mybrain;
            this.sense = senseToMonitor;
            this.outputString = "";
        }
        public void addLog(ref BindingList<String> debugLog)
        {
            this.logEntries = debugLog;
            
            
        }
        internal void log(String s)
        {
            if (this.logEntries != null)
            {
                logEntries.Insert(0,s);
            }
        }

        internal Neuron getInputNode(string inp)
        {
            int p = char.Parse(inp);
            Neuron n = sense.getInputNeuronIfExists(p);
            if (n != null)
            {
                return n;
            }
            else return null;

        }

        internal List<Neuron> getSubNodes(Neuron node)
        {
            if (node != null) return node.getSuccessors();
            return null;
        }

        internal void output(int p)
        {
            this.outputString += (Char)(p);
            OutputChanged(this.outputString);
            /*var handler = PropertyChanged;
            if (handler != null)
            { 
                handler(this, new PropertyChangedEventArgs(OutputString)); 
            }*/
        }

        internal void attachOutput(ref string p)
        {
            this.outputString = p;
            
        }


        private Action<string> OutputChanged;
        
        internal void attachOutput(Action<string> onOutputChanged)
        {
            OutputChanged = onOutputChanged;
            
        }

        internal void clearOutput()
        {
            this.outputString = "";
        }
    }
}
