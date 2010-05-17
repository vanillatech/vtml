#pragma once

namespace Odin
{

struct Neuron;

typedef int TDendriteID;

struct Dendrite
{
	TDendriteID id;
	Neuron* from;
	Neuron* to;	
	float weight;
	int activationDelay;
	
	Dendrite(Neuron* a_from, Neuron* a_to, float a_weight, int a_activationDelay = 1) :
		id(++idCounter),
		from(a_from),
		to(a_to),
		weight(a_weight),
		activationDelay(a_activationDelay)
	{}
	
	private:
		static TDendriteID idCounter;

		// No copying.
		//Dendrite(const Dendrite&);
		//Dendrite& operator=(const Dendrite&);
};

/*
class Dendrite {
	public:
	ActivationQueue *aqueuelocal;
	Neuron *dendriteFrom;
	Neuron *dendriteTo;
	int synapses;
	int lastUsed;
	//bool inhibitory;
	int activationDelay;
	float weight;
	Dendrite(void);
	Dendrite(int);
	void changeWeights (void);
	void stimulate (void);
};
*/

} //Odin
