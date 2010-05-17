#pragma once
#include <vector>

namespace Odin
{

struct Neuron;

typedef std::vector<Neuron*> RQItem;

class RecoveryQueue
{
		typedef std::vector<RQItem> TQueue;
	public:
		explicit RecoveryQueue(size_t depth = 20) :
			m_depth(depth),
			m_currentDepth(0),
			m_currentIndex(m_depth)
		{
			m_queue.resize(m_depth);
		}

		/// Returns max depth of this RQ
		size_t depth() const {
			return m_depth;
		}

		/// Makes a time step.
		void step()
		{
			if (m_currentDepth == m_depth-1) {
				at(m_currentDepth).clear();
			}
			else {
				++m_currentDepth;
			}
			if (m_currentIndex == 0) {
				m_currentIndex = m_depth-1;
			}
			else {
				--m_currentIndex;
			}
		}
		
		/// Puts a neuron to the current position of RQ.
		void put(Neuron& n)
		{
			if (m_currentDepth == 0)
				step();
			at(0).push_back(&n);
		}

		const RQItem& operator[](size_t index) const
		{
			if (index >= m_currentDepth) {
				return m_infiniteItem;
			}
			return m_queue[getInternalIndex(index)];
		}

	private:
		size_t getInternalIndex(size_t index) const
		{
			size_t i = m_currentIndex + index;
			if (i >= m_depth) {
				i -= m_depth;
			}
			return i;
		}

		RQItem& at(size_t index)
		{
			return m_queue[getInternalIndex(index)];
		}

		TQueue m_queue;
		size_t m_depth;
		size_t m_currentDepth;
		size_t m_currentIndex;
		RQItem m_infiniteItem; // always empty item for consistency of operator[]
};



} //Odin
