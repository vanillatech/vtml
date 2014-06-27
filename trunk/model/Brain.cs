using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace odin.model
{

    class Brain
    {
        public Brain()
        {
            this.readSense = new Sense(this);
        }


        Sense readSense;

        internal ActivationQueue activationQueue = new ActivationQueue();
        public void input(string inp) {
            foreach (int n in inp)
            {
                this.input(n);
            }
        }
        public void input(int byteRead)
        {
            readSense.input(byteRead);
            activationQueue.nextStep();
        }
        public void think()
        {
            activationQueue.nextStep();
        }
        
    }
}
