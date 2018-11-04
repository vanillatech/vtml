﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()] 
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
            brain.log("Schedule Activation: " + toNeuron.id + " (Delay: " + length + "; ActVal: " + activationVal + ")",Brushes.Brown);
        }


        internal Synapse addSynapse(Axon axon)
        {
            Synapse s = new Synapse(axon,this,brain);
          
            this.synapses.Add(s);
            return s;
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

        internal Synapse getSynapse(Axon axon)
        {
            foreach (Synapse s in this.synapses)
            {
                if (s.getFromAxon() == axon) return s;

            }
            return this.addSynapse(axon);
        }

        internal double countSynapses()
        {
            return (this.synapses.Count);
        }

        internal void removeSynapse(Synapse synapse)
        {
            this.synapses.Remove(synapse);
        }
    }
}