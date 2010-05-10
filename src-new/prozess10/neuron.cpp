#include "stdafx.h"
//#include <cassert>
#include "neuron.h"

namespace Odin
{

TNeuronID Neuron::idCounter = 0;

bool Neuron::connectedFrom(const Neuron& from) const {
	TOutputs::const_iterator it = from.outputs.begin();
	TOutputs::const_iterator end = from.outputs.end();
	for (; it != end; ++it) {
		if ((*it)->to == this)
			return true;
	}
	return false;
}

Neuron* Neuron::commonSubsequentNeuron(const Neuron& r) const
{
	TOutputs::const_iterator it = outputs.begin();
	TOutputs::const_iterator end = outputs.end();
	for (; it != end; ++it) {
		if (r.connectedFrom(*((*it)->to)))
			return (*it)->to;
	}
	return 0;
}

bool Neuron::connectFrom(
	Neuron& from, float connectionWeight, int connectionDelay
)
{
	if (connectedFrom(from))
		return false;
	inputs.push_back(Dendrite(&from, this, connectionWeight, connectionDelay));
	//from.outputs.insert(&inputs.back());
	from.outputs.push_back(&inputs.back());
	from.totalOutputsWeight += connectionWeight;
	return true;
}

} //Odin
