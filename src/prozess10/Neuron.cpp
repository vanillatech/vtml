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
		  this->threshold = 0.4;
		  this->outputLink = 0;
}

Neuron::~Neuron()
{
	for (size_t i=0; i<dendrites.size(); ++i) {
		delete dendrites[i];
	}
}

Dendrite* Neuron::newLink (Neuron *toNeuron) {
		return this->newLink(toNeuron,1,0,globals.defaultWeight);
}

Dendrite* Neuron::newLink (Neuron *toNeuron, float nWeight) {
		return this->newLink(toNeuron,1,0,nWeight);
}

Dendrite* Neuron::newLink (Neuron *toNeuron, int ndelay, int countTotal, float nWeight) {

		  int g = axons.size();
		  Dendrite *tempDend = new Dendrite(this->layer, nWeight);
		  axons.push_back(tempDend);
		  axons[g]->dendriteFrom = this;
		  axons[g]->dendriteTo = toNeuron;
		  axons[g]->synapses = 1;

		  axons[g]->activationDelay = ndelay;

		  toNeuron->dendrites.push_back(axons[g]);

#ifdef BORLAND_GUI
		  AnsiString captionID = toNeuron->id;
		  captionID += " layer: ";
		  //captionID += toNeuron->layer;
		  toNeuron->debugCallingNode = SDIAppForm->addLink(captionID,this->debugCallingNode);
		  Debug1->refreshTT();

		  Debug1->ListBox1->Items->Insert(0,"NewLink: " + id + " to " + toNeuron->id);
#else
		  std::string captionID = toNeuron->id;
		  callback->onCallback(
			  new CallbackMsg<MSG_NEW_LINK>(
					getLayer()->number, id, toNeuron->getLayer()->number, toNeuron->id
			  )
		  );	

#endif
		  
		  return axons[g];
}

Dendrite* Neuron::newOutputLink (Neuron *toNeuron) {

		  Dendrite *tempDend = new Dendrite(this->layer, globals.defaultWeight);
		  tempDend->dendriteFrom = this;
		  tempDend->dendriteTo = toNeuron;
		  tempDend->synapses = 1;
		  tempDend->activationDelay = 1;


#ifdef BORLAND_GUI
		  AnsiString captionID = toNeuron->id;
		  captionID += " layer: ";
		  //captionID += toNeuron->layer;
		  toNeuron->debugCallingNode = SDIAppForm->addLink(captionID,this->debugCallingNode);
		  Debug1->refreshTT();

		  Debug1->ListBox1->Items->Insert(0,"NewLink: " + id + " to " + toNeuron->id);
#else
		  std::string captionID = toNeuron->id;
		  callback->onCallback(
			  new CallbackMsg<MSG_NEW_LINK>(
					getLayer()->number, id, toNeuron->getLayer()->number, toNeuron->id
			  )
		  );	

#endif
		  return (tempDend);
}

Dendrite *Neuron::getOutputLink () {
	if (this->outputLink != 0) {
		return this->outputLink;
	} else {
		//return(0);
		//check if inputneuron
		//if (this->type == 0) return (0);
		/*for (unsigned int c=0;c < this->dendrites.size();c++) {
			if (this->dendrites[c]->dendriteFrom != 0 && this->type == 1) {
				Dendrite *nTmp = this->dendrites[c]->dendriteFrom->getOutputLink();
				if (nTmp != 0) {
					this->outputLink = nTmp;
					return nTmp;
				}
			} 
		}*/
	    return this->setNewOutputLink ();
	}
}

void Neuron::activateOutputLink () {
	if (this->outputLink != 0) {
		this->outputLink->stimulate(1.0f);
	}
}

Dendrite *Neuron::setNewOutputLink () {
	this->outputLink = this->newOutput();
	return (this->outputLink);
}

int Neuron::countSynapses () {
		  int countSyn = 0;
		  for (unsigned int n=0;n<dendrites.size();n++) {
			countSyn += (*dendrites[n]).synapses;
		  }
		  return (countSyn);
}

