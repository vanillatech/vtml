#pragma once

namespace Odin
{

class ICallbacks;

template <class InputType> class ISense
{
		virtual ~ISense() {}
	public:
		explicit ISense(ICallbacks& a_callbacks) :
			m_callbacks(a_callbacks)
		{}

		virtual void input(const InputType&)= 0;

		/// Input for multiple items.
		virtual void input(const InputType* a_vec, size_t a_count)
		{
			//default implementation.
			for (size_t i=0; i<a_count; ++i) {
				input(a_vec[i]);
			}
		}

	protected:
		ICallbacks& m_callbacks;
};

} //Odin
