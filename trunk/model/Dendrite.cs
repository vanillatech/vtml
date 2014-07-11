using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Dendrite
    {
        
        List<Synapse> synapses = new List<Synapse>();
        internal int length;
        
        Neuron toNeuron;
        Brain brain;
        
        internal Dendrite(Neuron neuron, Brain mybrain, int len)
        {
            toNeuron = neuron;
            brain = mybrain;
            this.length = len;
            
        }

        internal void activate(double activationVal)
        {
            brain.activationQueue.addToStep(toNeuron,length,activationVal);
        }


        internal void addSynapse(Axon axon)
        {
            Synapse s = new Synapse(axon,this,brain);
          
            this.synapses.Add(s);
        }

        internal Neuron getNeuron()
        {
            return this.toNeuron;
        }

        internal List<Neuron> getPredecessors()
        {
            List<Neuron> neurons = new List<Neuron>();
            foreach (Synapse s in this.synapses)
            {
                neurons.Add(s.getPredecessor());
            }
            return neurons;
        }

        internal IEnumerable<Synapse> getSynapses()
        {
            return this.synapses;
        }
    }
}
