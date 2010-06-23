//#include "stdafx.h"
#include "globals.h"
#include "ActivationQueue.h"
#include "Dendrite.h"

Dendrite::Dendrite (Layer *nLayer){
	this->layer = nLayer;
	this->weight=0.6;
	this->lastUsed = 0;
	this->synapses = 1;
	this->weightFrozen = false;
	this->activationDelay = 1;
};

Dendrite::Dendrite(Layer *nLayer, int strength) {
	this->layer = nLayer;
	this->lastUsed = 0;
	this->weight=1;
	this->synapses = 1;
	this->weightFrozen = false;
	this->activationDelay = 1;
}

void Dendrite::changeWeights (void) {
		//if (!this->weightFrozen) {
			

		//check if dendrite was last used not more than activationdelay ago
		if (this->lastUsed + this->activationDelay >= this->layer->step) {
			// then: learn
			//this algorithm is taken from James A. Anderson, 'Introduction to Neural Networks' (MIT Press)
			//related to Kohonen's SOFM.
			this->weight = this->weight * (1 - globals.learnRate )+    globals.learnRate;
		} else {
			//don't learn: this dendrite had not been used to activate the neuron
			//so this dendrite does not belong to the recognized pattern
			// so forget it
			this->weight = this->weight * (1 - globals.learnRate );
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
	this->layer->aqueue->schedActivation(this, aDelay, tWeight);
	this->lastUsed = this->layer->step;
	this->synapses++;
}

float Dendrite::getWeight (void) {
	return this->weight;
};