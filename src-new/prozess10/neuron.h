#pragma once
#include <vector>
//#include <set>
#include "dendrite.h"

namespace Odin
{

typedef int TNeuronID;

struct Neuron
{
	typedef std::vector<Dendrite> TInputs;
	typedef std::vector<Dendrite*> TOutputs;

	TNeuronID id;
	TInputs inputs;
	TOutputs outputs;
	float totalOutputsWeight;
	float threshold;
	float activationValue;

	explicit Neuron(float a_threshold) :
		id(++idCounter),
		totalOutputsWeight(0.0f),
		threshold(a_threshold),
		activationValue(0.0f)
	{}

	bool connectedFrom(const Neuron& from) const;

	Neuron* commonSubsequentNeuron(const Neuron& r) const;

	/// Returns fals if there is already a connection from the neuron given..
	bool connectFrom(Neuron& from, float connectionWeight, int connectionDelay = 1);

	private:
		static TNeuronID idCounter;

		// No copying.
		//Neuron(const Neuron&);
		//Neuron& operator=(const Neuron&);
};

} //Odin

// Functions moved to Layer...

/*
class Neuron {
	private:

	  void drainActivation (void);


	public:
	  //TDebug1 *debuginst;
	  TTreeNode *debugCallingNode;
	  ActivationQueue* activationQueue;
	  vector <RecoveryQueue> *recoveryQueue;
	  vector <Dendrite*> axons;
	  vector <Dendrite*> dendrites;
	  double activationVal;
	  int layer;
	  AnsiString id;
	  int lastchecked;
	  char outputData;
	  int lastfired;
	  int blockActivation;
	  int type; //0:temporal, 1:spatial, 2:downward
	  float threshold;
	  //Constructor
	  Neuron (ActivationQueue*, vector< RecoveryQueue > * ,unsigned int, int);
	  //Functions
	  void newLink (Neuron *, int, int);
	  int countSynapses ();
	  void activate (double);
	  void fire (void);
	  void predictNext (void);
	  void propagateDown(int *);
	  int checkActivation (void);
	  void inhibit (void);
	  bool containsDendrite(Neuron *);
	  int axonsRemove(Dendrite *);
};
*/
