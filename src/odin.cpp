//---------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("SDIMAIN.CPP", SDIAppForm);
USEFORM("about.cpp", AboutBox);
USEFORM("debug.cpp", Debug1);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application->Initialize();
	Application->CreateForm(__classid(TSDIAppForm), &SDIAppForm);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TDebug1), &Debug1);
		Application->Run();

	return 0;
}
//---------------------------------------------------------------------

