using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{

    class Brain
    {
        private Monitor monitor;
        private int nextFreeID = 1;
        public UInt64 currentStep = 0;
        //Model parameters
        public double activationThreshold = 0.5;
        public double synapseDefaultStrength = 0.8;
        public int synapseDefaultCount = 1;
        public int synapseMaxCount = 999;
        public double leakageFactor = 0.9;
        public int maxLayer = 8;
        public double adaptionRate = 0.4;
        public bool learnOnActivate = false;
        public bool learnOnFire = true;
        public double inhibitFactor = 0.5;
        public int temporalPatternLength = 3;
        public bool distributeActivationAmongSynapses = false;
        public bool activateNeuronBasedOnInputSynapses = true;
        //--

        public Brain()
        {
            this.activationQueue =  new ActivationQueue(this);
            this.recoveryQueue = new RecoveryQueue(this);
            this.readSense = new Sense(this);
            recoveryQueue.setMaxSteps(2);
        }
        internal void log(String s) {
            if (this.monitor != null)
            {
                monitor.log(s);
            }
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
            activationQueue.leakActivation();
            activationQueue.processActivation();
            activationQueue.fireCurrentNeurons();
            activationQueue.nextStep();
            this.reinforcementLearning();
            if (this.checkForRecentlyFiredNeuronsWithoutCommonSuccessor())
            {
                this.associateLastFiredNeuronsWithNewNeuron();
            }
            this.associateLastStepNeuronsWithCurrentStep();
            recoveryQueue.nextStep();
            this.currentStep++;
        }

        private void associateLastStepNeuronsWithCurrentStep()
        {
            foreach (Neuron n in recoveryQueue.getNeuronsInStep(1))
            {
                foreach (Neuron c in recoveryQueue.getNeuronsInStep(0))
                {
                    n.synapseOn(c.getDendrite(1));
                }
            }
        }

        private void reinforcementLearning()
        {
            foreach (Neuron n in recoveryQueue.getNeuronsInStep(0))
            {
                foreach (Dendrite dendrite in n.getDendrites())
                {
                    foreach (Synapse synapse in dendrite.getSynapses()) {
                        Neuron predecessor = synapse.getPredecessor();
                    
                        if ((int)(currentStep - predecessor.lastFired) == dendrite.length)
                        {
                            //Hebbian learning:
                            //if neuron has been involved in firing the successor reinforce
                            if (this.learnOnFire) synapse.reinforce(true);
                        }
                        else
                        {
                            //if neuron has not been involved create an inhibitive synapse
                            synapse.reinforce(false);
                        }
                    
                    }
                }
            }
        }

        private bool checkForRecentlyFiredNeuronsWithoutCommonSuccessor()
        {
            Neuron n;
            List<Neuron> commonSuccessors = new List<Neuron>();
                       
            n = recoveryQueue.getNext();
            if (n != null)
            {
                commonSuccessors = n.getSuccessors();
                
            }
            List<Neuron> commonSuccessorsIterate = new List<Neuron>(commonSuccessors);
            while ((n = recoveryQueue.getNext()) != null)
            {
                List<Neuron> compare = n.getSuccessors();
                
                foreach (Neuron c in commonSuccessorsIterate)
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

        private void associateLastFiredNeuronsWithNewNeuron()
        {
            Neuron tmpNeuron = new Neuron(this);
            
            Neuron n;
            while ((n = recoveryQueue.getNext())!=null)
            {

                Dendrite tmpDendrite = tmpNeuron.getDendrite(recoveryQueue.getCurrentStep()+1);
                n.synapseOn(tmpDendrite);
                if (n.layer >= tmpNeuron.layer) tmpNeuron.layer = n.layer + 1;
                if (n.layer == this.maxLayer) tmpNeuron.layer = n.layer;

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
            this.monitor = new Monitor(this,this.readSense);
            return monitor;
        }

        internal int getNextID()
        {
            return nextFreeID++;
        }

        internal void lateralInhibition(int layer)
        {
            activationQueue.inhibitNeuronsInStep(0,layer);
        }

        internal void monitorOutput(int p)
        {
            if (p > 0 && this.monitor != null) {
                monitor.output(p);
            }
        }
    }
}
