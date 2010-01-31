#pragma once
#include <deque>

namespace Odin
{

template <class ObjectType>
class RecoveryQueue
{
		typedef std::deque<ObjectType> TQueue;
	public:
		RecoveryQueue(size_t a_depth) :
			m_depth(a_depth)
		{}

		/// Recovers. Visitor takes objects as a parameter to operator()()
		template <class TVisitor> void recover()
		{
				////....
		}

	private:
		TQueue m_queue;
		size_t m_depth;
		size_t m_currentDepth;
};

} //Odin
