using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace odin.model
{
    class Queues
    {
        internal struct QueueElement
        {
            internal Neuron neuron;
            internal double val;
        }
        internal struct Step
        {
            internal List<QueueElement> element;
            internal int step;
        
        }
        internal List<Step> steps = new List<Step>();
        internal void addToStep(Neuron neuron, int when)
        {
            this.addToStep(neuron, when, 0);
        }
        internal void addToStep(Neuron neuron, int when, double value)
        {
            
                Step tmp = this.getStep(when);
                QueueElement aqe = new QueueElement();
                aqe.neuron = neuron;
                aqe.val = value;
                tmp.element.Add(aqe);
                         
            
        }

        internal Step getStep(int when)
        {
            if (steps.Exists(x => x.step == when))
            {
                return steps.Find(x => x.step == when);
            }
            else
            {
                Step tmp = new Step();
                tmp.element = new List<QueueElement>();
                tmp.step = when;
                steps.Add(tmp);
                return tmp;
            }
        }

        internal List<Neuron> getNeuronsInStep(int when)
        {
            List<Neuron> neurons = new List<Neuron>();
            foreach (QueueElement e in this.getStep(when).element)
            {
                neurons.Add(e.neuron);
            }
            return neurons;
        }
        internal void decrementSteps()
        {
            for (int s = 0; s < steps.Count; s++)
            {
                Step tmp = steps[s];
                tmp.step--;
                steps[s] = tmp;
            }
        }
        internal void incrementSteps()
        {
            for (int s = 0; s < steps.Count; s++)
            {
                Step tmp = steps[s];
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
