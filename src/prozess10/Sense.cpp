//#include "stdafx.h"
#include <stdlib.h>
#include "../callbacks.h"
#include "Neuron.h"
#include "Dendrite.h"
#include "Layer.h"
#include "ActivationQueue.h"
#include "Sense.h"	

using namespace Odin;
extern ICallback* callback;

Sense::Sense(Layer* nLayer) {
	this->layer = nLayer;
}

void Sense::init()
{
	for ( int  i = 0 ; i < 256 ; i++ ) 
	{
		 this->inputNeurons[i] = new  Neuron (this->layer,0);
		 this->outputNeurons[i] = new  Neuron (this->layer,2);

		 this->inputNeurons[i]->dendrites.push_back(new Dendrite(this->layer,1));
		 this->inputNeurons[i]->dendrites[0]->dendriteTo = this->inputNeurons[i];
		 this->inputNeurons[i]->dendrites[0]->dendriteFrom = this->outputNeurons[i];
		 this->inputNeurons[i]->dendrites[0]->synapses = 1;
		 this->inputNeurons[i]->dendrites[0]->weightFrozen = true;
		 /*this->inputNeurons[i]->axons.push_back(new Dendrite());
		 this->inputNeurons[i]->axons[0]->dendriteTo = this->outputNeurons[i];
		 this->inputNeurons[i]->axons[0]->dendriteFrom = this->inputNeurons[i];
		 this->inputNeurons[i]->axons[0]->synapses = 1;*/
		 /*this->outputNeurons[i]->axons.resize(1);
		 this->outputNeurons[i]->axons[0].push_back(new Dendrite());
		 this->outputNeurons[i]->axons[0][0]->dendriteFrom = this->outputNeurons[i];
		 this->outputNeurons[i]->axons[0][0]->dendriteTo = this->inputNeurons[i];
		 this->outputNeurons[i]->axons[0][0]->synapses = 1;
		 this->outputNeurons[i]->axons[0][0]->self=0;*/
		 this->outputNeurons[i]->dendrites.push_back(new Dendrite(this->layer));
		 this->outputNeurons[i]->dendrites[0]->dendriteTo = this->outputNeurons[i];
		 this->outputNeurons[i]->dendrites[0]->synapses = 1;
		 this->outputNeurons[i]->dendrites[0]->weightFrozen = true;
		 this->outputNeurons[i]->dendrites[0]->activationDelay = 0;

		 //this->outputNeurons[i]->dendrites[1]->inhibitory = true;


#ifdef BORLAND_GUI
			 AnsiString captionID = char(i);
			 outputNeurons[i]->outputData = char(i);
			 captionID +=  " (" +  inputNeurons[i]->id + ")";
			 inputNeurons[i]->debugCallingNode = SDIAppForm->addLink(captionID);

#else
			 std::string captionID;
			 //char buf[4];
			 //_itoa_s(i, buf, 4, 10);
			 //captionID += buf;
			 captionID += (char)i;
			 callback->onCallback(new CallbackMsg<MSG_NEW_INPUT_NEURON>(layer->number, inputNeurons[i]->id, captionID));
			 callback->onCallback(new CallbackMsg<MSG_NEW_LINK>(layer->number, inputNeurons[i]->id, layer->number, outputNeurons[i]->id));

#endif
			//*inputNeurons = new Neuron (queue);
	}


}

void Sense::input (int c) {
	this->outputNeurons[c]->lastfired = this->layer->step;
	//this->aqueue->schedActivation( this->inputNeurons[c]->dendrites[0],0);
	this->inputNeurons[c]->dendrites[0]->stimulate();
	//this->inputNeurons[c]->checkActivation();
}

void Sense::nextStep (void) {
     this->layer->newStep();
}