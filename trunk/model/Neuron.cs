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

        List<Dendrite> dendrites;
        internal Neuron()
        {
            axon = new Axon(this);
            
        }
        internal Dendrite getNewDendrite() {
            Dendrite tmp = new Dendrite(this);
            dendrites.Add(tmp);
            return tmp;
        }

    }
}
