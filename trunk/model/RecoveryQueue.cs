using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class RecoveryQueue : Queues
    {
        Brain brain;
        internal RecoveryQueue(Brain mybrain) {
            this.brain = mybrain;
        }
        private int maxSteps = 2;
        internal void add(Neuron neuron)
        {
            this.addToStep(neuron,0);
        }


        internal void nextStep()
        {
            this.incrementSteps();
            this.removeStep(maxSteps);
        }

        internal void setMaxSteps(int p)
        {
            this.maxSteps = p;
        }

        internal void learnNewPatterns()
        {
            Neuron tmpNeuron = new Neuron(brain);
            foreach (aQueue s in steps)
            {
                
                Dendrite tmpDendrite = tmpNeuron.getNewDendrite(s.step);
                foreach (Neuron n in s.neuron)
                {
                    n.synapseOn(tmpDendrite);

                }
                    
            }
        }
    }
}
