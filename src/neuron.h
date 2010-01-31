#pragma once
#include <list>
#include <algorithm>

namespace Odin
{

/// Dendrite dynamic storage policy used by neurons.
template <class DendriteType>
class DendriteDynamicStorage 
{
		typedef std::list<DendriteType*> TDendrites; //change to std::set?
	public:
		typedef TDendrites::iterator iterator;
		typedef TDendrites::const_iterator const_iterator;
		typedef typename DendriteType TDendrite;
		typedef typename DendriteType::TData TDendriteData;
		
		void add(TDendrite* a_dendrite)
		{
			m_dendrites.push_back(a_dendrite);
		}

		TDendrite* add(const TDendriteData& a_data, void* a_neuronFrom, void* a_neuronTo)
		{
			TDendrite* d = new TDendrite(a_data, a_neuronFrom, a_neuronTo);
			m_dendrites.push_back(d);
			return d;
		}

		void remove(TDendrite* a_dendrite)
		{
			TDendrites::iterator it = 
				std::find(m_dendrites.begin(), m_dendrites.end(), a_dendrite);
			if (it != m_dendrites.end())
				m_dendrites.erase(it);
		}

		void destroyAll()
		{
			for (TDendrites::iterator it = m_dendrites.begin(); it != m_dendrites.end(); ++it) {
				delete *it;
			}
		}

		iterator begin() {
			return m_dendrites.begin();
		}

		iterator end() {
			return m_dendrites.end();
		}

	private:
		TDendrites m_dendrites;
};

/// A neuron is the entity that can be connected to other neurons
/// via dendrites (DendriteStorageType<DendriteType>). 
/// Takes ownership on input connections.
/// All data associated with neuron provided in NeuronDataType object.
/// NeuronIDType unique identifies a neuron in the model 
/// (it could be int, pointer, or some coordinates)
template <
	class NeuronIDType, class NeuronDataType, 
	class DendriteStorageType
> 
class TNeuron
{
		typedef TNeuron<NeuronIDType, NeuronDataType, DendriteStorageType> ThisType;
		
	public:
		typedef typename DendriteStorageType::TDendriteData TDendriteData;
		typedef typename DendriteStorageType::TDendrite TDendrite;
		typedef DendriteStorageType::iterator iterator;
		typedef DendriteStorageType::const_iterator const_iterator;

		/// Ctor.
		TNeuron(const NeuronIDType& a_id, const NeuronDataType& a_data) :
			m_id(a_id),
			m_data(a_data)
		{}

		/// Dtor.
		~TNeuron() {
			m_dendrites.destroyAll();	
		}

		void connect(TNeuron* a_from, const TDendriteData& a_dendriteData)
		{
			TDendrite* d = m_dendrites.add(a_dendriteData, a_from, this);
			a_from->m_axons.add(d);
		}

		//disconnect etc..

		// Accessors.
		const NeuronIDType& id() const {
			return m_id;
		}

		const NeuronDataType& data() const {
			return m_data;
		}

		NeuronDataType& data() {
			return m_data;
		}

		//begin(), end() for axons and dendrites...

		iterator dendritesBegin() {
			return m_dendrites.begin();
		}

		iterator dendritesEnd() {
			return m_dendrites.end();
		}

		iterator axonsBegin() {
			return m_axons.begin();
		}

		iterator axonsEnd() {
			return m_axons.end();
		}
	
	private:
		
		NeuronIDType m_id;
		NeuronDataType m_data;
		DendriteStorageType m_dendrites;
		DendriteStorageType m_axons;

		// No copying.
		TNeuron(const TNeuron&);
		TNeuron& operator=(const TNeuron&);
};

} //Odin
