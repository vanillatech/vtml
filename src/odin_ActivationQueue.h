//**********
//Member functions ActivationQueue
//**********

void ActivationQueue::schedActivation (Dendrite* dendrite, int aDelay) {
		this->q.push(Aqueue(aDelay+stepCounter,dendrite,1));
}

void ActivationQueue::schedActivation (Dendrite* dendrite, int aDelay, double aVal) {
		this->q.push(Aqueue(aDelay+stepCounter,dendrite,aVal));
}

void ActivationQueue::activate (void) {
	stepCounter++;
	for (unsigned int n=0;n<recQueue.size();n++ ) {
				recQueue[n].recover();
	}
	Debug1->ListBox1->Items->Insert(0,AnsiString("----------- ") + AnsiString(stepCounter) + AnsiString("------------"));
	priority_queue <Aqueue> qtemp;
	//first activate subsequent neurons
	while ((!this->q.empty()) && this->q.top().getPos() <= stepCounter) {
		Dendrite *d;
		d = this->q.top().getDendrite();
		double actVal = 0;
		/*int cs = d->dendriteTo->countSynapses();
		if (cs > 0) actVal = d->synapses / double(d->dendriteTo->countSynapses());
		else actVal = 0;*/
		actVal = this->q.top().getAct();
		//actVal = 1;
		if (actVal > minActivation) {
		if (d->dendriteTo->lastfired < (stepCounter - recoveryTime)) {
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
