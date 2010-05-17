#include <stdio.h>
#include "odin.h"

namespace Odin
{

static const int BUF_SIZE = 1024;

std::string CallbackMsg<MSG_STEP_COUNTER>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "------- step %d (layer #%d) -------", step, layer());
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
	sprintf_s(b, BUF_SIZE, "NewLink: %d to %d", fromId, toId);
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
	sprintf_s(b, BUF_SIZE, "Activate Neuron: %d (Increase: %f; activationVal: %f)",
		id, increase, actVal);
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
	sprintf_s(b, BUF_SIZE, "Neuron fired: %d (ActVal: %f)", id, actVal);
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
	sprintf_s(b, BUF_SIZE, "Schedule Activation: %d (Delay: %d)", id, delay);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*Debug1->ListBox1->Items->Insert(0,"PredictNext: " + this->id );*/

std::string CallbackMsg<MSG_PREDICT_NEXT>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "PredictNext: %d", id);
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
	sprintf_s(b, BUF_SIZE, "PropagateDown: %d (timeoffset: %d)",
		id, timeOffset);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/* Debug1->ListBox1->Items->Insert(0,"Inhibit: " + id );*/

std::string CallbackMsg<MSG_INHIBIT>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "Inhibit: %d", id);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

/*Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[forderNew][n]->id);*/

std::string CallbackMsg<MSG_DELETE_FROM_RQ>::message() const
{
	char b[BUF_SIZE];
	sprintf_s(b, BUF_SIZE, "Delete from Rq: %d", id);
	b[BUF_SIZE-1]=0;
	return std::string(b);
}

} //Odin
