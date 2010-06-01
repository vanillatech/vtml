

//**********
//Member functions Neuron
//**********


Neuron::Neuron (Layer *nLayer, int neuronType) {
		  this->type = neuronType;
		  this->layer = nLayer;

		  this->activationVal = 0;
		  this->lastchecked = this->layer->step;
		  this->lastfired = 0;
		  this->id = neuronCounter++;
		  this->outputData = 0;
		  this->blockActivation = 0;
		  this->threshold = 0.5;
}

Dendrite* Neuron::newLink (Neuron *toNeuron) {
		return this->newLink(toNeuron,1,0);
}
Dendrite* Neuron::newLink (Neuron *toNeuron, int ndelay, int countTotal) {

		  int g = axons.size();
		  Dendrite *tempDend = new Dendrite(this->layer);
		  axons.push_back(tempDend);
		  (*axons[g]).dendriteFrom = this;
		  (*axons[g]).dendriteTo = toNeuron;
		  (*axons[g]).synapses = 1;

		  (*axons[g]).activationDelay = ndelay;

		  toNeuron->dendrites.push_back(&(*axons[g]));

		  AnsiString captionID = toNeuron->id;
		  captionID += " layer: ";
		  //captionID += toNeuron->layer;
		  toNeuron->debugCallingNode = SDIAppForm->addLink(captionID,this->debugCallingNode);
		  Debug1->refreshTT();

		  Debug1->ListBox1->Items->Insert(0,"NewLink: " + id + " to " + toNeuron->id);
		  return &(*axons[g]);
}

int Neuron::countSynapses () {
		  int countSyn = 0;
		  for (unsigned int n=0;n<dendrites.size();n++) {
			countSyn += (*dendrites[n]).synapses;
		  }
		  return (countSyn);
}

void Neuron::activate(double activationValNew) {
  this->drainActivation();
  if (this->lastfired < this->layer->step - recoveryTime && this->blockActivation < this->layer->step) {
	this->activationVal += activationValNew;
	Debug1->ListBox1->Items->Insert(0,"Activate Neuron: " + id + " (Increase: " + AnsiString (activationValNew)+"; activationVal: " + AnsiString(activationVal) + ") " );
  };
}

void Neuron::drainActivation(void) {
  for (unsigned int n=0;n<50 && n<(this->layer->step-lastchecked)&&lastchecked>0 ;n++ ) {
			this->activationVal *= drain;
  }
  lastchecked = this->layer->step;
}

int Neuron::axonsRemove(Dendrite* dent) {
	for (unsigned int r=0;r<axons.size() ;r++ ) {
		if (axons[r] == dent) {
			axons.erase(axons.begin()+r);
			return 1;
		}
	}
	return 0;
}

void Neuron::fire (void) {
          if (this->outputData != 0) {
			SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
		  }
		  Debug1->ListBox1->Items->Insert(0,"Neuron fired: " + id + " ActVal: " + activationVal );

		  this->activationVal = 0;
		  this->lastfired = this->layer->step;
          // Neuron abtrennen wenn keine Synapsen mehr 0.0.67
		  //if (this->layer > 0) {
		  for (unsigned int n=0;n<dendrites.size();n++) {
				if (dendrites[n] != 0) {
                //learn
				dendrites[n]->changeWeights();
				  if ((*dendrites[n]).dendriteFrom != 0)

					if ((*dendrites[n]).synapses == 0) {
						/* If enabled again please be sure that there is not dendrite
						// in the activation queue anymore. otherwise segfaults.
						int rettest = (*dendrites[n]).dendriteFrom->axonsRemove(dendrites[n]);

						delete dendrites[n];
						dendrites.erase(dendrites.begin()+n);*/
						//(*dendrites[n][m]).dendriteFrom = 0;
						//(*dendrites[n][m]).dendriteTo = 0;


					} else {
					  //WTA: Winner Takes All - inhibit
					  (*dendrites[n]).dendriteFrom->inhibit();
					}
					(*dendrites[n]).synapses--;
				}
		  }
		  //}
		  //activate parent neurons
		  for (unsigned int n=0;n<axons.size();n++ ) {
					Debug1->ListBox1->Items->Insert(0,"Schedule Activation: " + axons[n]->dendriteTo->id + " in: " + AnsiString((*axons[n]).activationDelay) );
					axons[n]->stimulate();
					//this->activationQueue->schedActivation(&(*axons[n]),(*axons[n]).activationDelay);
		  }
		  //only insert into rec.queue when fired twice within recoveryTime
		  //--EXPERIMENTAL--
		  //if (this->lastfired + recoveryTime > stepCounter) {

		  //delete fired pattern from recoveryqueue
		  /* -- Don't do this anymore 0.2.02
		  if (this->layer >= 1) {

					  (*recoveryQueue)[this->layer - 1].deletePattern(this,0,0);

		  } */
		  this->layer->recQueue->insert(this);
		  //this->predictNext();
		  /*if (this->layer > 0) {
			  this->propagateDown(0);
		  } */
		  /*0.0.37
		  if (lastFiredNeuron != 0) {
			if (!this->containsDendrite(lastFiredNeuron)) {
				lastFiredNeuron->newLink(this,1,lastFiredNeuron->dendrites.size(),1);
			}
		  }*/
		  lastFiredNeuron = this;

		  //} *
}

