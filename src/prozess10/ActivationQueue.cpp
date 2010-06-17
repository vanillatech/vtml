//#include "stdafx.h"
#include "globals.h"
#include "Dendrite.h"
#include "Neuron.h"
#include "ActivationQueue.h"

ActivationQueue::ActivationQueue (Layer *caller) {
	this->layer = caller;
	queue.resize(globals.queueMax);
}
void ActivationQueue::schedActivation (Dendrite* dendrite, int aDelay) {
		this->q.push(Aqueue(aDelay+this->layer->step,dendrite,1));
		this->layer->setIdle(false);
}

void ActivationQueue::schedActivation (Dendrite* dendrite, int aDelay, double aVal) {
		this->q.push(Aqueue(aDelay+this->layer->step,dendrite,aVal));
		this->layer->setIdle(false);
}

void ActivationQueue::activate (void) {
	//this->layer->newStep();
	priority_queue <Aqueue> qtemp;
	//first activate subsequent neurons
	while ((!this->q.empty()) && this->q.top().getPos() <= this->layer->step) {
		Dendrite *d;
		d = this->q.top().getDendrite();
		double actVal = 0;
		/*int cs = d->dendriteTo->countSynapses();
		if (cs > 0) actVal = d->synapses / double(d->dendriteTo->countSynapses());
		else actVal = 0;*/
		actVal = this->q.top().getAct();
		//actVal = 1;
		if (actVal > globals.minActivation) {
		if (d->dendriteTo->lastfired < (this->layer->step - globals.recoveryTime)) {
			d->dendriteTo->activate(actVal);
			qtemp.push(Aqueue(d->dendriteTo->activationVal,d));
		}}
		this->q.pop();
	}
	//now check if the activation leads to depolarization of the subsequent neuron
	while (!qtemp.empty()) {
		Dendrite *d;
		d = qtemp.top().getDendrite();

		if (d->dendriteTo->checkActivation() == 1) {
			//d->synapses += 2; //hebbian learning rule - synaptic plasticity
		} else {
			//activated Neuron didn't fire, so activating neuron (d) is probably representation of a new pattern
			//therefor we create a new input neuron on higher layer and connect it to this neuron if it
			//doesn't exist already
			//only add output if neuron is not an input neuron
			//only if activated Neuron had not been inhibited before
			if (!d->dendriteFrom->isOutputNeuron() && d->dendriteFrom->type == 1 && !d->dendriteTo->isInhibited()) {
				d->dendriteFrom->newOutput();
			}

		}
        d->synapses += 1; //hebbian learning rule - synaptic plasticity
		qtemp.pop();
	}
}

bool ActivationQueue::isEmpty(void) {
    return this->q.empty();
}
