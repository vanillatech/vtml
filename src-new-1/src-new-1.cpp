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
public delegate void D(ICallbackMsg* msg);

[StructLayoutAttribute( LayoutKind::Sequential, CharSet = CharSet::Ansi )]
public ref struct Managed_Delegate_Wrapper
{
	[MarshalAsAttribute(UnmanagedType::FunctionPtr)]
	D^ d;
};


#pragma unmanaged

struct Unmanaged_Wrapper
{
	Forward func;
};

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

	Sense* sense = new Sense(&globals.inputLayer);

	Form1^ form = gcnew Form1(sense);
	
	Managed_Delegate_Wrapper^ w = gcnew Managed_Delegate_Wrapper;
	w->d = gcnew D(form, &Form1::OnCallback);
	Unmanaged_Wrapper u;
	Marshal::StructureToPtr(w, (System::IntPtr)&u, false);
	callback = new Callback(u.func);

	// Create the main window and run it
	Application::Run(form);
	delete callback;
	delete sense;
	return 0;
}
