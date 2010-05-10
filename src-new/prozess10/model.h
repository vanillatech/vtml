#pragma once
#include <vector>
#include "configuration.h"

namespace Odin
{

class Layer;
class Callbacks;

class Model
{
	public:
		explicit Model(const Configuration& config);
		~Model();
		
		/// Called by external code.
		void setCallbacks(Callbacks& callbacks) {
			m_callbacks = &callbacks;
		}

		/// Called by external code.
		void input(const TPattern& pattern);

		/// Called from layer #layerFrom to propagate impulse to higher layers.
		void inputToHigherLayer(int layerFrom, const TPattern& pattern, float impulse);

	private:
		const Configuration& m_config;
		Callbacks* m_callbacks;
		std::vector<Layer*> m_layers;
};

} //Odin
