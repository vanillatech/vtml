#pragma once
#include <vector>

namespace Odin
{

struct Dendrite;

struct AQPair
{
	float impulse;		///< activation weight
	Dendrite* dendrite; ///< connection

	AQPair(float imp, Dendrite* d) : impulse(imp), dendrite(d)
	{}
};

typedef std::vector<AQPair> AQItem;

template <class TActivationFunction>
class ActivationQueue
{
		typedef std::vector<AQItem> TQueue;
	public:
		
		ActivationQueue(TActivationFunction& func, size_t maxDepth = 10) :
			m_function(func),
			m_depth(maxDepth),
			m_currentIndex(0)
		{
			m_queue.resize(m_depth);
		}

		~ActivationQueue()
		{}

		/// Makes the next step
		void step()
		{
			AQItem& item = m_queue[m_currentIndex];
			if (!item.empty()) {
				m_function(item);
				item.clear();
			}
			if (++m_currentIndex >= m_depth) {
				m_currentIndex = 0;
			}
		}

		/// Schedules activation.
		bool scheduleActivation(Dendrite* d, float impulse, int delay)
		{
			if (delay <= 0 || delay >= (int)m_depth) {
				return false;
			}
			size_t index = m_currentIndex + delay;
			if (index >= m_depth) {
				index -= m_depth;
			}
			m_queue[index].push_back(AQPair(impulse, d));
			return true;
		}

	private:
		TQueue m_queue;
		TActivationFunction& m_function;
		size_t m_depth;
		size_t m_currentIndex;
};

} //Odin
