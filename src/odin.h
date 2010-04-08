#include <queue>
#include <vector>
#include "debug.h"

using namespace std;

const int queueMax = 9;
const int recoveryTime = 1;
const int blockTime = 1;
const float activationBias = 0.5;
const float drain = 0.9;
const float minActivation = 0.01;
const float fireMargin = 0.1;
const float learnRate = 0.5;

int neuronCounter = 0;
int stepCounter = recoveryTime + 1;
int recQueueIdle = -1;

class Neuron;
class Dendrite;
class ActivationQueue;
class Sense;
class RecoveryQueue;


#include "odin_classes.h"

vector <RecoveryQueue> recQueue;
//RecoveryQueue *recQueue = new RecoveryQueue[1];
ActivationQueue aqueue;
Sense *readSense;
Neuron *lastFiredNeuron;

bool operator<(const Aqueue &a, const Aqueue &b) {
	if (a.retMode()) return a.getPos() > b.getPos();
	else return a.getPos() < b.getPos();
}

#include "odin_ActivationQueue.h"
#include "odin_Neuron.h"
#include "odin_Dendrite.h"
#include "odin_RecoveryQueue.h"

