using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Synapse
    {
        Axon fromAxon;
        Dendrite toDendrite;
        Brain brain;
        internal int countExcitatorySynapses;
        internal int countInhibitorySynapses = 0;

        internal Synapse(Axon axon, Dendrite dendrite, Brain mybrain) {
            this.brain = mybrain;
            this.fromAxon = axon;
            this.toDendrite = dendrite;
            this.countExcitatorySynapses = brain.synapseDefaultCount;
            //toDendrite.addSynapse(this);
        }
        internal Axon getFromAxon()
        {
            return this.fromAxon;
        }
        internal void activate()
        {
            toDendrite.activate(this.getStrength());
        }

        internal Neuron getPredecessor()
        {
            Axon a = this.getFromAxon();
            return (a.getParentNeuron());
        }

        internal void reinforce(bool excitatory)
        {
            if (excitatory) this.countExcitatorySynapses++;
            else this.countInhibitorySynapses++;
        }
        internal double getStrength() {
            return brain.synapseDefaultStrength * (this.countExcitatorySynapses - this.countInhibitorySynapses);
        }

        internal Neuron getNeuron()
        {
            return (this.toDendrite.getNeuron());
        }
    }
}
