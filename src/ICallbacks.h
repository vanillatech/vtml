#pragma once

namespace Odin
{

class ICallbacks
{
	virtual ~ICallbacks() {}
	
	public:
		virtual void stepCounterCallback(int stepCounter) = 0;
};

} //Odin
