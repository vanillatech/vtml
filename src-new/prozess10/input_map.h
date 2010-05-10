#pragma once
#include <string>
#include <map>
#include "configuration.h"

namespace Odin
{

struct Neuron;

// Simple input map so far. Something more complex will be provided for audio patterns.
class InputMap
{
		typedef std::map<TPattern, Neuron*> TMap;
	public:
		Neuron* find(const TPattern& pattern)
		{
			TMap::iterator it = m_map.find(pattern);
			return (it == m_map.end() ? 0 : it->second);
		}
		
		void insert(const TPattern& pattern, Neuron* neuron)
		{
			m_map.insert(std::make_pair(pattern, neuron));
		}
	
	private:
		TMap m_map;
};

} //Odin
