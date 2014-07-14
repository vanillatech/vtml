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
            this.decrementSteps();
           
            this.removeStep(-1);
            
        }

        

        internal void fireCurrentNeurons()
        {
            
            foreach (QueueElement a in this.getElementsInStep(0)) {
                if (a.neuron.crossesThreshold())
                {
                    a.neuron.fire();
                }

            }
        }

        internal void processActivation()
        {
            foreach (QueueElement a in this.getElementsInStep(0))
            {
                a.neuron.polarize(a.val);

            }
        }

        internal void leakActivation()
        {
            foreach (QueueElement a in this.getElementsInStep(0))
            {
                a.neuron.leakActivation();

            }
        }
    }
}
