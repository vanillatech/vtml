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
	for ( int  i = 0 ; i < 256 ; i++ ) 
	{
		 this->inputNeurons[i] = 0;
		 this->outputNeurons[i] = 0;
	}
}

void Sense::init()
{
	/*
	for ( int  i = 0 ; i < 256 ; i++ ) 
	{
		 this->inputNeurons[i] = new  Neuron (this->layer,0);
		 this->outputNeurons[i] = new  Neuron (this->layer,2);

		 this->inputNeurons[i]->dendrites.push_back(new Dendrite(this->layer,1));
		 this->inputNeurons[i]->dendrites[0]->dendriteTo = this->inputNeurons[i];
		 this->inputNeurons[i]->dendrites[0]->dendriteFrom = this->outputNeurons[i];
		 this->inputNeurons[i]->dendrites[0]->synapses = 1;
		 this->inputNeurons[i]->dendrites[0]->weightFrozen = true;
		 //this->inputNeurons[i]->axons.push_back(new Dendrite());
		 //this->inputNeurons[i]->axons[0]->dendriteTo = this->outputNeurons[i];
		 //this->inputNeurons[i]->axons[0]->dendriteFrom = this->inputNeurons[i];
		 //this->inputNeurons[i]->axons[0]->synapses = 1;
		 //this->outputNeurons[i]->axons.resize(1);
		 //this->outputNeurons[i]->axons[0].push_back(new Dendrite());
		 //this->outputNeurons[i]->axons[0][0]->dendriteFrom = this->outputNeurons[i];
		 //this->outputNeurons[i]->axons[0][0]->dendriteTo = this->inputNeurons[i];
		 //this->outputNeurons[i]->axons[0][0]->synapses = 1;
		 //this->outputNeurons[i]->axons[0][0]->self=0;
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
	*/

}

void Sense::init(int c)
{
	if (!inputNeurons[c]) {
		inputNeurons[c] = new  Neuron (layer,0);
		
		//svn48: temporarily disable output neurons
		//outputNeurons[c] = new  Neuron (layer,2);

		this->inputNeurons[c]->dendrites.push_back(new Dendrite(this->layer,1));
		 this->inputNeurons[c]->dendrites[0]->dendriteTo = this->inputNeurons[c];

		 //svn48: temporarily disable output neurons
		 this->inputNeurons[c]->dendrites[0]->dendriteFrom = 0;
		 //this->inputNeurons[c]->dendrites[0]->dendriteFrom = this->outputNeurons[c];
		 this->inputNeurons[c]->dendrites[0]->synapses = 1;
		 this->inputNeurons[c]->dendrites[0]->weightFrozen = true;
		 //this->inputNeurons[c]->axons.push_back(new Dendrite());
		 //this->inputNeurons[c]->axons[0]->dendriteTo = this->outputNeurons[c];
		 //this->inputNeurons[c]->axons[0]->dendriteFrom = this->inputNeurons[c];
		 //this->inputNeurons[c]->axons[0]->synapses = 1;
		 //this->outputNeurons[c]->axons.resize(1);
		 //this->outputNeurons[c]->axons[0].push_back(new Dendrite());
		 //this->outputNeurons[c]->axons[0][0]->dendriteFrom = this->outputNeurons[c];
		 //this->outputNeurons[c]->axons[0][0]->dendriteTo = this->inputNeurons[c];
		 //this->outputNeurons[c]->axons[0][0]->synapses = 1;
		 //this->outputNeurons[c]->axons[0][0]->self=0;
		 //svn48: temporarily disable output neurons
		 /*this->outputNeurons[c]->dendrites.push_back(new Dendrite(this->layer));
		 this->outputNeurons[c]->dendrites[0]->dendriteTo = this->outputNeurons[c];
		 this->outputNeurons[c]->dendrites[0]->synapses = 1;
		 this->outputNeurons[c]->dendrites[0]->weightFrozen = true;
		 this->outputNeurons[c]->dendrites[0]->activationDelay = 0;*/

#ifdef BORLAND_GUI
			 AnsiString captionID = char(i);
			 outputNeurons[i]->outputData = char(i);
			 captionID +=  " (" +  inputNeurons[i]->id + ")";
			 inputNeurons[i]->debugCallingNode = SDIAppForm->addLink(captionID);

#else
			 std::string captionID;
			 //char buf[4];
			 //_itoa_s(c, buf, 4, 10);
			 //captionID += buf;
			 captionID += (char)c;
			 callback->onCallback(new CallbackMsg<MSG_NEW_INPUT_NEURON>(layer->number, inputNeurons[c]->id, captionID));
			 //svn48: temporarily disable output neurons
			 //callback->onCallback(new CallbackMsg<MSG_NEW_LINK>(layer->number, inputNeurons[c]->id, layer->number, outputNeurons[c]->id));

#endif
	}
}

void Sense::input (int c) {
	init(c);
	//svn48: temporarily disable output neurons
	//this->outputNeurons[c]->lastfired = this->layer->step;
	//--svn48
	//this->aqueue->schedActivation( this->inputNeurons[c]->dendrites[0],0);
	this->inputNeurons[c]->dendrites[0]->stimulate();
	//this->inputNeurons[c]->checkActivation();
}

void Sense::nextStep (void) {
     this->layer->newStep();
}