void Neuron::predictNext(void) {
	Debug1->ListBox1->Items->Insert(0,"PredictNext: " + this->id );
	//0.0.50: select strongest axon
	Dendrite *strongestAxon = NULL;
	for (unsigned int g=0;g<this->axons.size() ;g++ ) {
		if (g==0 && this->axons[0] != 0) strongestAxon = this->axons[0];
		if (this->axons[g] != 0) {
			AnsiString d = this->axons[g]->dendriteFrom->id;
			AnsiString e = this->axons[g]->dendriteTo->id;
			//a is actually wrong! will only cause to count the lastfired axon to be used for calculation.
			double a, b;
			if (strongestAxon->dendriteTo->countSynapses() > 0) {
				a = strongestAxon->synapses / double(strongestAxon->dendriteTo->countSynapses());
			} else a = 0;
			if (this->axons[g]->dendriteTo->countSynapses() > 0) {
				b = this->axons[g]->synapses / double(this->axons[g]->dendriteTo->countSynapses());
			} else b = 0;
			if (a < b) {
				strongestAxon = this->axons[g];
			}
		}
	}
	//--0.0.50

	if (strongestAxon) {
		if (strongestAxon->dendriteTo != 0) {

			for (unsigned int n=0;n<strongestAxon->dendriteTo->dendrites.size() ;n++ ) {
				if (strongestAxon->dendriteTo->dendrites[n]->dendriteFrom != 0) {
						int timeOffset = strongestAxon->activationDelay;
						timeOffset -= strongestAxon->dendriteTo->dendrites[n]->activationDelay;
						if (timeOffset > 0) {
							int tOffset = 0;
							strongestAxon->dendriteTo->dendrites[n]->dendriteFrom->propagateDown(&tOffset);
						}
				}
				/*if (this->axons[0]->dendriteTo->dendrites[m][n] == this->axons[0]) {
					if (this->axons[0]->dendriteTo->dendrites[m][n+1]->dendriteFrom != 0) {
						this->axons[0]->dendriteTo->dendrites[n+1]->dendriteFrom->propagateDown();
					}
				}*/

			}
		}
	}
}

void Neuron::propagateDown(int *timeOffset) {
	int toutput = int(timeOffset);
	Debug1->ListBox1->Items->Insert(0,"PropagateDown: " + this->id + " timeoffset: " + AnsiString(toutput) );
	if (this->outputData != 0) {
			this->layer->aqueue->schedActivation(&(*dendrites[0]), int(timeOffset));
			//SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
			//this->fire();
	}
	int lastDelay = 0;
	for (unsigned int m=this->dendrites.size() - 1 ; m>0 ; m--) {

		if (this->dendrites[m]->dendriteFrom != 0) {

			if (lastDelay != this->dendrites[m]->activationDelay) {
				timeOffset += (this->dendrites[m]->activationDelay - lastDelay);
			}
			this->dendrites[m]->dendriteFrom->propagateDown(timeOffset);
		}

	}
}

int Neuron::checkActivation (void) {
	this->drainActivation();
	if (this->activationVal >= this->threshold + fireMargin) {
			  this->fire();
			  return(1);
	}
	if (this->lastfired == this->layer->step) {
        return (1);
	}
	return(0);
}

void Neuron::inhibit (void) {
		  //inhibits current neuron and all successors with the value that had
		  //been scheduled for activation of the successor before.
		  Debug1->ListBox1->Items->Insert(0,"Inhibit: " + id );

		  this->blockActivation = this->layer->step + blockTime;//-1; //sets recoveryTime
		  this->activationVal = 0;
		  for (unsigned int n=0;n<axons.size();n++ ) {
				//double inval = double((*axons[n]).synapses) / (*axons[n]).dendriteTo->countSynapses((*axons[n]).self);
				//(*axons[n]).dendriteTo->activationVal -= inval;
				(*axons[n]).dendriteTo->blockActivation = this->layer->step + blockTime;
                (*axons[n]).dendriteTo->activationVal = 0;
		  }
}

bool Neuron::containsDendrite(Neuron *compareNeuron) {
	for (unsigned int n = 0;n< this->dendrites.size() ;n++ ) {

			if (this->dendrites[n]->dendriteFrom == compareNeuron) {
				return (true);
			}

	}
	return (false);
}

bool Neuron::hasSameSuccessor(Neuron *compareNeuron) {
	for (unsigned int n=0;n<axons.size();n++) {
	  for (unsigned int m=0;m<compareNeuron->axons.size();m++) {
		if (axons[n]->dendriteTo == compareNeuron->axons[m]->dendriteTo) {
			return true;
		}
	  }
	}
	return false;
}
