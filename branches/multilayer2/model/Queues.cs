using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace odin.model
{
    [Serializable()] 
    class Queues
    {
        [Serializable()]
        internal struct QueueElement
        {
            internal Neuron neuron;
            internal double val;
        }
        [Serializable()] 
        internal struct Step
        {
            internal List<QueueElement> elements;
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
                tmp.elements.Add(aqe);
                neuron.layer.setIdle(false);         
            
        }
        internal int countElements(int inStep)
        {
            if (steps.Exists(x => x.step == inStep))
            {
                return steps.Find(x => x.step == inStep).elements.Count;
            }
       
            return 0;
        }
        internal int countElements() {
            int elements = 0;
            foreach (Step s in steps)
            {
                elements += s.elements.Count();
            }
            return elements; 
        }
        
        internal bool empty()
        {
            return (this.countElements() == 0);
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
                tmp.elements = new List<QueueElement>();
                tmp.step = when;
                steps.Add(tmp);
                return tmp;
            }
        }

        internal List<QueueElement> getElementsInStep(int when)
        {
            //return copy of list because List could be changed
            return new List<QueueElement>(getStep(when).elements);
        }

        internal List<QueueElement> getElementsInStepOrderByActivation(int when)
        {
            //return copy of list because List could be changed
            return new List<QueueElement>(getStep(when).elements.OrderByDescending(ele => ele.neuron.activation));
        }


        internal List<Neuron> getNeuronsInStep(int when)
        {
            List<Neuron> neurons = new List<Neuron>();
            foreach (QueueElement e in this.getStep(when).elements)
            {
                neurons.Add(e.neuron);
            }
            return neurons;
        }
        internal List<Neuron> getInputNeuronsInStep(int when)
        {
            List<Neuron> neurons = new List<Neuron>();
            foreach (QueueElement e in this.getStep(when).elements)
            {
                if (e.neuron.layer.number == 0)
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
        internal void clear()
        {
            for (int n = steps.Count - 1; n > 0; n-- )
            {
                steps.RemoveAt(n);
            }
        }
    }
}
