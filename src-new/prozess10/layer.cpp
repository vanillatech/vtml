#include "stdafx.h"
#include "neuron.h"
#include "layer.h"

#pragma warning(disable: 4355) //'this' in ctor. initializer list

namespace Odin
{
/*
int m_layerNumber;
		const Configuration& m_config;
		TNeurons m_neurons;
		InputMap m_inputs;
		ActivationFunc m_af;
		ActivationQueue<ActivationFunc> m_AQ;
		RecoveryQueue m_RQ;
		int m_timeCounter;
*/


Layer::Layer(int layerNumber, const Configuration& config, Model& model) :
	m_layerNumber(layerNumber),
	m_config(config),
	m_model(model),
	m_af(*this),
	m_AQ(m_af),
	m_timeCounter(0)
{}

Layer::~Layer()
{
	TNeurons::iterator it = m_neurons.begin();
	TNeurons::iterator end = m_neurons.end();
	for (; it != end; ++it) {
		delete (*it);
	}
}

const float Layer::DEFAULT_IMPULSE = -1.0f;

void Layer::step(const TPattern& pattern, float impulse)
{
	float imp = (impulse > 0.0f) ? impulse : m_config.defaultImpulse;
	
	// If input neuron designated by given pattern doesn't exist, it's created.
	// (Then this neuron is fired.)
	Neuron* n = m_inputs.find(pattern);
	if (!n) {
		n = addNeuron();
		m_inputs.insert(pattern, n);
		exciteNeuron(*n, imp);
		fireNeuron(*n);
	}
	else {
		exciteNeuron(*n, imp);
	}
	


	decayActivation();		
}

Neuron* Layer::addNeuron()
{
	Neuron* n = new Neuron(m_config.threshold);
	m_neurons.insert(n);
	return n;
}

void Layer::exciteNeuron(Neuron& n, float impulse)
{
	n.activationValue += impulse;
	if (n.activationValue > n.threshold)
		fireNeuron(n);
}

void Layer::fireNeuron(Neuron& n)
{
	float impulse = n.activationValue;
	n.activationValue = 0.0f;
	if (n.totalOutputsWeight > 0.0f) {
		impulse /= n.totalOutputsWeight;
		Neuron::TOutputs::iterator it = n.outputs.begin();
		Neuron::TOutputs::iterator end = n.outputs.end();
		for (; it != end; ++it) {
			Dendrite* d = *it;
			m_AQ.scheduleActivation(d, impulse*d->weight, d->activationDelay);
		}
	}
	m_RQ.put(n);
}

void Layer::decayActivation()
{
	TNeurons::iterator it = m_neurons.begin();
	TNeurons::iterator end = m_neurons.end();
	for (; it != end; ++it) {
		(*it)->activationValue *= m_config.activationDecay;
	}
}

} //Odin
