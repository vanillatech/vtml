using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()] 
    class Axon
    {
        Neuron fromNeuron;
        Brain brain;
        List<Synapse> synapses = new List<Synapse>();
        internal Axon(Neuron neuron,Brain mybrain)
        {
            fromNeuron = neuron;
            brain = mybrain;
        }

        internal void synapseOn(Dendrite tmpDendrite, bool inhibitory = false)
        {
            Synapse s = tmpDendrite.getSynapse(this);
            if (inhibitory) s.weight = -s.weight;
            if (!this.synapses.Contains(s))
            {
                this.synapses.Add(s);

            }
           
            
        }

        internal List<Neuron> getSuccessors()
        {
            List<Neuron> tmp = new List<Neuron>();
            foreach (Synapse s in this.synapses)
            {
                tmp.Add(s.getNeuron());
            }
            return tmp;
        }

        internal Neuron getParentNeuron()
        {
            return (this.fromNeuron);
        }

        internal void propagateActionPotential()
        {
            foreach (Synapse s in this.synapses)
            {
                if (brain.distributeActivationAmongSynapses) s.activate(this.synapses.Count);
                else s.activate();
            }
        }

        internal void removeSynapse(Synapse synapse)
        {
            this.synapses.Remove(synapse);
        }
    }
}
