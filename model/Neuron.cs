using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Neuron
    {
        Axon axon;
        Brain brain;
        internal double activation = 0;
        internal UInt64 lastFired = 0;
        internal UInt64 lastLeakEvent = 0;
        internal int layer;
        internal int tag;
        internal int type = 0; //1: input, 2: output

        List<Dendrite> dendrites = new List<Dendrite>();
        internal Neuron(Brain mybrain)
        {
            this.id = mybrain.getNextID();
            axon = new Axon(this,mybrain);
            brain = mybrain;
            this.layer = 1;
            this.tag = 0;

        }
        
        internal Dendrite getNewDendrite(int length) {
            Dendrite tmp = new Dendrite(this,brain,length);
            dendrites.Add(tmp);
            return tmp;
        }
        internal Dendrite getDendrite(int length)
        {
            if (this.dendrites.Exists(l => l.length == length))
            {
                return this.dendrites.Find(l => l.length == length);
            }
            else
            {
                return getNewDendrite(length);
            }
        }
        internal void fire()
        {
            if (this.type == 2)// && !brain.isInLearnMode)
            {
                brain.addToOutputStack(this.tag);
            }
            brain.log("Fired: " + this.id);
            brain.monitorOutput(this.tag);
            this.activation = 0;
            this.lastFired = brain.currentStep;
            if (this.type != 2)
            {
               brain.addToRecoveryQueue(this);
               brain.lateralInhibition(this.layer);
               this.axon.propagateActionPotential();
            }
            
        }


        internal void synapseOn(Dendrite tmpDendrite,bool inhibitory = false)
        {
            
            this.axon.synapseOn(tmpDendrite, inhibitory);
            
        }

        internal bool crossesThreshold()
        {
            if (this.activation > brain.activationThreshold) {
                return true;
            }
            return false;
        }

        public int id { get; set; }

        internal List<Neuron> getSuccessors()
        {
            return this.axon.getSuccessors();
        }

        internal void polarize(double p)
        {
            this.activation += p;
            brain.log("Polarized: " + this.id + " with Activation: " + p + " now having " + this.activation);
            
        }

        internal List<Neuron> getPredecessors()
        {
            List<Neuron> neurons = new List<Neuron>();
            foreach (Dendrite d in this.dendrites)
            {
                foreach (Neuron n in d.getPredecessors())
                {
                    neurons.Add(n);
                }
            }
            return neurons;
        }

        internal List<Dendrite> getDendrites()
        {
            return (this.dendrites);
        }

        internal void leakActivation()
        {
            this.activation *= Math.Pow(brain.leakageFactor, (brain.currentStep - this.lastLeakEvent));
            this.lastLeakEvent = brain.currentStep;
        }

        internal void inhibit()
        {
            this.activation = this.activation * brain.inhibitFactor;
        }

        internal bool isWithinRefractoryPeriod()
        {
            return (!(this.lastFired < brain.currentStep - (ulong)(brain.refractoryPeriod) || this.lastFired == 0 || this.type > 0)); 
        }
    }
}
