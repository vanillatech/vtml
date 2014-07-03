using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Dendrite
    {
        List<Axon> synapses = new List<Axon> ();
        int length;
        double strength;
        Neuron toNeuron;
        Brain brain;
        
        internal Dendrite(Neuron neuron, Brain mybrain, int len)
        {
            toNeuron = neuron;
            brain = mybrain;
            this.length = len;
            this.strength = brain.synapseDefaultStrength;
        }

        internal void activate()
        {
            brain.activationQueue.addToStep(toNeuron,length,strength);
        }


        internal void addSynapse(Axon axon)
        {
            this.synapses.Add(axon);
        }

        internal Neuron getNeuron()
        {
            return this.toNeuron;
        }
    }
}
