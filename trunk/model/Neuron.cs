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
        double activation = 0;
        internal UInt64 lastFired = 0;


        List<Dendrite> dendrites = new List<Dendrite>();
        internal Neuron(Brain mybrain)
        {
            this.id = mybrain.getNextID();
            axon = new Axon(this,mybrain);
            brain = mybrain;

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
            brain.log("Fired: " + this.id);
            this.activation = 0;
            this.lastFired = brain.currentStep;
            brain.addToRecoveryQueue(this);
            this.axon.propagateActionPotential();
        }


        internal void synapseOn(Dendrite tmpDendrite)
        {
            
            this.axon.synapseOn(tmpDendrite);
            
        }

        internal bool crossesThreshold()
        {
            if (this.activation >= brain.activationThreshold) {
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
            brain.log("Polarized: " + this.id + " with Activation: " + p);
            this.activation += p;
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
            this.activation *= brain.leakageFactor;
        }
    }
}
