#pragma once
#include <vector>
#include "../odin.h"
#include "configuration.h"

namespace Odin
{

class Layer;
struct ICallback;

class Model : public IModel<TPattern>
{
	public:
		explicit Model(const Configuration& config);
		~Model();
		
		/// Called by external code.
		void setCallbacks(ICallback& callbacks) {
			m_callbacks = &callbacks;
		}

		/// Called by external code.
		virtual void input(const TPattern& pattern);

		/// Called from layer #layerFrom to propagate impulse to higher layers.
		void inputToHigherLayer(int layerFrom, const TPattern& pattern, float impulse);

	private:
		const Configuration& m_config;
		ICallback* m_callbacks;
		std::vector<Layer*> m_layers;
};

} //Odin
