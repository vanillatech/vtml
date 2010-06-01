
//**********
//Member functions Dendrite
//**********

Dendrite::Dendrite (Layer *nLayer){
	this->layer = nLayer;
	this->weight=1;
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
			this->weight = this->weight * (1 - learnRate )+    (1 - learnRate);
		} else {
			//don't learn: this dendrite had not been used to activate the neuron
			//so this dendrite does not belong to the recognized pattern
			// so forget it
			this->weight = this->weight * (1 - learnRate );
		}
		//}
};

void Dendrite::stimulate (void) {
		this->layer->aqueue->schedActivation(this, this->activationDelay, this->weight);
		this->lastUsed = this->layer->step;
};

void Dendrite::stimulate (int aDelay) {
		this->layer->aqueue->schedActivation(this, aDelay, this->weight);
		this->lastUsed = this->layer->step;
};
