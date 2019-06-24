using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()] 
    class Sense
    {
        Brain brain;
        bool noOutput;
        internal Sense(Brain mybrain,bool noOutput=false)
        {
            brain = mybrain;
            this.noOutput = noOutput;
        }
        [Serializable()] 
        internal struct Inputs
        {
            internal Dendrite dendrite;
            internal int value;
        }
        internal List<Inputs> cInputNeurons = new List<Inputs>();
        public int input(int byteRead,double activationVal = 1)
        {
            Dendrite inp = this.getInputDendrite(byteRead);

            if (inp == null)
                return (1);       
            
            inp.activate(activationVal);
            brain.blockOutputNeuronsForStep(brain.currentStep + 1);
            return (0);
        }

        private Dendrite getInputDendrite(int byteRead)
        {
            if (cInputNeurons.Exists(x => x.value == byteRead))
            {
                return cInputNeurons.Find(x => x.value == byteRead).dendrite;
            }
            else if (brain.isInLearnMode)
            {
                Inputs tmp = new Inputs();

                Neuron tmpNeuron = new Neuron(brain);
                tmpNeuron.tag = byteRead;
                if (this.noOutput) tmpNeuron.type = 3;
                else tmpNeuron.type = 1;
                
                tmp.dendrite = tmpNeuron.getNewDendrite(1);

                tmp.value = byteRead;
                cInputNeurons.Add(tmp);
                brain.log("New Inputneuron: " + brain.getIntAsFeature(tmpNeuron.tag) + "(" + tmpNeuron.id + ")", Brushes.Green);
                return tmp.dendrite;
            }
            else return null;
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
