using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()] 
    class RecoveryQueue : Queues
    {
        Brain brain;
        private int getNextCurrentStepPointer = 0;
        private int getNextCurrentPos = 0;
        internal RecoveryQueue(Brain mybrain) {
            this.brain = mybrain;
        }

        internal void add(Neuron neuron)
        {
            this.addToStep(neuron,0);
        }


        internal void nextStep()
        {
            this.incrementSteps();
            this.removeStep(brain.temporalPatternLength+1);
            //brain.log("next Step in RecoveryQueue");
        }

        internal Neuron getFirst(int step)
        {
            this.getNextCurrentPos = 0;
            return getNext(step);
        }
        internal Neuron getFirst()
        {
            this.getNextCurrentPos = 0;
            return getNext();
        }
        internal Neuron getNext(int step)
        {

            if (steps.Exists(x => x.step == step))
            {
                List<QueueElement> n = steps.Find(x => x.step == step).elements;
                if (n.Count > getNextCurrentPos)
                {
                    return n[getNextCurrentPos++].neuron;
                }
                else
                {
                    getNextCurrentPos = 0;
                    
                    return null;
                }
            }
            else
            {
                return null;
            }
        }
       

        internal Neuron getNext()
        {
            if (steps.Count > getNextCurrentStepPointer) {
                List<QueueElement> n = steps[getNextCurrentStepPointer].elements;
                if (n.Count > getNextCurrentPos)
                {
                    return n[getNextCurrentPos++].neuron;
                }
                else
                {
                    getNextCurrentPos = 0;
                    getNextCurrentStepPointer++;
                    return getNext();
                }
            }
            else
            {
                getNextCurrentStepPointer = 0;
                return null;
            }
        }

        internal int getCurrentStep()
        {
            return steps[getNextCurrentStepPointer].step;
        }

        internal int countElementsInLastStep()
        {
            if (steps.Exists(x => x.step == 1))
            {
                return steps.Find(x => x.step == 1).elements.Count;
            }
            return 0;
        }
        internal bool containsElementsInLastStep()
        {
            if (this.countElementsInLastStep() == 0)
                return false;
            return true;
        }
        internal List<Neuron> getNeuronsStartingStep(int when)
        {
            List<Neuron> neurons = new List<Neuron>();
            for (; when < brain.temporalPatternLength; when++)
            {
                
                foreach (QueueElement e in this.getStep(when).elements)
                {
                    neurons.Add(e.neuron);
                }
            }
            return neurons;
        }




        internal bool containsElementAfterStep(Neuron c, int when)
        {

            if (this.getNeuronsStartingStep(when).Contains(c))
            {
                return true;
            }
            return false;
        }




        internal bool FirstStepContainsSuccessorOf(Neuron c)
        {
            foreach (Neuron n in c.getSuccessors())
            {
                if (this.getNeuronsInStep(0).Contains(n))
                    return true;
            }
            return false;
        }
    }
}
