using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()] 
    class Synapse
    {
        Axon fromAxon;
        Dendrite toDendrite;
        Brain brain;
        internal int countExcitatorySynapses;
        internal int countInhibitorySynapses = 0;
        internal double weight;
        internal ulong lastActivation = 0;

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
            if (brain.forgetAfterSteps > 0)
            //if (brain.forgetAfterSteps > 0)
            {
                this.forget();
            }
            if (brain.activateNeuronBasedOnInputSynapses)
            {
                toDendrite.activate(this.getStrength() / p / toDendrite.countSynapses());
            }
            else
            {
                toDendrite.activate(this.getStrength() / p);
            }
            if (brain.learnOnActivate) this.reinforce(true);
            this.lastActivation = brain.currentStep;
        }

        private int forget()
        {
            if (this.lastActivation + brain.forgetAfterSteps < brain.lastStepInLearnmode + brain.forgetAfterLearnMode)
            {
                if (this.countExcitatorySynapses <= brain.synapseDefaultCount && this.countInhibitorySynapses <= brain.synapseDefaultCount) this.weight *= (1 - brain.forgetRate);
                /*this.countExcitatorySynapses--;
                this.countInhibitorySynapses--;
                if (countExcitatorySynapses < 0) countExcitatorySynapses = 0;
                if (countInhibitorySynapses < 0) countInhibitorySynapses = 0;*/
                /*if (countInhibitorySynapses == 0 && countExcitatorySynapses == 0)
                {
                    toDendrite.removeSynapse(this);
                    fromAxon.removeSynapse(this);
                    return 1;
                }*/
                this.lastActivation = brain.currentStep;

            } 
            return 0;
        }
        internal void activate()
        {
            this.activate(1);
        }
    }
}
