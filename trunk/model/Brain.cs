﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()]  
    class Brain
    {
        private Monitor monitor;
        private int nextFreeID = 1;
        
        public UInt64 leaveLearnModeAfter = 0; //0 to disable
        public bool isInLearnMode { get { return lm; } set { if (leaveLearnModeAfter > currentStep || leaveLearnModeAfter == 0) lm = value; else lm = false; } }
        private bool lm = false;
        private string outPutStack = "";

        public UInt64 currentStep = 0;
        //Model parameters
        public double activationThreshold = 0.52;
        public double activationThresholdInLearnMode = 0.49;
        public double synapseDefaultStrength = 0.5;
        public int synapseDefaultCount = 1;
        public int synapseMaxCount = 999;
        public double leakageFactor = 0; //0==remove all activation after each step, 1==store activation forever
        public int maxLayer = 2;
        public double adaptionRate = 0.1;
        public bool learnOnActivate = false;
        public bool learnOnFire = true;
        public double inhibitFactor = 0; // 0==WTA, 1==no lateral inhibition
        public int temporalPatternLength = 1;
        public bool distributeActivationAmongSynapses = false;
        public bool activateNeuronBasedOnInputSynapses = false;
        public int refractoryPeriod = 1;
        public double forgetRate = 0.1;
        public UInt64 forgetAfterSteps = 0; //0 to disable
        public DateTime inactiveSince = DateTime.Now;
        //--

        public Brain()
        {
            this.activationQueue =  new ActivationQueue(this);
            this.recoveryQueue = new RecoveryQueue(this);
            
            this.readSense = new Sense(this);
            this.contextSense = new Sense(this,true);
            //recoveryQueue.setMaxSteps(temporalPatternLength);
        }
        internal void log(String s) {
            if (this.monitor != null)
            {
                monitor.log(s);
            }
        }

        Sense readSense,contextSense;

        internal ActivationQueue activationQueue; 
        internal RecoveryQueue recoveryQueue;

        public string query(string inp, bool learnMode = false) {
            this.outPutStack = "";
            this.isInLearnMode = learnMode;
            foreach (int n in inp)
            {
                if (this.input(n) == 1)
                    return(null);
                //this.thinkToEnd();

            }
            //this.thinkToEnd();
            return (this.outPutStack);
            
        }
        public string query(int[] inp, int[] context=null, bool learnMode = false)
        {
            this.outPutStack = "";
            this.isInLearnMode = learnMode;

            if (inp != null)
            {
                foreach (int n in inp)
                {
                    //if (this.input(n) == 1)
                    //    return (null);
                    this.input(n);
                    //this.thinkToEnd();

                }
            }
            if (context != null)
            {
                foreach (int n in context)
                {
                    //if (contextSense.input(n) == 1)
                    //return (null);
                    contextSense.input(n);
                    //this.thinkToEnd();

                }
            }
            think();
            //this.thinkToEnd();
            return (this.outPutStack);

        }
        public string query(int inp, bool learnMode = false)
        {
            this.outPutStack = "";
            this.isInLearnMode = learnMode;
            if(this.input(inp) == 1)
                return (null);
            think();
            return (this.outPutStack);

        }

        public void thinkToEnd()
        {
            while (!activationQueue.empty() || !recoveryQueue.empty())
                think();
        }
        public void think(int outputLMT)
        {
            if (outputLMT > 0)
            {
                for (int n = 0; n <= outputLMT + 1 && (!activationQueue.empty() || !recoveryQueue.empty()); n++)
                    think();
                this.stopThinking();
            }
        }

        private void stopThinking()
        {
            activationQueue.clear();
            recoveryQueue.clear();

        }
        public int input(int byteRead)
        {
            if (readSense.input(byteRead) == 1)
                    return(1);
            //think();
            return (0);
        }
        public void think()
        {


            this.inactiveSince = DateTime.Now;
            activationQueue.nextStep();
            activationQueue.leakActivation();
            activationQueue.processActivation();
            activationQueue.fireCurrentNeurons();
            activationQueue.removeStep(0);

            
            if (this.isInLearnMode)
            {
                this.reinforcementLearning();
                if (this.checkForRecentlyFiredNeuronsWithoutCommonSuccessor())
                {
                    Neuron tmpNeuron = this.associateLastFiredNeuronsWithNewNeuron();

                    //activationQueue.addToStep(tmpNeuron, 1, this.synapseMaxCount * 1);
                }
                //this.associateLastStepNeuronsWithCurrentStepInputNeurons();
            }
            recoveryQueue.nextStep();
            this.currentStep++;
            this.log("-----Next thinkstep: " + this.currentStep + " -----");
        }

        private void associateLastStepNeuronsWithCurrentStep()
        {
            foreach (Neuron n in recoveryQueue.getNeuronsInStep(1))
            {
                foreach (Neuron c in recoveryQueue.getNeuronsInStep(0))
                {
                    if (n.layer > c.layer)
                    {
                        n.synapseOn(c.getDendrite(1));
                    }
                }
            }
        }
        private void associateLastStepNeuronsWithCurrentStepInputNeurons()
        {
            foreach (Neuron n in recoveryQueue.getNeuronsInStep(1))
            {
                foreach (Neuron c in recoveryQueue.getInputNeuronsInStep(0))
                {
                    if (n.layer > c.layer)
                    {
                        n.synapseOn(c.getDendrite(1));
                    }
                }
            }
        }

        private void reinforcementLearning()
        {
            if (this.learnOnFire)
            {
                foreach (Neuron n in recoveryQueue.getNeuronsInStep(0))
                {
                    foreach (Dendrite dendrite in n.getDendrites())
                    {
                        foreach (Synapse synapse in dendrite.getSynapses())
                        {
                            Neuron predecessor = synapse.getPredecessor();

                            if ((int)(currentStep - predecessor.lastFired) == dendrite.length)
                            {
                                //Hebbian learning:
                                //if neuron has been involved in firing the successor reinforce
                                synapse.reinforce(true);
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
        }

        private bool checkForRecentlyFiredNeuronsWithoutCommonSuccessor()
        {
            Neuron n;
            List<Neuron> commonSuccessors = new List<Neuron>();
                       
            n = recoveryQueue.getFirst(0);
            if (n == null)
            { //if recoveryQueue is empty we don't have commonSuccessors
                return false;
            }
            commonSuccessors = n.getSuccessors();
            // if only 1 element left wo input neuron 
            if (n.layer > 1 && recoveryQueue.countElements(0) == 1)
                return false;
            
            List<Neuron> commonSuccessorsIterate = new List<Neuron>(commonSuccessors);
            while ((n = recoveryQueue.getNext()) != null)
            { // every pair must have at least one common successor
                List<Neuron> compare = n.getSuccessors();
                
                foreach (Neuron c in commonSuccessorsIterate)
                {
                    if (c.type == 2 || !compare.Contains(c))
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

        private Neuron associateLastFiredNeuronsWithNewNeuron()
        {
            Neuron tmpNeuron = new Neuron(this);
            //tmpNeuron.lastFired = currentStep+1;
            Neuron n;
            while ((n = recoveryQueue.getNext())!=null)
            {

                Dendrite tmpDendrite = tmpNeuron.getDendrite(recoveryQueue.getCurrentStep()+1);
                n.synapseOn(tmpDendrite);
                
                if (n.layer >= tmpNeuron.layer) tmpNeuron.layer = n.layer + 1;
                if (n.layer == this.maxLayer) tmpNeuron.layer = n.layer;
                if (n.type == 1) //if n is an input neuron
                {
                    //create an output neuron that gets inhibited by current inputneuron and that gets excited by tmpNeuron
                    //this causes the outputneuron only to be fired in case we didn't input the same value
                    Neuron outputNeuron = new Neuron(this);
                    Dendrite od1 = outputNeuron.getDendrite(recoveryQueue.getCurrentStep() + 2);
                    n.synapseOn(od1,true);
                    
                    outputNeuron.tag = n.tag;
                    outputNeuron.type = 2;
                    Dendrite od2 = outputNeuron.getDendrite(recoveryQueue.getCurrentStep() + 1);
                    tmpNeuron.synapseOn(od2);
                }
                else
                {

                }
            }
            return tmpNeuron;
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
        
        internal void addToOutputStack(int p)
        {
            this.outPutStack += p + ", ";
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

        internal string getOutput()
        {
            String tmpop = this.outPutStack;
            this.outPutStack = "";
            return tmpop;
        }

        
    }
}
