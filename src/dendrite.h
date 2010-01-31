#pragma once

namespace Odin
{

/// Dendrite holds information about link between 2 neurons.
/// All data associated with the link are parametrized in DendriteDataType object.
template <class DendriteDataType>
class TDendrite
{
	public:
		typedef typename DendriteDataType TData;

		/// Ctor.
		TDendrite(const DendriteDataType& a_data, void* a_from, void* a_to) :
			m_data(a_data),
			m_from(a_from),
			m_to(a_to)
		{}

		/// Dtor.
		~TDendrite()
		{}

		// Accessors.

		const DendriteDataType& data() const {
			return m_data;
		}

		const void* from() const {
			return m_from;
		}

		const void* to() const {
			return m_to;
		}

		DendriteDataType& data() {
			return m_data;
		}

		void* from() {
			return m_from;
		}

		void* to() {
			return m_to;
		}

	private:

		DendriteDataType m_data;
		void* m_from;
		void* m_to;

		// No copying.
		TDendrite(const TDendrite&);
		TDendrite& operator=(const TDendrite&);
};

/*
/// Dendrite holds information about link between 2 neurons.
/// All data associated with the link are parametrized in DendriteDataType object.
template <class DendriteDataType, class NeuronType>
class TDendrite
{
	public:
		typedef typename DendriteDataType TData;

		/// Ctor.
		TDendrite(const DendriteDataType& a_data, NeuronType& a_from, NeuronType& a_to) :
			m_data(a_data),
			m_from(a_from),
			m_to(a_to)
		{}

		/// Dtor.
		~TDendrite()
		{}

		// Accessors.

		const DendriteDataType& data() const {
			return m_data;
		}

		const NeuronType& from() const {
			return m_from;
		}

		const NeuronType& to() const {
			return m_to;
		}

		DendriteDataType& data() {
			return m_data;
		}

		NeuronType& from() {
			return m_from;
		}

		NeuronType& to() {
			return m_to;
		}

	private:

		DendriteDataType m_data;
		NeuronType& m_from;
		NeuronType& m_to;

		// No copying.
		TDendrite(const TDendrite&);
		TDendrite& operator=(const TDendrite&);
};
*/

} //Odin
