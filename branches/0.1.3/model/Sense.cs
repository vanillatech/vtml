﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Sense
    {
        Brain brain;
        internal Sense(Brain mybrain)
        {
            brain = mybrain;
        }
        internal struct Inputs
        {
            internal Dendrite dendrite;
            internal int value;
        }
        internal List<Inputs> cInputNeurons = new List<Inputs>();
        public void input(int byteRead)
        {
            Dendrite inp = this.getInputDendrite(byteRead);
            inp.activate(brain.synapseMaxCount*1);
        }

        private Dendrite getInputDendrite(int byteRead)
        {
            if (cInputNeurons.Exists(x => x.value == byteRead))
            {
                return cInputNeurons.Find(x => x.value == byteRead).dendrite;
            }
            else
            {
                Inputs tmp = new Inputs();
                Neuron tmpNeuron = new Neuron(brain);
                tmpNeuron.tag = byteRead;
                tmp.dendrite = tmpNeuron.getNewDendrite(1);
                tmp.value = byteRead;
                cInputNeurons.Add(tmp);
                return tmp.dendrite;
            }
        }
        internal Neuron getInputNeuronIfExists(int byteRead)
        {
            if (cInputNeurons.Exists(x => x.value == byteRead))
            {
                return cInputNeurons.Find(x => x.value == byteRead).dendrite.getNeuron();
            }
            else
            {
                return null;
            }
        }
        
        public void generateAlphaNumericalInputs()
        {
            for (int n = 0; n < 256; n++)
            {
                Inputs tmp = new Inputs();
                tmp.dendrite = new Neuron(brain).getNewDendrite(1);
                tmp.value = n;
                cInputNeurons.Add(tmp);
                
            }
        }
    }
}