#pragma once

class Neuron;
class Layer;

class Dendrite {
	public:
	bool weightFrozen;
	Neuron *dendriteFrom;
	Neuron *dendriteTo;
	int synapses;
	unsigned int lastUsed;
	//bool inhibitory;
	int activationDelay;
	float weight;
	Dendrite(Layer *);
	Dendrite(Layer *, float);
	void changeWeights (void);
	void stimulate (void);
	void stimulate (int);
	void stimulate (float);
	void stimulate (float, int);
	float getWeight (void);
	private:
	Layer *layer;
};
