#pragma once
#include <vector>

using namespace std;

class Neuron;

class RecoveryQueue {
	private:
	unsigned int focusStep;
	unsigned int getElement;
	Layer* layer;
	int counter;
	int lastelement;
	//unsigned int lastInsertedElement;
	unsigned int stepLastElement;
	vector < vector<Neuron*> > queue;
	int getStep(int);
	public:
	Neuron *lastType2Neuron;
	RecoveryQueue (Layer*);
	int deletePattern(Neuron *,  unsigned int, unsigned int);
	void insert(Neuron *);
	void checkNewPattern ();
	void recover(void);
	//bool noChangeInCycle(void);
	int countItems (void);
	int countInputNeuronsCurrentStep(void);
	int countItemsLastStep(void);
	Neuron *getNext(void);
	int setFocusStep(int);
};
