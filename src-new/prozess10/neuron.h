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
