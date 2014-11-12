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
        private int value;

        List<Dendrite> dendrites = new List<Dendrite>();
        internal Neuron(Brain mybrain, int val = 0)
        {
            this.value = val;
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
            brain.addToOutputStack(this.value);
            brain.addToRecoveryQueue(this);
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
    }
}
