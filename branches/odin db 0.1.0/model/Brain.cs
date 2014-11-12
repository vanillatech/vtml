using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{

    class Brain
    {
        private int nextFreeID = 1;
        public double activationThreshold = 1.0;
        public double synapseDefaultStrength = 1.0;
        public bool isInLearnMode = false;
        private string outPutStack = "";
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
        public string query(string inp, bool learnMode = false) {
            this.outPutStack = "";
            this.isInLearnMode = learnMode;
            foreach (int n in inp)
            {
                if (this.input(n) == 1)
                    return(null);
            }
            this.thinkToEnd();
            return (this.outPutStack);
            
        }

        private void thinkToEnd()
        {
            while (activationQueue.containsElements() || recoveryQueue.containsElements())
                think();
        }
        public int input(int byteRead)
        {
            if (readSense.input(byteRead) == 1)
                    return(1);
            think();
            return (0);
        }
        public void think()
        {
            
            activationQueue.fireCurrentNeurons();
            activationQueue.nextStep();
            //this.reinforcementLearning();
            if (this.isInLearnMode && this.checkForNewPattern())
            {
                this.learnNewPatterns();
            }
            recoveryQueue.nextStep();
        }

        private void reinforcementLearning()
        {
            
        }

        private bool checkForNewPattern()
        {
            Neuron n;
            List<Neuron> commonSuccessors = new List<Neuron>();
            n = recoveryQueue.getNext();
            if (n != null)
            {
                commonSuccessors = n.getSuccessors();
            }
            while ((n = recoveryQueue.getNext()) != null)
            {
                List<Neuron> compare = n.getSuccessors();
                foreach (Neuron c in commonSuccessors)
                {
                    if (!compare.Contains(c))
                    {
                        commonSuccessors.Remove(c);
                    }
                }
                

            } 
            if (commonSuccessors.Count == 0 && !recoveryQueue.empty()) {
                return true;
            } else
                return false;
        }

        private void learnNewPatterns()
        {
            Neuron tmpNeuron = new Neuron(this);
            Neuron n;
            while ((n = recoveryQueue.getNext())!=null)
            {

                Dendrite tmpDendrite = tmpNeuron.getDendrite(recoveryQueue.getNextCurrentStep());
                n.synapseOn(tmpDendrite);

            } 
            
        }


        internal void addToRecoveryQueue(Neuron neuron)
        {
            recoveryQueue.add(neuron);
        }
        internal void addToActivationQueue(Neuron neuron, int when)
        {
            this.activationQueue.addToStep(neuron,when);
        }



        public Monitor addMonitor()
        {
            return new Monitor(this,this.readSense);
        }

        internal int getNextID()
        {
            return nextFreeID++;
        }

        internal void addToOutputStack(int p)
        {
            this.outPutStack += p.ToString();
        }
    }
}
