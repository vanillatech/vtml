#pragma once
#include <set>
#include "input_map.h"
#include "activation_queue.h"
#include "recovery_queue.h"

namespace Odin
{

class Model;

class Layer
{
	public:
		static const float DEFAULT_IMPULSE;

		Layer(int layerNumber, const Configuration& config, Model& model);
		~Layer();

		/// Makes a time step. For layer #1, pattern consists of 1 element
		/// and impulse has default value (from config)
		void step(const TPattern& pattern, float impulse = DEFAULT_IMPULSE);
		
	private:
		typedef	std::set<Neuron*> TNeurons;

		struct ActivationFunc
		{
			ActivationFunc(Layer& layer) : m_layer(layer)
			{}
			
			void operator()(AQItem& item) {
				m_layer.activate(item);
			}

			Layer& m_layer;
		};
		friend struct ActivationFunc;

		Neuron* addNeuron();
		void fireNeuron(Neuron& n);
		void exciteNeuron(Neuron& n, float impulse);
		void decayActivation();
		void activate(AQItem& item);
		
		int m_layerNumber;
		const Configuration& m_config;
		Model& m_model;
		TNeurons m_neurons;
		InputMap m_inputs;
		ActivationFunc m_af;
		ActivationQueue<ActivationFunc> m_AQ;
		RecoveryQueue m_RQ;
		int m_timeCounter;
};

} //Odin
