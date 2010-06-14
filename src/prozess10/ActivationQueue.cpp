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
}

void ActivationQueue::schedActivation (Dendrite* dendrite, int aDelay, double aVal) {
		this->q.push(Aqueue(aDelay+this->layer->step,dendrite,aVal));
}

void ActivationQueue::activate (void) {
	this->layer->newStep();
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
		}
        d->synapses += 1; //hebbian learning rule - synaptic plasticity
		qtemp.pop();
	}
}

bool ActivationQueue::isEmpty(void) {
    return this->q.empty();
}
