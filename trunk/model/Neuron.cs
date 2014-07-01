using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Neuron
    {
        Axon axon;
        Brain brain;
        List<Dendrite> dendrites = new List<Dendrite>();
        internal Neuron(Brain mybrain)
        {
            axon = new Axon(this,mybrain);
            brain = mybrain;
        }
        internal Dendrite getNewDendrite(int length) {
            Dendrite tmp = new Dendrite(this,brain,length);
            dendrites.Add(tmp);
            return tmp;
        }
        internal void fire()
        {
            brain.addToRecoveryQueue(this);
        }

    }
}
