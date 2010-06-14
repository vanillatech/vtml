#pragma once
#include <vector>

using namespace std;

class Neuron;

class RecoveryQueue {
	private:
	Layer* layer;
	int counter;
	int lastelement;
	//unsigned int lastInsertedElement;
	unsigned int stepLastElement;
	vector < vector<Neuron*> > queue;
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
};
