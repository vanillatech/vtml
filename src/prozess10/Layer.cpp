//#include "stdafx.h"
#include "ActivationQueue.h"
#include "RecoveryQueue.h"
#include "../callbacks.h"
#include "Layer.h"

using namespace Odin;

extern ICallback* callback;

Layer::Layer(void) {
		this->step = 1;
		this->aqueue = new ActivationQueue(this);
		this->recQueue = new RecoveryQueue(this);
}

Layer::Layer(Layer *caller) {
		this->step = 1;
		this->aqueue = new ActivationQueue(this);
		this->recQueue = new RecoveryQueue(this);
		this->lower = caller;
}

void Layer::newStep(void) {
	this->step++;
	this->recQueue->recover();

#ifdef BORLAND_GUI
	Debug1->ListBox1->Items->Insert(0,AnsiString("----------- ") + AnsiString(this->step) + AnsiString("------------"));
#else
	callback->onCallback(new CallbackMsg<MSG_STEP_COUNTER>(0, step));
#endif
}
