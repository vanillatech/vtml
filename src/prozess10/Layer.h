#pragma once

class ActivationQueue;
class RecoveryQueue;

class Layer {
	public:
	Layer(void);
	Layer(Layer *);
	void newStep (void);
	Layer *getHigher(void);
	Layer *lower;
	Layer *higher;
	unsigned int step;
	ActivationQueue *aqueue;
	RecoveryQueue *recQueue;
};
