﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Monitor
    {
        private Brain brain;
        private Sense sense;

        public Monitor(Brain mybrain, Sense senseToMonitor)
        {
            
            this.brain = mybrain;
            this.sense = senseToMonitor;
        }


        internal Neuron getInputNode(string inp)
        {
            int p = char.Parse(inp);
            Neuron n = sense.getInputNeuronIfExists(p);
            if (n != null)
            {
                return n;
            }
            else return null;

        }

        internal List<Neuron> getSubNodes(Neuron node)
        {
            if (node != null) return node.getSuccessors();
            return null;
        }
    }
}