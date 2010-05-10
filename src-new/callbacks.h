#pragma once
#include <string>

namespace Odin
{

struct ICallbackMsg
{
	virtual ~ICallbackMsg() {}
	virtual int type() = 0;
	virtual std::string message() = 0;
};

class Callbacks
{
	public:
		virtual ~Callbacks() 
		{}
		
		//default callbacks do nothing... inherit.
		virtual void onCallback(ICallbackMsg& msg)
		{}
};

//TODO: add callbacks on various events...

} //Odin
