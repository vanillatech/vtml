using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{
    class Sense
    {
        List<Dendrite> cInputNeurons;

        public void activate(int index)
        {
            cInputNeurons[index].activate();
        }
        public void generateAlphaNumericalInputs()
        {
            for (int n = 0; n < 256; n++)
            {
                Neuron tmp = new Neuron();
                cInputNeurons.Add(tmp.getNewDendrite());
                
            }
        }
    }
}
