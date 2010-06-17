//#include "stdafx.h"
#include "globals.h"
#include "../callbacks.h"
#include "RecoveryQueue.h"
#include "ActivationQueue.h"
#include "Layer.h"
#include "Neuron.h"
#include "Dendrite.h"

using namespace Odin;

extern ICallback* callback;

Neuron::Neuron (Layer *nLayer, int neuronType) {
		  this->type = neuronType;
		  this->layer = nLayer;
		  
		  this->activationVal = 0;
		  this->lastchecked = this->layer->step;
		  this->lastfired = 0;
		  char buf[8];
		  _itoa_s(globals.neuronCounter++, buf, 7, 10);
		  this->id = buf;
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

#ifdef BORLAND_GUI
		  AnsiString captionID = toNeuron->id;
		  captionID += " layer: ";
		  //captionID += toNeuron->layer;
		  toNeuron->debugCallingNode = SDIAppForm->addLink(captionID,this->debugCallingNode);
		  Debug1->refreshTT();

		  Debug1->ListBox1->Items->Insert(0,"NewLink: " + id + " to " + toNeuron->id);
#else
		  std::string captionID = toNeuron->id;
		  callback->onCallback(new CallbackMsg<MSG_NEW_LINK>(0, id, 0, toNeuron->id));	

#endif
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
  if (this->lastfired < this->layer->step - globals.recoveryTime && this->blockActivation < this->layer->step) {
	this->activationVal += activationValNew;

#ifdef BORLAND_GUI
	Debug1->ListBox1->Items->Insert(0,"Activate Neuron: " + id + " (Increase: " + AnsiString (activationValNew)+"; activationVal: " + AnsiString(activationVal) + ") " );
#else
	callback->onCallback(new CallbackMsg<MSG_NEURON_ACTIVATE>(0, id, (float)activationVal, (float)activationValNew));
#endif

  };
}

void Neuron::drainActivation(void) {
  for (unsigned int n=0;n<50 && n<(this->layer->step-lastchecked)&&lastchecked>0 ;n++ ) {
			this->activationVal *= globals.drain;
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

#ifdef BORLAND_GUI
          if (this->outputData != 0) {
			SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
		  }
		  Debug1->ListBox1->Items->Insert(0,"Neuron fired: " + id + " ActVal: " + activationVal );
#else
		callback->onCallback(new CallbackMsg<MSG_NEURON_FIRE>(0, id, (float)activationVal));
#endif

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
		  float totalWeight = this->getAxonsWeight();
		  //activate parent neurons
		  for (unsigned int n=0;n<axons.size();n++ ) {

#ifdef BORLAND_GUI
					Debug1->ListBox1->Items->Insert(0,"Schedule Activation: " + axons[n]->dendriteTo->id + " in: " + AnsiString((*axons[n]).activationDelay) );
#else
			callback->onCallback(new CallbackMsg<MSG_ACTIVATION_SCHEDULED>(0, id, axons[n]->activationDelay));
#endif
					float aWeight = axons[n]->getWeight();
					if (axons[n]->dendriteTo->type==0) {
						//if subsequent neuron is an input neuron we'll activate this with 1
						axons[n]->stimulate(float(1.0));
					} else {
						//otherwise it is within a layer then we'll distribute the activation among the 
						//subsequent neurons
						axons[n]->stimulate(aWeight * aWeight / totalWeight);
					}
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
		  globals.lastFiredNeuron = this;

		  //} *
}

float Neuron::getAxonsWeight (void) {
	float totalWeight = 0;
	for (unsigned int n=0;n<axons.size();n++ ) {
		totalWeight += axons[n]->getWeight();
	}
	return totalWeight;
}

void Neuron::predictNext(void) {

#ifdef BORLAND_GUI
	Debug1->ListBox1->Items->Insert(0,"PredictNext: " + this->id );
#else
	callback->onCallback(new CallbackMsg<MSG_PREDICT_NEXT>(0, id));
#endif

	//0.0.50: select strongest axon
	Dendrite *strongestAxon = NULL;
	for (unsigned int g=0;g<this->axons.size() ;g++ ) {
		if (g==0 && this->axons[0] != 0) strongestAxon = this->axons[0];
		if (this->axons[g] != 0) {

#ifdef BORLAND_GUI
			AnsiString d = this->axons[g]->dendriteFrom->id;
			AnsiString e = this->axons[g]->dendriteTo->id;
#endif
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
							strongestAxon->dendriteTo->dendrites[n]->dendriteFrom->propagateDown(tOffset);
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

void Neuron::propagateDown(int timeOffset) {
	//int toutput = int(timeOffset);

#ifdef BORLAND_GUI
	Debug1->ListBox1->Items->Insert(0,"PropagateDown: " + this->id + " timeoffset: " + AnsiString(toutput) );
#else
	callback->onCallback(new CallbackMsg<MSG_PROPAGATE_DOWN>(0, id, timeOffset));
#endif

	if (this->outputData != 0) {
			this->layer->aqueue->schedActivation(&(*dendrites[0]), timeOffset);
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
	if (this->activationVal >= this->threshold + globals.fireMargin) {
			  this->fire();
			  return(1);
	}
	if (this->lastfired == this->layer->step) {
        return (1);
	}
	return(0);
}

void Neuron::inhibit (void) {
	this->inhibit(true);
}

void Neuron::inhibit (bool recursive) {
		  //inhibits current neuron and all successors with the value that had
		  //been scheduled for activation of the successor before.

#ifdef BORLAND_GUI
		  Debug1->ListBox1->Items->Insert(0,"Inhibit: " + id );
#else
	callback->onCallback(new CallbackMsg<MSG_INHIBIT>(0, id));
#endif

		  this->blockActivation = this->layer->step + globals.blockTime;//-1; //sets recoveryTime
		  this->activationVal = 0;
		  for (unsigned int n=0;n<axons.size() && recursive;n++ ) {
				//double inval = double((*axons[n]).synapses) / (*axons[n]).dendriteTo->countSynapses((*axons[n]).self);
				//(*axons[n]).dendriteTo->activationVal -= inval;
				//(*axons[n]).dendriteTo->blockActivation = this->layer->step + globals.blockTime;
                //(*axons[n]).dendriteTo->activationVal = 0;
				axons[n]->dendriteTo->inhibit(false);
		  }
}

bool Neuron::isInhibited (void) {
	if (this->blockActivation > this->layer->step) return true;
	return false;
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

bool Neuron::isOutputNeuron (void) {
	for (unsigned int n=0;n<axons.size();n++) {
		if (axons[n]->dendriteTo->getLayer() != this->layer) 
			return true;
	}
	return false;
}

void Neuron::newOutput (void) {

	//create new Neuron type=input
	Neuron *newNeuron = new Neuron(this->getLayer()->getHigher(),0);
	#ifdef BORLAND_GUI
	
	#else
	//callback->onCallback(new CallbackMsg<MSG_NEW_OUTPUT>(0, this->id, newNeuron->id));
	#endif

	//link new Neuron to current Neuron's axon
	this->newLink(newNeuron);
}

Layer *Neuron::getLayer (void) {
	return this->layer;
}