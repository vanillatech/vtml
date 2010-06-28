//#include "stdafx.h"
#include "ActivationQueue.h"
#include "RecoveryQueue.h"
#include "../callbacks.h"
#include "Layer.h"

using namespace Odin;

extern ICallback* callback;

Layer::Layer(Layer* caller) :
	number(caller ? (caller->number + 1) : 1),
	step(1),
	lower(caller),
	higher(0),
	aqueue(0),
	recQueue(0),
	idle(true)
{
	this->aqueue = new ActivationQueue(this);
	this->recQueue = new RecoveryQueue(this);
}

Layer::~Layer()
{
	if (higher) {
		delete higher;
	}
	delete recQueue;
	delete aqueue;
}

void Layer::newStep(void) {
	this->setIdle(true);
	if (this->higher != 0) {
		if (!this->higher->idle) {
			this->higher->newStep();
		}
	}
	this->step++;
	this->recQueue->recover();

#ifdef BORLAND_GUI
	Debug1->ListBox1->Items->Insert(0,AnsiString("----------- ") + AnsiString(this->step) + AnsiString("------------"));
#else
	callback->onCallback(new CallbackMsg<MSG_STEP_COUNTER>(number, step));
#endif
	this->aqueue->activate();
}

void Layer::setIdle(bool val) {
	this->idle = val;
}

Layer *Layer::getHigher(void) {
	if (this->higher == 0) {
		this->higher = new Layer(this);
	}
	return this->higher;
}
