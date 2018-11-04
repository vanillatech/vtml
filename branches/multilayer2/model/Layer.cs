using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()]
    class Layer
    {
        internal ActivationQueue activationQueue;
        internal RecoveryQueue recoveryQueue;
        internal Brain brain;
        internal Layer higher;
        internal Layer lower;
        public UInt64 currentStep = 0;
        public int number;
        private bool idle;

        public Layer(Brain brain, Layer caller = null)
        {
            this.brain = brain;
            this.lower = caller;
            if (caller == null) this.number = 0;
            else this.number = lower.number + 1;
            this.activationQueue = new ActivationQueue(brain);
            this.recoveryQueue = new RecoveryQueue(brain);
        }

        internal void think()
        {
            this.idle = true;
            if (this.higher != null)
            {
                if (!this.higher.isIdle())
                {
                    this.higher.think();
                }
            }
            else if (!brain.outputLayer.isIdle()) { brain.outputLayer.think(); }

            this.currentStep++;
            brain.log("-----Next thinkstep (Layer: " + this.number + "): " + this.currentStep + " -----", Brushes.Black);
            

            activationQueue.nextStep();
            activationQueue.leakActivation();
            activationQueue.processActivation();
            activationQueue.fireCurrentNeurons();
            activationQueue.removeStep(0);

            if (brain.isInLearnMode)
            {
                this.reinforcementLearning();

                /*if (this.checkForRecentlyFiredNeuronsWithoutCommonSuccessor())
                {
                    Neuron tmpNeuron = this.associateLastFiredNeuronsWithNewNeuron();

                    if (brain.activateNewNeurons)
                    {
                        activationQueue.addToStep(tmpNeuron, 1, brain.synapseMaxCount * 1);
                    }
                }*/
                this.associateLastStepsNeuronsWithCurrentStep();
                this.checkNewPattern();


                //this.associateLastStepNeuronsWithCurrentStepInputNeurons();

            }

            recoveryQueue.nextStep();
            

            
        }

        private void checkNewPattern()
        {
            Neuron outputNeuron = brain.getOutputNeuron(brain.desiredOutput);
            Neuron newInputNeuronInNextLayer = new Neuron(brain, this.getHigher());
            Neuron newInterNeuron = new Neuron(brain, this);

            for (int s = 1; s < brain.temporalPatternLength; s++)
            {
                foreach (Neuron c in recoveryQueue.getNeuronsInStep(s))
                {
                    if (!recoveryQueue.FirstStepContainsSuccessorOf(c))
                    {
                        //add a new neuron to represent this pattern
                        if (c.type == 0) //interneuron
                        {
                            if (s == 1) // only for last step neuron
                            {
                                if (!checkForCommonType1SuccessorInStep(1))
                                {
                                    if (this.number == brain.maxLayer)
                                    {



                                        Dendrite od1 = outputNeuron.getDendrite(s);
                                        c.synapseOn(od1);
                                        brain.log("New outputneuron: " + c.id + " to " + outputNeuron.tag, Brushes.LimeGreen);

                                    }
                                    else
                                    {

                                        newInputNeuronInNextLayer.type = 1;
                                        c.synapseOn(newInputNeuronInNextLayer.getDendrite(1));
                                        brain.log("New Input in Layer " + this.getHigher().number + ": " + c.id + " to " + newInputNeuronInNextLayer.id, Brushes.Orange);
                                    }
                                }
                            }
                        }
                        else if (c.type == 1) //inputneuron
                        {
                            
                            bool newLink = c.synapseOn(newInterNeuron.getDendrite(s));
                            brain.log("New Interneuron: " + c.id + " to " + newInterNeuron.id, Brushes.LightSkyBlue);

                        }

                    }
                }
            }
            
        }

        private bool isIdle()
        {
            return this.idle;
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
        private void associateLastStepsNeuronsWithCurrentStep()
        {
            for (int when = 1; when < brain.temporalPatternLength; when++)
            {
                foreach (Neuron n in recoveryQueue.getNeuronsInStep(when))
                {
                    foreach (Neuron c in recoveryQueue.getNeuronsInStep(0))
                    {
                        if (n.type == 0 && c.type == 0) // interneuron
                        {
                            bool newlink = n.synapseOn(c.getDendrite(when),0.1);
                            if (newlink) brain.log("New Link: " + n.id + " to " + c.id, Brushes.Turquoise);
                        }
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
                    if (n.layer.number > c.layer.number)
                    {
                        n.synapseOn(c.getDendrite(1));
                    }
                }
            }
        }

        private void reinforcementLearning()
        {
            if (brain.learnOnFire)
            {
                foreach (Neuron n in recoveryQueue.getNeuronsInStep(0))
                {
                    foreach (Dendrite dendrite in n.getDendrites())
                    {
                        foreach (Synapse synapse in dendrite.getSynapses())
                        {
                            //Neuron predecessor = synapse.getPredecessor();

                            if ((int)(currentStep - synapse.lastActivation) == dendrite.length)
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
            // if only 1 element left 
            if (recoveryQueue.countElements() == 1)
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
            if (commonSuccessors.Count == 0 && !recoveryQueue.empty())
            {
                return true;
            }
            else
                return false;
        }
        private bool checkForCommonType1SuccessorInStep(int stp)
        {
            Neuron n;
            List<Neuron> commonSuccessors = new List<Neuron>();

            n = recoveryQueue.getFirst(stp);
            if (n == null)
            { //if recoveryQueue is empty we don't have commonSuccessors
                return false;
            }
            commonSuccessors = n.getSuccessors();
            // if only 1 element left 
            if (recoveryQueue.countElements() == 1 && commonSuccessors == null)
                return false;

            List<Neuron> commonSuccessorsIterate = new List<Neuron>(commonSuccessors);
            while ((n = recoveryQueue.getNext(stp)) != null)
            { // every pair must have at least one common successor
                List<Neuron> compare = n.getSuccessors();

                foreach (Neuron c in commonSuccessorsIterate)
                {
                    if (c.type != 1 || !compare.Contains(c))
                    {
                        commonSuccessors.Remove(c);
                    }
                }


            }
            if (commonSuccessors.Count > 0)
            {
                return true;
            }
            else
                return false;
        }

        private Neuron associateLastFiredNeuronsWithNewNeuron()
        {
            Neuron tmpNeuron = new Neuron(brain,this.getHigher());
            
            //tmpNeuron.lastFired = currentStep+1;
            Neuron n;
            while ((n = recoveryQueue.getNext()) != null)
            {

                if (n.layer.number == brain.maxLayer)
                {

                    //tmpNeuron.layer = n.layer;

                    Neuron outputNeuron = brain.getOutputNeuron(brain.desiredOutput);

                    Dendrite od1 = outputNeuron.getDendrite(recoveryQueue.getCurrentStep() + 1);
                    n.synapseOn(od1);
                    //avoid intial activation after adding connection
                    outputNeuron.layer.activationQueue.addToStep(outputNeuron, 1, -od1.countSynapses());
                    return outputNeuron;

                }
                Dendrite tmpDendrite = tmpNeuron.getDendrite(recoveryQueue.getCurrentStep() + 1);
                n.synapseOn(tmpDendrite);
                tmpNeuron.type = 0;

                //if (n.layer >= tmpNeuron.layer) tmpNeuron.layer = n.layer + 1;

                /*if (n.type == 1) //if n is an input neuron
                {
                    //create an output neuron that gets inhibited by current inputneuron and that gets excited by tmpNeuron
                    //this causes the outputneuron only to be fired in case we didn't input the same value
                    Neuron outputNeuron = this.getOutputNeuron(n.tag);
                    Dendrite od1 = outputNeuron.getDendrite(recoveryQueue.getCurrentStep() + 2);
                    n.synapseOn(od1,true);
                    //avoid intial activation after adding connection
                    activationQueue.addToStep(outputNeuron, 2,-od1.countSynapses());
                    
                    //outputNeuron.tag = n.tag;
                    //outputNeuron.type = 2;
                    Dendrite od2 = outputNeuron.getDendrite(recoveryQueue.getCurrentStep() + 1);
                    tmpNeuron.synapseOn(od2);
                }*/

            }
            return tmpNeuron;
        }

        private Layer getHigher()
        {
            if (this.higher != null) return this.higher;
            else
            {
                if (this.number < brain.maxLayer)
                {
                    this.higher = new Layer(brain, this);
                }
            }
            return this.higher;
        }
        internal void addToRecoveryQueue(Neuron neuron)
        {
            recoveryQueue.add(neuron);
        }
        internal void addToActivationQueue(Neuron neuron, int when)
        {
            this.activationQueue.addToStep(neuron, when);
        }
        internal void lateralInhibition(Neuron caller)
        {
            activationQueue.inhibitNeuronsInStep(0,caller);
        }

        internal void setIdle(bool p)
        {
            this.idle = p;
        }
    }
}
