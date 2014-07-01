using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class ActivationQueue : Queues
    {
        
        internal void nextStep()
        {
            this.decrementSteps();
           
            this.removeStep(0);
            
        }

        

        internal void fireCurrentNeurons()
        {
            foreach (Neuron n in getStep(0).neuron) {
                n.fire(); 

            }
        }
    }
}
