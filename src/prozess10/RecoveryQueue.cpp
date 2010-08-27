//#include "stdafx.h"
#include "globals.h"
#include "Neuron.h"
#include "Dendrite.h"
#include "../callbacks.h"
#include "RecoveryQueue.h"

using namespace Odin;
extern ICallback* callback;

RecoveryQueue::RecoveryQueue (Layer *caller) {
		this->queue.resize(globals.queueMax);
		this->counter = 0;
		this->lastelement = 0;
		this->stepLastElement = 0;
		this->layer = caller;
		this->focusStep = 0;
		this->getElement = 0;
		//this->lastInsertedElement = 0;
}

int RecoveryQueue::deletePattern(Neuron *neuron, unsigned int pos, unsigned int forderOld) {
   int forderNew = 0;
   int retVal = 0;
   if (pos == neuron->dendrites.size()) {return(1);}
   if (pos > 0) {
	   int delay = neuron->dendrites[pos]->activationDelay - neuron->dendrites[pos - 1]->activationDelay;
	   forderNew = forderOld - delay;
	   if (forderNew < 0) forderNew+=globals.queueMax;
	   for (int n = 0;n<globals.queueMax && !queue[forderNew].empty();n++) {
		if (neuron->dendrites[pos]->dendriteFrom == queue[forderNew][n]) {
		  retVal = deletePattern(neuron,pos+1,forderNew);
		  if (retVal == 1) {

#ifdef BORLAND_GUI
			Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[forderNew][n]->id);
#else
			callback->onCallback(new CallbackMsg<MSG_DELETE_FROM_RQ>(layer->number, queue[forderNew][n]->id));
#endif
			queue[forderNew].erase(queue[forderNew].begin()+n);
			return(1);
		  }

		}
	   }
   } else {
	for (int m=0;m<globals.queueMax ;m++ ) {
	for (unsigned int n=0;n < queue[m].size() ;n++ ) {
			if (neuron->dendrites[pos]->dendriteFrom == queue[m][n] ) {
				retVal = deletePattern(neuron,pos+1,m);
				if (retVal == 1) {

#ifdef BORLAND_GUI
                   Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[m][n]->id);
#else
	callback->onCallback(new CallbackMsg<MSG_DELETE_FROM_RQ>(layer->number, queue[m][n]->id));
#endif
				   queue[m].erase(queue[m].begin()+n);
				   return(1);
				}
			}
	}}
   }
   return(retVal);
}


void RecoveryQueue::insert(Neuron *neuron) {
        //if a new neuron is inserted into the queue it is not idle anymore.
		//if (recQueueIdle >= this->layer) recQueueIdle = this->layer - 1;

		//insert actual neuron
		this->queue[this->counter].push_back(neuron);
		//this->lastInsertedElement = stepCounter;
		//new type=2 neuron
		/*if (this->layer > 1) {

			Neuron *dNeuron = new Neuron(this->layer,2);
			neuron->newLink(dNeuron,0,1);
			this->lastType2Neuron = dNeuron;
		}*/
		//link lastType2Neuron of uppler layer to this element
		/* -- segmentation fault
		if (recQueue[this->layer+1].lastType2Neuron != 0) {
			recQueue[this->layer+1].lastType2Neuron->newLink(neuron,0,1);
		}*/

}

