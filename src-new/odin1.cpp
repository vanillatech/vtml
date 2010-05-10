#include "stdafx.h"
#include "prozess10/configuration.h"
#include "prozess10/model.h"
#include "odin1.h"
#include "odin1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Codin1App, CWinApp)
END_MESSAGE_MAP()

Codin1App::Codin1App()
{
}

Codin1App theApp;

BOOL Codin1App::InitInstance()
{
	CWinApp::InitInstance();

	Odin::Configuration config;
	Odin::Model model(config);
	Codin1Dlg dlg(model);
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
