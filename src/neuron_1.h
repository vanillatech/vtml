#pragma once
#include <string>
#include "neuron.h"
#include "dendrite_1.h"

namespace Odin
{

struct Neuron1Data
{
	double activationVal;
	int layer;
	TTime lastchecked;
	std::string outputData;
	int lastfired;
	TTime blockActivation;
	//int type;
	float threshold;
};

typedef TNeuron<TNeuronID, Neuron1Data, DendriteDynamicStorage<Dendrite1Data> Neuron;

} //Odin
