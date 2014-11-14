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
        private int value;

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

            brain.addToOutputStack(this.tag);

            brain.log("Fired: " + this.id);
            brain.monitorOutput(this.tag);
            this.activation = 0;
            this.lastFired = brain.currentStep;

            brain.addToRecoveryQueue(this);
            brain.lateralInhibition(this.layer);
            this.axon.propagateActionPotential();
        }


        internal void synapseOn(Dendrite tmpDendrite)
        {
            
            this.axon.synapseOn(tmpDendrite);
            
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
            this.activation = brain.inhibitFactor;
        }
    }
}
