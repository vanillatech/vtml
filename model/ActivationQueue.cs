﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    [Serializable()] 
    class ActivationQueue : Queues
    {
        Brain brain;
        internal ActivationQueue(Brain mybrain)
        {
            this.brain = mybrain;
        }
        internal void nextStep()
        {
                   
            
            this.decrementSteps();
            

            //brain.log("next Step in ActivationQueue");
        }

        

        internal void fireCurrentNeurons()
        {

            Parallel.ForEach(this.getElementsInStepOrderByActivation(0), a =>
            {
                if (a.neuron.crossesThreshold())
                {
                    if (!a.neuron.isWithinRefractoryPeriod())
                    {
                        a.neuron.fire();
                    }
                }

            });
        }


        internal void processActivation()
        {
            Parallel.ForEach(this.getElementsInStep(0), a =>
            {
                a.neuron.polarize(a.val);

            });
            
        }

        internal void leakActivation()
        {
            foreach (QueueElement a in this.getElementsInStep(0))
            {
                a.neuron.leakActivation();

            }
        }

        internal void inhibitNeuronsInStep(int p,int l)
        {
            foreach (QueueElement a in this.getElementsInStep(p))
            {
                if (a.neuron.layer == l && a.neuron.type != 3 )
                    a.neuron.inhibit();
                

            }
        }
        internal void inhibitOutputNeuronsInStep(int p)
        {
            foreach (QueueElement a in this.getElementsInStep(p))
            {
                if (a.neuron.type == 2)
                    a.neuron.inhibit();


            }
        }


    }
}
