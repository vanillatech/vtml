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
        internal Dendrite(Neuron neuron)
        {
            toNeuron = neuron;
        }

        internal void activate()
        {
            throw new NotImplementedException();
        }
    }
}
