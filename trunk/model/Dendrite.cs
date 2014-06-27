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
        Neuron toNeuron;
        Brain brain;
        
        internal Dendrite(Neuron neuron, Brain mybrain)
        {
            toNeuron = neuron;
            brain = mybrain;
        }

        internal void activate()
        {
            brain.activationQueue.addToStep(toNeuron,1);
        }
    }
}
