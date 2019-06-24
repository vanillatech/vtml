﻿using System;
using System.Collections.Generic;
using System.Drawing;
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
        public bool isInLearnMode { get { return lm; } set { if (leaveLearnModeAfter > inputLayer.currentStep || leaveLearnModeAfter == 0) lm = value; else lm = false; } }
        private bool lm = false;
        private string outPutStack = "";
        internal Dictionary<int,Neuron> outputNeurons = new Dictionary<int,Neuron> ();
        internal Dictionary<int, string> intToFeature = new Dictionary<int, string>();
        internal Dictionary<string, int> featureToInt = new Dictionary<string, int>();
        
        //Model parameters
        public double activationThreshold = 0.55;
        public double activationThresholdInLearnMode = 0.49;
        public double synapseDefaultStrength = 0.5;
        public int synapseDefaultCount = 1;
        public int synapseMaxCount = 999;
        public double leakageFactor = 0.1; //0==remove all activation after each step, 1==store activation forever
        public int maxLayer = 1;
        public double adaptionRate = 0.1;
        public bool learnOnActivate = false;
        public bool learnOnFire = true;
        public bool activateNewNeurons = true;
        public double inhibitFactor = 0.1; // 0==WTA, 1==no lateral inhibition
        public int temporalPatternLength = 2;
        public bool distributeActivationAmongSynapses = false;
        public bool activateNeuronBasedOnInputSynapses = false;
        public int refractoryPeriod = 1;
        public double forgetRate = 0.1;
        public UInt64 forgetAfterSteps = 0; //0 to disable
        public DateTime inactiveSince = DateTime.Now;
        public bool temporary = false;
        //--

        public Brain()
        {
            this.inputLayer = new Layer(this);
            this.outputLayer = new Layer(this);
            this.addFeature();
            this.readSense = new Sense(this);
            
            //recoveryQueue.setMaxSteps(temporalPatternLength);
        }

        private void addFeature()
        {
            //this.featureMatrix.Add(new Sense(this, true));
            this.featureMatrix.Add(new Sense(this)); //disable nooutput as output now on outputlayer
        }
        internal void log(String s,Brush b ) {
            if (this.monitor != null)
            {
                monitor.log(s,b);
            }
        }
        internal void log(String s)
        {
            this.log(s, Brushes.Black);
        }

        Sense readSense;
        List <Sense> featureMatrix = new List<Sense>();

        internal Layer inputLayer, outputLayer;
        
        public int desiredOutput;

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
                    if (n!=0)
                        this.input(n);
                    //this.thinkToEnd();

                }
            }
            if (context != null)
            {
                for (int n = 0; n < context.Count();n++ )
                {
                    if (this.featureMatrix.Count < (n + 1))
                        this.addFeature();
                    //if (contextSense.input(n) == 1)
                    //return (null);
                    if (context[n]!=0)
                        this.featureMatrix[n].input(context[n]);
                    //this.thinkToEnd();

                }
            }
            //think();
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


        public int input(int byteRead)
        {
            if (readSense.input(byteRead) == 1)
                    return(1);
            //think();
            return (0);
        }
        public void think(int lmt = 0)
        {


            this.inactiveSince = DateTime.Now;

            if (lmt > 0) inputLayer.think(lmt); 
            else inputLayer.think();

            
            
        }

        

        internal Neuron getOutputNeuron(int p)
        {
            if (!this.outputNeurons.ContainsKey(p))
            {
                
                this.outputNeurons.Add(p, new Neuron(this,outputLayer));
                this.outputNeurons[p].tag = p;
                this.outputNeurons[p].type = 2;

            }
            return this.outputNeurons[p];
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
            this.outPutStack += this.getIntAsFeature(p) + ", ";
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



        internal void setTemporalPatternLength(int p)
        {
            this.temporalPatternLength = p;

        }

        internal int getFeatureAsInt(string p)
        {
            if (p == "") return 0;
            int val = this.featureToInt.Count + 1;
            if (!this.featureToInt.ContainsKey(p))
            {
                this.featureToInt.Add(p, val);
                this.intToFeature.Add(val, p);
            }
            return (this.featureToInt[p]);
        }
        internal string getIntAsFeature(int val)
        {
            if (val == 0) return "";
            if (this.intToFeature.ContainsKey(val))
            {

                return (this.intToFeature[val]);
            }
            else return val.ToString();
        }
    }
}