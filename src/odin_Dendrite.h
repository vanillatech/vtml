Dendrite::Dendrite (void){
	this->aqueuelocal = &aqueue;
	this->weight=0.5;
	this->lastUsed = 0;
	this->synapses = 1;
};

Dendrite::Dendrite(int strength) {
	this->aqueuelocal = &aqueue;
	this->lastUsed = 0;
	this->weight=1;
	this->synapses = 1;
}

void Dendrite::changeWeights (void) {
		if (this->lastUsed + queueMax >= stepCounter) {
			//if last usage of dendrite was within recoveryTime
			// then: learn
			this->weight = this->weight * (1 - learnRate )+    (1 - learnRate);
		} else {
			//don't learn: this dendrite had not been used to activate the neuron
			//so this dendrite does not belong to the recognized pattern
			// so forget it
			this->weight = this->weight * (1 - learnRate );
        }
};

void Dendrite::stimulate (void) {
		this->aqueuelocal->schedActivation(this, this->activationDelay, this->weight);
		this->lastUsed = stepCounter;
};
