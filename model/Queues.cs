using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace odin.model
{
    class Queues
    {
        internal struct aQueue
        {
            internal List<Neuron> neuron;
            internal int step;
            internal double val;
        }
        internal List<aQueue> steps = new List<aQueue>();
        internal void addToStep(Neuron neuron, int when)
        {
            if (when > 0)
                this.getStep(when).neuron.Add(neuron);
        }

        internal aQueue getStep(int when)
        {
            if (steps.Exists(x => x.step == when))
            {
                return steps.Find(x => x.step == when);
            }
            else
            {
                aQueue tmp = new aQueue();
                tmp.neuron = new List<Neuron>();
                tmp.step = when;
                steps.Add(tmp);
                return tmp;
            }
        }
        internal void decrementSteps()
        {
            for (int s = 0; s < steps.Count; s++)
            {
                aQueue tmp = steps[s];
                tmp.step--;
                steps[s] = tmp;
            }
        }
        internal void incrementSteps()
        {
            for (int s = 0; s < steps.Count; s++)
            {
                aQueue tmp = steps[s];
                tmp.step++;
                steps[s] = tmp;
            }
        }

        internal void removeStep(int p)
        {
            if (steps.Exists(n => n.step == p))
            {
                steps.Remove(getStep(p));
            }
        }
    }
}
