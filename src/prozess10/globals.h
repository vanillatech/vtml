#pragma once
#include <vector>
//#include "RecoveryQueue.h"
//#include "ActivationQueue.h"
#include "Layer.h"

using namespace std;

class Neuron;
//class Dendrite;
//class ActivationQueue;
//class Sense;
//class RecoveryQueue;

struct Globals
{
	static const int queueMax = 9;
	static const int recoveryTime = 1;
	static const int blockTime = 1;
	const float activationBias;
	const float drain;
	const float minActivation;
	const float fireMargin;
	const float learnRate;

	//vector <RecoveryQueue> recQueue;
	//RecoveryQueue *recQueue = new RecoveryQueue[1];
	//ActivationQueue aqueue;
	//Sense *readSense;
	Neuron *lastFiredNeuron;
	Layer inputLayer;
	//--doesn't work because form isn't initialized yet - borland c++
	//Sense readSense(&inputLayer);
	
	//Sense readSense;

	int neuronCounter;
	int stepCounter;
	int recQueueIdle;

	Globals() :
		activationBias(0.5f),
		drain(0.9f),
		minActivation(0.01f),
		fireMargin(0.1f),
		learnRate(0.5f),
		lastFiredNeuron(0),
		neuronCounter(0),
		stepCounter(recoveryTime+1),
		recQueueIdle(-1)
	{}
};

extern Globals globals;


//#include "odin_ActivationQueue.h"
//#include "odin_Neuron.h"
//#include "odin_Dendrite.h"
//#include "odin_RecoveryQueue.h"
