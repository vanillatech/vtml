using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Monitor
    {
        private Brain brain;
        private Sense sense;

        public Monitor(Brain mybrain, Sense senseToMonitor)
        {
            
            this.brain = mybrain;
            this.sense = senseToMonitor;
        }


        internal int getInputNode(string inp)
        {
            int p = char.Parse(inp);
            Neuron n = sense.getInputNeuronIfExists(p);
            if (n != null)
            {
                return n.id;
            }
            else return 0;

        }
    }
}
