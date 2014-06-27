using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Axon
    {
        Neuron fromNeuron;
        Brain brain;
        List<Synapse> synapses;
        internal Axon(Neuron neuron,Brain mybrain)
        {
            fromNeuron = neuron;
            brain = mybrain;
        }
    }
}
