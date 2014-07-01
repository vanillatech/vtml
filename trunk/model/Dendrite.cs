using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Dendrite
    {
        List<Synapse> synapses;
        int length;
        Neuron toNeuron;
        Brain brain;
        
        internal Dendrite(Neuron neuron, Brain mybrain, int len)
        {
            toNeuron = neuron;
            brain = mybrain;
            this.length = len;
        }

        internal void activate()
        {
            brain.activationQueue.addToStep(toNeuron,length);
        }
    }
}
