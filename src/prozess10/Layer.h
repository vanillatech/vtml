#pragma once

class ActivationQueue;
class RecoveryQueue;

class Layer 
{
public:
	explicit Layer(Layer* caller = 0);
	~Layer();

	void newStep (void);
	void setIdle (bool);
	Layer *getHigher(void);
	
	int number;
	unsigned int step;
	Layer *lower;
	Layer *higher;
	ActivationQueue *aqueue;
	RecoveryQueue *recQueue;
	
private:
	bool idle;
};