void RecoveryQueue::checkNewPattern() {
	//Associate everything in recQueue with a new neuron
		//check if there are input neurons in current step.
		if ( this->countInputNeuronsCurrentStep() > 0) {
			//check if there is a pattern not yet known. Means: check if
			//all of the neurons in current time step have a connection to same
			//successor.
			bool patternIsAlreadyKnown = true;
			//if there is only one neuron without axon in current step
			/*if (queue[this->counter].size() == 1)
				if (queue[this->counter][0]->axons.size() == 0)
					patternIsAlreadyKnown = false;*/
			for (unsigned int n=0;n<queue[this->counter].size();n++ ) {
				for (unsigned int g=0;n<queue[this->counter].size();g++ ) {
					Neuron *q = queue[this->counter][n];
					Neuron *q2 = queue[this->counter][g];
					//if a neuron has no successor yet, insert one and activate
					if (q->axons.size() == 0) {
						Neuron *nNew = new Neuron(this->layer,1);
						Dendrite *d = q->newLink(nNew);
						d->stimulate();
					}
					
				
					//if a (newly created) intermediate neuron has no neighbors yet, 
					//link it to all input neurons that have recently been fired.
					if (!queue[this->counter][g]->hasNeighbors() && queue[this->counter][g]->type == 1) {
						for (unsigned int m=0;m<queue[this->counter].size();m++ ) {
							if (queue[this->counter][m]->type == 0 && (g) != m)
								queue[this->counter][m]->newNeighbor(queue[this->counter][g]);
						}
					}
					//if a neuron is located next to another neuron and both neurons don't synapse on a commmon successor we insert one
					if (queue[this->counter][n]->isNeighborOf (queue[this->counter][g]) && !queue[this->counter][n]->hasSameSuccessor(queue[this->counter][g])) {
						patternIsAlreadyKnown = false;
					}
					//check for neighborhood connection 
					/*if (!queue[this->counter][g]->isNeighborOf (queue[this->counter][n])) {
						patternIsAlreadyKnown = false;
					}*/
				}
			}
			if (!patternIsAlreadyKnown) {
				//new neuron in same layer, type intermediate.
				Neuron *newNeuron = new Neuron(this->layer,1);
				Dendrite *newDend;
				for (unsigned int n=0;n<queue[this->counter].size();n++ ) {

					newDend = queue[this->counter][n]->newLink(newNeuron);
					newDend->stimulate(0);
				}
			}
			
		}

		/*if (recQueue.capacity() < recQueue.size() + 3) {
			recQueue.reserve(recQueue.size()+3);
		} */
}

void RecoveryQueue::recover(void) {
	//a new time step begins... first check if we learn something in recent
	//time step
    if (stepLastElement != this->layer->step) {


		this->lastelement = this->counter;
		this->checkNewPattern();
		this->counter++;
		if (this->counter>=globals.queueMax) {  this->counter = 0; }
		this->queue[this->counter].clear();

		stepLastElement = globals.stepCounter;
	}
}

/*bool RecoveryQueue::noChangeInCycle() {
	if (recQueue[this->layer+1].lastInsertedElement <=stepCounter - queueMax && (recQueue[this->layer+1].lastInsertedElement != 0 || stepCounter > queueMax)) {
		return(true);
	}
	return(false);
} */

int RecoveryQueue::countItems() {
	int cItems = 0;
	for (int n=0;n<globals.queueMax ;n++ ) {
		cItems += queue[n].size();
	}
	return (cItems);
}


int RecoveryQueue::countInputNeuronsCurrentStep() {
	int cItems = 0;
	for (unsigned int n=0;n<queue[this->counter].size() ;n++ ) {
		if (queue[this->counter][n]->type == 0) cItems ++;
	}
	return (cItems);
}

int RecoveryQueue::countItemsLastStep() {
	int cItems = 0;
	return (queue[this->getStep(-1)].size());
}

int RecoveryQueue::getStep(int val) {
	//gets queue position relative from current recoverystep
	if (val > globals.queueMax || val < -(globals.queueMax)) return -1;
	if (this->counter + val >=globals.queueMax) { 
		val -= this->counter;
		return (val); 
	}
	if (this->counter + val < 0) {
		val += this->counter;
		return ( globals.queueMax + val);
	}
	return (this->counter + val);

}

Neuron *RecoveryQueue::getNext() {
	this->getElement++;
	if (this->queue[this->focusStep].size() < this->getElement) return 0;
	return (this->queue[this->focusStep][this->getElement-1]);
}

int RecoveryQueue::setFocusStep(int val) {
	//sets the focus for this::getNext on specific queue position
	int s = this->getStep(val);
	if (s == -1) return -1;
	else {
		this->focusStep = s;
		this->getElement = 0;
		return (s);
	}
}