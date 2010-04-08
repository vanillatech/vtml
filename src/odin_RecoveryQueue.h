

//**********
//Member functions RecoveryQueue
//**********

RecoveryQueue::RecoveryQueue (int nLayer) {
		//this->queue.resize(queueMax);
		this->counter = 0;
		this->lastelement = 0;
		this->stepLastElement = 0;
		this->layer = nLayer;
		this->lastInsertedElement = 0;
}

int RecoveryQueue::deletePattern(Neuron *neuron, unsigned int pos, unsigned int forderOld) {
   int forderNew = 0;
   int retVal = 0;
   if (pos == neuron->dendrites.size()) {return(1);}
   if (pos > 0) {
	   int delay = neuron->dendrites[pos]->activationDelay - neuron->dendrites[pos - 1]->activationDelay;
	   forderNew = forderOld - delay;
	   if (forderNew < 0) forderNew+=queueMax;
	   for (int n = 0;n<queueMax && !queue[forderNew].empty();n++) {
		if (neuron->dendrites[pos]->dendriteFrom == queue[forderNew][n]) {
		  retVal = deletePattern(neuron,pos+1,forderNew);
		  if (retVal == 1) {
			Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[forderNew][n]->id);
			queue[forderNew].erase(queue[forderNew].begin()+n);
			return(1);
		  }

		}
	   }
   } else {
	for (int m=0;m<queueMax ;m++ ) {
	for (unsigned int n=0;n < queue[m].size() ;n++ ) {
			if (neuron->dendrites[pos]->dendriteFrom == queue[m][n] ) {
				retVal = deletePattern(neuron,pos+1,m);
				if (retVal == 1) {
                   Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[m][n]->id);
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
		if (recQueueIdle >= this->layer) recQueueIdle = this->layer - 1;

		//insert actual neuron
		this->queue[this->counter].push_back(neuron);
		this->lastInsertedElement = stepCounter;
		//new type=2 neuron
		if (this->layer > 1) {
            
			Neuron *dNeuron = new Neuron(&aqueue,&recQueue,this->layer,2);
			neuron->newLink(dNeuron,0,1);
			this->lastType2Neuron = dNeuron;
		}
		//link lastType2Neuron of uppler layer to this element
		/* -- segmentation fault
		if (recQueue[this->layer+1].lastType2Neuron != 0) {
			recQueue[this->layer+1].lastType2Neuron->newLink(neuron,0,1);
		}*/

}

void RecoveryQueue::checkNewPattern() {
	//Associate everything in recQueue with a new neuron
		//old: only check for new pattern if nothing has been fired on all of the layers below current layer (0.0.61)
		/*if (this->queue[this->lastelement].empty() && recQueueIdle == (this->layer - 1)) {
			recQueueIdle = this->layer;
		} */
		//new: if recovery queue is 'full' and on the upper layer has nothing been fired
		//means - if no pattern has been recognized
		//then insert a new neuron in our model representing the current queue as a pattern
		if (this->noChangeInCycle() && this->countItems() > 1) {
			
	
		//if ( recQueueIdle == this->layer && this->countItems() > 1) { //experimental
		//if ( this->countItems() > 1) { //experimental
			Neuron *newNeuron = new Neuron(&aqueue,&recQueue,this->layer + 1,0);
			int c = this->counter - 1;
			for (int n=0;n<queueMax ;n++ ) {

				if (c<0) { c = queueMax - 1; }
				unsigned int countTotal = queue[c].size();
				if (countTotal > 1) {
				Neuron *newNeuron2 = new Neuron(&aqueue,&recQueue,this->layer + 1,1);
				for (unsigned int m=0;m < countTotal ;m++ ) {
						queue[c][m]->newLink(newNeuron2,0,countTotal);
				}
				newNeuron2->newLink(newNeuron,n,countTotal);
				} else if (countTotal == 1) {
					queue[c][0]->newLink(newNeuron,n,countTotal);
				}
				c--;
			}
			//deletePattern(newNeuron,0,0);
			newNeuron->fire();
			//lastFiredNeuron = newNeuron;
		}

		if (recQueue.capacity() < recQueue.size() + 3) {
			recQueue.reserve(recQueue.size()+3);
		}
}

void RecoveryQueue::recover(void) {
    if (stepLastElement != stepCounter) {


		this->lastelement = this->counter;
		this->counter++;
		if (this->counter>=queueMax) {  this->counter = 0; }
		this->queue[this->counter].clear();
		this->checkNewPattern();
		stepLastElement = stepCounter;
	}
}

bool RecoveryQueue::noChangeInCycle() {
	if (recQueue[this->layer+1].lastInsertedElement <=stepCounter - queueMax && (recQueue[this->layer+1].lastInsertedElement != 0 || stepCounter > queueMax)) {
		return(true);
	}
	return(false);
}

int RecoveryQueue::countItems() {
	int cItems = 0;
	for (int n=0;n<queueMax ;n++ ) {
		cItems += queue[n].size();
	}
	return (cItems);
}
