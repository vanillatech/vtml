using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace odin.model
{
    [Serializable()] 
    public class Monitor
    {
        private Brain brain;
        private Sense sense;
        private String lastLogEntry;
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
        
        internal void log(String s, Brush b)
        {
            this.lastLogEntry = s;
            NewLogEntry(s,b);
        }

        internal Neuron getInputNode(string inp)
        {
            int p = int.Parse(inp);
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
            this.outputString += p+", ";
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

        private Action<string,Brush> NewLogEntry;
        internal void attachLog(Action<string,Brush> onNewLogEntry)
        {
            NewLogEntry = onNewLogEntry;

        }

        internal void clearOutput()
        {
            this.outputString = "";
        }
    }
}
