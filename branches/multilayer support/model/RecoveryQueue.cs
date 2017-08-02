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
            this.maxSteps = brain.temporalPatternLength;
        }
        private int maxSteps;
        internal void add(Neuron neuron)
        {
            this.addToStep(neuron,0);
        }


        internal void nextStep()
        {
            this.incrementSteps();
            this.removeStep(maxSteps);
            brain.log("next Step in RecoveryQueue");
        }

        internal void setMaxSteps(int p)
        {
            this.maxSteps = p;
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

        
    }
}
