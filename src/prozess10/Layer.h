#pragma once

class ActivationQueue;
class RecoveryQueue;

class Layer {
	public:
	Layer(void);
	Layer(Layer *);
	void newStep (void);
	void setIdle (bool);
	Layer *getHigher(void);
	Layer *lower;
	Layer *higher;
	unsigned int step;
	ActivationQueue *aqueue;
	RecoveryQueue *recQueue;
private:
	bool idle;
};
