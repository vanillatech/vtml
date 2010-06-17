//#include "stdafx.h"
#include "ActivationQueue.h"
#include "RecoveryQueue.h"
#include "../callbacks.h"
#include "Layer.h"

using namespace Odin;

extern ICallback* callback;

Layer::Layer(void) {
		this->step = 1;
		this->idle = true;
		this->aqueue = new ActivationQueue(this);
		this->recQueue = new RecoveryQueue(this);
}

Layer::Layer(Layer *caller) {
		this->step = 1;
		this->idle = true;
		this->aqueue = new ActivationQueue(this);
		this->recQueue = new RecoveryQueue(this);
		this->lower = caller;
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
	callback->onCallback(new CallbackMsg<MSG_STEP_COUNTER>(0, step));
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
