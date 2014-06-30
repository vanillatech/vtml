using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class ActivationQueue
    {
        internal struct aQueue
        {
            internal List<Neuron> neuron;
            internal int stepsLeft;
        }
        internal List<aQueue> steps = new List<aQueue>();
        internal void addToStep(Neuron neuron, int when)
        {
            if (when > 0)
                this.getStep(when).neuron.Add(neuron);
        }

        private aQueue getStep(int when)
        {
            if (steps.Exists(x => x.stepsLeft == when))
            {
                return steps.Find(x => x.stepsLeft == when);
            }
            else
            {
                aQueue tmp = new aQueue();
                tmp.neuron = new List<Neuron>();
                tmp.stepsLeft = when;
                steps.Add(tmp);
                return tmp;
            }
        }
        internal void nextStep()
        {
            this.decrementSteps();
            this.fireCurrentNeurons();
            this.removeStep(0);
            
        }

        private void decrementSteps()
        {
            for (int s = 0; s < steps.Count;s++ )
            {
                aQueue tmp = steps[s];
                tmp.stepsLeft--;
                steps[s] = tmp;
            }
        }

        private void removeStep(int p)
        {
            if (steps.Exists(n => n.stepsLeft == p))
            {
                steps.Remove(getStep(p));
            }
        }

        private void fireCurrentNeurons()
        {
            foreach (Neuron n in getStep(0).neuron) {
                n.fire(); 

            }
        }
    }
}
