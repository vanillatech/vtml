#include "stdafx.h"
#include "layer.h"
#include "model.h"

namespace Odin
{

Model::Model(const Configuration& config) :
	m_config(config),
	m_callbacks(0)
{
}

Model::~Model()
{
	int sz = (int)m_layers.size();
	for (int i=sz-1; i>=0; --i) {
		delete m_layers[i];
	}
}
		
void Model::input(const TPattern& pattern)
{
	if (m_layers.empty()) {
		m_layers.push_back(new Layer(1, m_config, *this));
	}
	TPattern p;
	for (size_t i = 0; i<pattern.size(); ++i) {
		p = pattern[i];
		m_layers[0]->step(p);
	}
}

void Model::inputToHigherLayer(int layerFrom, const TPattern& pattern, float impulse)
{
	//layers are counted from 1, so the subsequent layer should be at index 'layerFrom'
	
	if (layerFrom < 1 || layerFrom > (int)m_layers.size()) {
		//unknown error
		return;
	}
	if (m_layers.size() == (size_t)layerFrom) {
		m_layers.push_back(new Layer(layerFrom + 1, m_config, *this));
	}
	m_layers[layerFrom]->step(pattern, impulse);
}

} //Odin