int Neuron::countSynapsesOnAxons () {
		  int countSyn = 0;
		  for (unsigned int n=0;n<axons.size();n++) {
			countSyn += this->axons[n]->synapses;
		  }
		  return (countSyn);
}

int Neuron::countMaxSynapsesOnAxons () {
		  int countSyn = 0;
		  for (unsigned int n=0;n<axons.size();n++) {
			if (this->axons[n]->synapses > countSyn)
				countSyn = this->axons[n]->synapses;
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
	callback->onCallback(new CallbackMsg<MSG_NEURON_ACTIVATE>(getLayer()->number, id, (float)activationVal, (float)activationValNew));
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

void Neuron::activateSuccessors(void) {
		  float totalWeight = this->getAxonsWeight();
		  
		  //activate parent neurons
		  for (unsigned int n=0;n<axons.size();n++ ) {
					float weightToStimulate = 0.0f;
					
					float totalDWeight = axons[n]->dendriteTo->getDendritesWeight();
					float aWeight = axons[n]->getWeight();
					if (axons[n]->dendriteTo->type==0) {
						//if subsequent neuron is an input neuron we'll activate this with 1
						weightToStimulate = 1.0f;
						axons[n]->stimulate(weightToStimulate);
					} else {
						//otherwise it is within a layer then we'll distribute the activation among the 
						//subsequent neurons
						//wFactor: 'oldest'/strongest Dendrite will get most attention
						float wFactor = float(axons[n]->synapses) / this->countMaxSynapsesOnAxons();
						weightToStimulate = aWeight * aWeight / totalWeight * wFactor;
						
						axons[n]->stimulate(weightToStimulate);
					}
					
		  }

}

void Neuron::connectFromLastStep(void) {

	//look what neurons were fired in current layer in last step and connect them to this neuron
		  if (this->layer->recQueue->setFocusStep(-1) != -1) {
			Neuron *n;
			while ((n = this->layer->recQueue->getNext()) != 0) {
				if (this->type == 1) {
					if (!this->containsDendrite(n)) {
						n->newLink(this,0.1f);
					}
				}
			}
		  }

}

void Neuron::fire (void) {

		callback->onCallback(new CallbackMsg<MSG_NEURON_FIRE>(getLayer()->number, id, (float)activationVal));

		  this->activationVal = 0;
		  this->lastfired = this->layer->step;
          
		  for (unsigned int n=0;n<dendrites.size();n++) {
				if (dendrites[n] != 0) {
                //learn
					dendrites[n]->changeWeights();
				}
		  }
		 
		  this->activateSuccessors();
		  
		  this->layer->recQueue->insert(this);
		  
		  if (this->layer->number != 0) {
			  this->predictNext();
			  //this->propagateDown(0);
		  } 
		  this->connectFromLastStep();
		  globals.lastFiredNeuron = this;

}

float Neuron::getAxonsWeight (void) {
	float totalWeight = 0;
	for (unsigned int n=0;n<axons.size();n++ ) {
		totalWeight += axons[n]->getWeight();
	}
	return totalWeight;
}

float Neuron::getDendritesWeight (void) {
	float totalWeight = 0;
	for (unsigned int n=0;n<dendrites.size();n++ ) {
		totalWeight += dendrites[n]->getWeight();
	}
	return totalWeight;
}

Dendrite *Neuron::getStrongestAxon(void) {
//0.0.50: select strongest axon
	Dendrite *strongestAxon = NULL;
	for (unsigned int g=0;g<this->axons.size() ;g++ ) {
		if (g==0 && this->axons[0] != 0) strongestAxon = this->axons[0];
		if (this->axons[g] != 0) {

			//a is actually wrong! will only cause to count the lastfired axon to be used for calculation.
			double a, b;
			if (strongestAxon->dendriteTo->countSynapses() > 0) {
				a = strongestAxon->synapses / double(strongestAxon->dendriteTo->countSynapses());
			} else a = 0;
			if (this->axons[g]->dendriteTo->countSynapses() > 0) {
				b = this->axons[g]->synapses / double(this->axons[g]->dendriteTo->countSynapses());
			} else b = 0;
			if (a < b) {
				return this->axons[g];
			}
		}
	}
	//--0.0.50
	return strongestAxon;
}

void Neuron::predictNext(void) {


	//callback->onCallback(new CallbackMsg<MSG_PREDICT_NEXT>(getLayer()->number, id));

	Dendrite *strongestAxon = this->getStrongestAxon();
	if (strongestAxon) {
		if (strongestAxon->dendriteTo != 0) {
			callback->onCallback(new CallbackMsg<MSG_PREDICT_NEXT>(strongestAxon->dendriteTo->getLayer()->number, strongestAxon->dendriteTo->id));
			for (unsigned int n=0;n<strongestAxon->dendriteTo->dendrites.size() ;n++ ) {
				if (strongestAxon->dendriteTo->dendrites[n]->dendriteFrom != 0) {
					if (strongestAxon != strongestAxon->dendriteTo->dendrites[n]) {
						Neuron* toNeuron = strongestAxon->dendriteTo->dendrites[n]->dendriteFrom;
						
						if (toNeuron->type == 0) {
							toNeuron->propagateDown(0);
							
						}
					}
				}
				

			}
		}
	}
}

void Neuron::propagateDown(int timeOffset) {
	//int toutput = int(timeOffset);


	callback->onCallback(new CallbackMsg<MSG_PROPAGATE_DOWN>(getLayer()->number, id, timeOffset));
	if (this->getLayer()->number == 1) {
		if (this->type == 0) {
			this->fire();
		}
	}
	
	if (this->outputData != 0) {
		//form->OutputText(this->outputData);
		//textBox2->Text = this->ouputData;
		
		std::string buf = "";
		buf += this->outputData;
		callback->onCallback(new CallbackMsg<MSG_NEW_OUTPUTTEXT>(buf));
	}
			//this->dendrites[0]->dendriteFrom->getLayer()->aqueue->schedActivation(dendrites[0], timeOffset);
			//SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
			//this->fire();
	//}
	//int lastDelay = 0;
	for (int m=this->dendrites.size() - 1 ; m>0 ; m--) {

		if (this->dendrites[m]->dendriteFrom != 0) {

			/*if (lastDelay != this->dendrites[m]->activationDelay) {
				timeOffset += (this->dendrites[m]->activationDelay - lastDelay);
			}*/
			this->dendrites[m]->dendriteFrom->getLayer()->aqueue->schedActivation(dendrites[m], timeOffset);
			//this->dendrites[m]->dendriteFrom->propagateDown(timeOffset);
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
	callback->onCallback(new CallbackMsg<MSG_INHIBIT>(getLayer()->number, id));
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

Dendrite *Neuron::newOutput (void) {

	//create new Neuron type=input
	Neuron *newNeuron = new Neuron(this->getLayer()->getHigher(), 0);

#ifdef BORLAND_GUI
	
#else

	callback->onCallback(new CallbackMsg<MSG_NEW_OUTPUT>(
			getLayer()->number, this->id, newNeuron->getLayer()->number, newNeuron->id
		)
	);

	callback->onCallback(new CallbackMsg<MSG_NEW_INPUT_NEURON>(
			newNeuron->getLayer()->number, newNeuron->id, ""
		)
	);

#endif

	//link new Neuron to current Neuron's axon
	Dendrite *tDend = this->newOutputLink(newNeuron);
	return (tDend);
}

Layer *Neuron::getLayer (void) {
	return this->layer;
}

bool Neuron::newNeighbor (Neuron *n2) {
	if (this->isNeighborOf(n2)) return false;
	this->neighbors.push_back(n2);
	n2->newNeighbor(this);
	return true;
};

bool Neuron::isNeighborOf (Neuron *n2) {
	for (unsigned int n=0; n<this->neighbors.size();n++) {
		if (this->neighbors[n] == n2)
			return true;
	}
	return false;
};

bool Neuron::hasNeighbors () {
	if (this->neighbors.size() == 0) return false;
	else return true;
};