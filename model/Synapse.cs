using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Synapse
    {
        Axon fromAxon;
        Dendrite toDendrite;
        internal Synapse(Axon axon, Dendrite dendrite) {
            this.fromAxon = axon;
            this.toDendrite = dendrite;
        }
        internal void activate()
        {
            throw new NotImplementedException();
        }
    }
}
