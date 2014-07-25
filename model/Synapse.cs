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
        internal double weight;

        internal Synapse(Axon axon, Dendrite dendrite, Brain mybrain) {
            this.brain = mybrain;
            this.fromAxon = axon;
            this.toDendrite = dendrite;
            this.countExcitatorySynapses = brain.synapseDefaultCount;
            this.weight = brain.synapseDefaultStrength;
            //toDendrite.addSynapse(this);
        }
        internal Axon getFromAxon()
        {
            return this.fromAxon;
        }
        

        internal Neuron getPredecessor()
        {
            Axon a = this.getFromAxon();
            return (a.getParentNeuron());
        }

        internal void reinforce(bool excitatory)
        {
            if (excitatory && (this.countExcitatorySynapses-this.countInhibitorySynapses) <= brain.synapseMaxCount)
            {
                this.countExcitatorySynapses++;
                //this algorithm is taken from James A. Anderson, 'Introduction to Neural Networks' (MIT Press)
                //related to Kohonen's SOFM.
                this.weight = this.weight - (this.weight - 1) *  brain.adaptionRate / (this.countExcitatorySynapses+this.countInhibitorySynapses);
            }
            else if (!excitatory && (this.countInhibitorySynapses - this.countExcitatorySynapses) <= brain.synapseMaxCount)
            {
                this.countInhibitorySynapses++;
                this.weight = this.weight * (1 - brain.adaptionRate / (this.countExcitatorySynapses + this.countInhibitorySynapses));
            }
        }
        internal double getStrength() {
            return this.weight;
        }

        internal Neuron getNeuron()
        {
            return (this.toDendrite.getNeuron());
        }

        internal void activate(int p)
        {
            if (brain.activateNeuronBasedOnInputSynapses)
            {
                toDendrite.activate(this.getStrength() / p / toDendrite.countSynapses());
            }
            else
            {
                toDendrite.activate(this.getStrength() / p);
            }
            if (brain.learnOnActivate) this.reinforce(true);
        }
        internal void activate()
        {
            this.activate(1);
        }
    }
}
