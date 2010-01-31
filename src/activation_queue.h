#pragma once
#include <utility>
#include <vector>
#include <config.h>

namespace Odin
{

/// Activation queue. IDType identifies objects. Default IDType() is the invalid object.
/// CallbackType must have void operator()() and be copy-constructable.
template <class IDType, class CallbackType>
class ActivationQueue
{
		typedef std::vector<std::pair<IDType, CallbackType> > TQueueItem;
		typedef std::vector<TQueueItem> TQueue;
	public:
		
		explicit ActivationQueue(size_t a_maxDepth) :
			m_depth(a_maxDepth),
			m_currentIndex(0)
		{
			m_queue.resize(m_depth);
		}

		~ActivationQueue()
		{}

		/// Makes the next step
		void step()
		{
			static const IDType noID = IDType();
			TQueueItem& item = m_queue[m_currentIndex];
			size_t sz = item.size();
			for (size_t i=0; i<sz; ++i) {
				if (item[i].first != noID) {
					item[i].second();
				}
			}
			item.clear();
			if (++m_currentIndex >= m_depth) {
				m_currentIndex = 0;
			}
		}

		/// Schedules activation.
		int scheduleActivation(TTime a_delay, const IDType& a_id, CallbackType& a_callback)
		{
			if (a_delay == 0) {
				a_callback();
				return 0;
			}
			if (a_delay < 0 || a_delay >= m_depth) {
				return -1;
			}
			size_t index = m_currentIndex + a_delay - 1;
			if (index >= m_depth) {
				index -= m_depth;
			}
			m_queue[index].push_back(std::make_pair(a_id, a_callback));
			return 1;
		}

		/// Removes all occurences of a given object from the queue.
		void remove(const IDType& a_id)
		{
			for (size_t i=0; i<m_depth; ++i) {
				TQueueItem& item = m_queue[i];
				size_t sz = item.size();
				for (size_t j=0; j<sz; ++j) {
					if (item[j].first == a_id) {
						item[j].first = IDType();
					}
				}
			}
		}

	private:
		TQueue m_queue;
		size_t m_depth;
		size_t m_currentIndex;
};

} //Odin
