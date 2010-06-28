#include "stdafx.h"
#include "Form1.h"
#pragma unmanaged
#include "callbacks.h"
#include "prozess10/Sense.h"
#include "prozess10/globals.h"
#pragma managed

using namespace srcnew1;
using namespace Odin;
using namespace System::Runtime::InteropServices;
using namespace System;

typedef void (*Forward)(ICallbackMsg* msg);
typedef void (*Forward1)(int code, const char* param1, const char* param2, const char* param3);
public delegate void D(ICallbackMsg* msg);
public delegate void D1(int code, const char* param1, const char* param2, const char* param3);

[StructLayoutAttribute( LayoutKind::Sequential, CharSet = CharSet::Ansi )]
public ref struct Managed_Delegate_Wrapper
{
	[MarshalAsAttribute(UnmanagedType::FunctionPtr)]
	D^ d;
};

[StructLayoutAttribute( LayoutKind::Sequential, CharSet = CharSet::Ansi )]
public ref struct Managed_Delegate_Wrapper1
{
	[MarshalAsAttribute(UnmanagedType::FunctionPtr)]
	D1^ d;
};



#pragma unmanaged

struct Unmanaged_Wrapper
{
	Forward func;
};

struct Unmanaged_Wrapper1
{
	Forward1 func;
};

/*
class Callback : public ICallback
{
	public:
		//Callback(Forward f) : func(f)
		//{}

		Callback(Forward1 f) : func1(f)
		{}

		virtual void onCallback(ICallbackMsg* msg)
		{
			//func(msg);
			if (!msg) return;
			std::string message = msg->message();
			const char* s = message.c_str();
	
			if (msg->type() == MSG_NEW_INPUT_NEURON) {
				CallbackMsg<MSG_NEW_INPUT_NEURON>* m = (CallbackMsg<MSG_NEW_INPUT_NEURON>*)msg;
				func1(msg->type(), s, m->id.c_str(), m->value.c_str());
			}
			else if (msg->type() == MSG_NEW_LINK) {
				CallbackMsg<MSG_NEW_LINK>* m = (CallbackMsg<MSG_NEW_LINK>*)msg;
				func1(msg->type(), s, m->fromId.c_str(), m->toId.c_str());
			}
			else {
				func1(msg->type(), s, "", "");
			}
			delete msg;
		}

	private:
		//Forward func;
		Forward1 func1;
};
*/

class Callback : public ICallback
{
	public:
		Callback(Forward f) : func(f)
		{}

		virtual void onCallback(ICallbackMsg* msg)
		{
			func(msg);
		}

	private:
		Forward func;
		
};

ICallback* callback;

#pragma managed

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	Sense* sense = new Sense(globals.inputLayer);

	Form1^ form = gcnew Form1(sense);

	Managed_Delegate_Wrapper^ w = gcnew Managed_Delegate_Wrapper;
	//Managed_Delegate_Wrapper1^ w = gcnew Managed_Delegate_Wrapper1;
	w->d = gcnew D(form, &Form1::OnCallback);
	//w->d = gcnew D1(form, &Form1::OnCallback1);
	Unmanaged_Wrapper u;
	//Unmanaged_Wrapper1 u;
	Marshal::StructureToPtr(w, (System::IntPtr)&u, false);
	callback = new Callback(u.func);

	// Create the main window and run it
	Application::Run(form);
	delete callback;
	delete sense;
	return 0;
}
