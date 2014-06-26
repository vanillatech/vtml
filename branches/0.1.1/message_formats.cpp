#include "stdafx.h"
#include <stdio.h>
#include "callbacks.h"

namespace Odin
{

static const int BUF_SIZE = 1024;

std::string CallbackMsg<MSG_STEP_COUNTER>::message() const
{
	char b[BUF_SIZE];
	//sprintf_s(b, BUF_SIZE, "------- step %d (layer #%d) -------", step, layer());
	sprintf_s(b, BUF_SIZE, "------- step %d -------", step);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

std::string CallbackMsg<MSG_NEW_INPUT_NEURON>::message() const
{
	char b[BUF_SIZE];
	//sprintf_s(b, BUF_SIZE, "New Input Neuron: layer: %d, id: %s, value: %s", layer(), id.c_str(), value.c_str());
	sprintf_s(b, BUF_SIZE, "New Input Neuron: id: %s, value: %s", id.c_str(), value.c_str());
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*
AnsiString captionID = toNeuron->id;
		  captionID += " layer: ";
		  captionID += toNeuron->layer;
		  toNeuron->debugCallingNode = SDIAppForm->addLink(captionID,this->debugCallingNode);
		  Debug1->refreshTT();

		  Debug1->ListBox1->Items->Insert(0,"NewLink: " + id + " to " + toNeuron->id);
*/

std::string CallbackMsg<MSG_NEW_LINK>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "New Link: %s to %s", fromId.c_str(), toId.c_str());
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*Debug1->ListBox1->Items->Insert
(0,"Activate Neuron: " + id + " (Increase: " 
+ AnsiString (activationValNew)+"; activationVal: " + AnsiString(activationVal) + ") " );
*/

std::string CallbackMsg<MSG_NEURON_ACTIVATE>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "Activate Neuron: %s (Increase: %f; activationVal: %f)",
		id.c_str(), increase, actVal);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*
if (this->outputData != 0) {
			SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
		  }
		  Debug1->ListBox1->Items->Insert(0,"Neuron fired: " + id + " ActVal: " + activationVal );
*/

std::string CallbackMsg<MSG_NEURON_FIRE>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "Neuron fired: %s (ActVal: %f)", id.c_str(), actVal);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*
Debug1->ListBox1->Items->Insert
(0,"Schedule Activation: " + axons[n]->dendriteTo->id + " in: " + AnsiString((*axons[n]).activationDelay) );
*/

std::string CallbackMsg<MSG_ACTIVATION_SCHEDULED>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "Schedule Activation: %s (Delay: %d, Value: %f)", id.c_str(), delay, actValue);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*Debug1->ListBox1->Items->Insert(0,"PredictNext: " + this->id );*/

std::string CallbackMsg<MSG_PREDICT_NEXT>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "PredictNext: %s", id.c_str());
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*
int toutput = int(timeOffset);
	Debug1->ListBox1->Items->Insert(0,"PropagateDown: " + this->id + " timeoffset: " + AnsiString(toutput) );
*/

std::string CallbackMsg<MSG_PROPAGATE_DOWN>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "PropagateDown: %s (timeoffset: %d)",
		id.c_str(), timeOffset);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/* Debug1->ListBox1->Items->Insert(0,"Inhibit: " + id );*/

std::string CallbackMsg<MSG_INHIBIT>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "Inhibit: %s", id.c_str());
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[forderNew][n]->id);*/

std::string CallbackMsg<MSG_DELETE_FROM_RQ>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "Delete from Rq: %s", id.c_str());
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

std::string CallbackMsg<MSG_NEW_OUTPUT>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "New Output from Neuron: %s to Neuron: %s", fromId.c_str(), toId.c_str());
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

std::string CallbackMsg<MSG_NEW_OUTPUTTEXT>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "%s", value.c_str());
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

} //Odin
