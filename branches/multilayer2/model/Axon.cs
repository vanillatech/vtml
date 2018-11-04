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

        internal bool synapseOn(Dendrite tmpDendrite, bool inhibitory = false)
        {
            Synapse s = tmpDendrite.getSynapse(this);
            
            if (!this.synapses.Contains(s))
            {
                if (inhibitory) s.weight = -s.weight;
                this.synapses.Add(s);
                return true;
            }
            return false;
            
        }
        internal bool synapseOn(Dendrite tmpDendrite, double weight)
        {
            Synapse s = tmpDendrite.getSynapse(this);

            if (!this.synapses.Contains(s))
            {
                s.weight = weight;
                this.synapses.Add(s);
                return true;
            }
            return false;

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
