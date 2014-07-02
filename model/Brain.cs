using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{

    class Brain
    {
        public double activationThreshold = 0.5;
        public double synapseDefaultStrength = 0.1;
        public Brain()
        {
            this.activationQueue =  new ActivationQueue(this);
            this.recoveryQueue = new RecoveryQueue(this);
            this.readSense = new Sense(this);
            recoveryQueue.setMaxSteps(2);
        }


        Sense readSense;

        internal ActivationQueue activationQueue; 
        internal RecoveryQueue recoveryQueue; 
        public void input(string inp) {
            foreach (int n in inp)
            {
                this.input(n);
            }
        }
        public void input(int byteRead)
        {
            readSense.input(byteRead);
            think();
        }
        public void think()
        {
            activationQueue.fireCurrentNeurons();
            activationQueue.nextStep();
            recoveryQueue.learnNewPatterns();
            recoveryQueue.nextStep();
        }


        internal void addToRecoveryQueue(Neuron neuron)
        {
            recoveryQueue.add(neuron);
        }
        internal void addToActivationQueue(Neuron neuron, int when)
        {
            this.activationQueue.addToStep(neuron,when);
        }

        
    }
}
