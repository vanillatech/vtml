//#include "stdafx.h"
#include "globals.h"
#include "ActivationQueue.h"
#include "Neuron.h"
#include "Dendrite.h"

Dendrite::Dendrite (Layer *nLayer){
	Dendrite(nLayer,globals.defaultWeight);
};


Dendrite::Dendrite(Layer *nLayer, float nWeight) {
	this->layer = nLayer;
	this->lastUsed = 0;
	this->weight=nWeight;
	this->eSynapses = 1;
	this->iSynapses = 0;
	this->weightFrozen = false;
	this->activationDelay = 1;
};

void Dendrite::changeWeights (void) {
		//if (!this->weightFrozen) {
		//check if dendrite was last used not more than activationdelay ago
		if (this->lastUsed + this->activationDelay >= this->layer->step) {
			// then: learn
			//this algorithm is taken from James A. Anderson, 'Introduction to Neural Networks' (MIT Press)
			//related to Kohonen's SOFM.
			//this->weight = this->weight - (this->weight - 1) *  globals.learnRate / this->synapses;
			this->eSynapses++;
		} else {
			//don't learn: this dendrite had not been used to activate the neuron
			//so this dendrite does not belong to the recognized pattern
			// so forget it
			//this->weight = this->weight * (1 - globals.learnRate / this->synapses  );
			this->iSynapses++;
		}
		//}
};

void Dendrite::stimulate (void) {
	this->stimulate(this->weight, this->activationDelay);
};

void Dendrite::stimulate (int aDelay) {
	this->stimulate(this->weight,aDelay);
};

void Dendrite::stimulate (float tWeight) {
	this->stimulate(tWeight, this->activationDelay);	
}

void Dendrite::stimulate(float tWeight, int aDelay) {
	this->dendriteTo->getLayer()->aqueue->schedActivation(this, aDelay, tWeight);
	this->lastUsed = this->layer->step;
	//this->synapses++;
}

float Dendrite::getWeight (void) {
	//return this->weight;
	return (this->weight * (this->eSynapses - this->iSynapses) )/ ((float)this->eSynapses + this->iSynapses);
};