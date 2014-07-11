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
        List<Dendrite> synapses = new List<Dendrite>();
        internal Axon(Neuron neuron,Brain mybrain)
        {
            fromNeuron = neuron;
            brain = mybrain;
        }

        internal void synapseOn(Dendrite tmpDendrite)
        {
            if (!this.synapses.Contains(tmpDendrite))
            {
                this.synapses.Add(tmpDendrite);
                tmpDendrite.addSynapse(this);
            }
            
        }

        internal List<Neuron> getSuccessors()
        {
            List<Neuron> tmp = new List<Neuron>();
            foreach (Dendrite d in this.synapses)
            {
                tmp.Add(d.getNeuron());
            }
            return tmp;
        }

        internal Neuron getParentNeuron()
        {
            return (this.fromNeuron);
        }
    }
}
