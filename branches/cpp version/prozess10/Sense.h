#pragma once

class Neuron;
class Layer;

class Sense {
	private:
	//provide 8 bit input interface
	Neuron *inputNeurons[256];
	Neuron *outputNeurons[256];
	Layer *layer;
	public:
	Sense (Layer* nLayer);

	void init();

	// initializes input #c
	void init(int c);

	void input (int c);
	void nextStep (void);
	void nextStep (bool);
};
