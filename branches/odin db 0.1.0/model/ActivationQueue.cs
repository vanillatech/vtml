using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class ActivationQueue : Queues
    {
        Brain brain;
        internal ActivationQueue(Brain mybrain)
        {
            this.brain = mybrain;
        }
        internal void nextStep()
        {
                   
            this.removeStep(0);
            this.decrementSteps();
            
        }

        

        internal void fireCurrentNeurons()
        {
            foreach (Neuron n in getStep(0).neuron) {
                if (n.crossesThreshold())
                {
                    n.fire();
                }

            }
        }

       
    }
}
