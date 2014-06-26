using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{

    class Brain
    {
        public Brain()
        {
            this.addSense();
        }

        private void addSense()
        {
            senses.Add(new Sense());
        }

        List<Sense> senses = new List<Sense>();
        internal struct aQueue
        {
            internal List<Neuron> neuron;
            internal int stepsLeft;
        }
        internal List<aQueue> activationQueue;
        void addToActivationQueue(Neuron neuron,int when)
        {
            this.getActivationQueue(when).neuron.Add(neuron);
        }

        private aQueue getActivationQueue(int when)
        {
            if (activationQueue.Exists(x => x.stepsLeft == when)) { 
                return activationQueue.Find(x => x.stepsLeft == when);
            }
            else
            {
                aQueue tmp = new aQueue();
                tmp.stepsLeft = when;
                activationQueue.Add(tmp);
                return tmp;
            }
        }
    }
}
