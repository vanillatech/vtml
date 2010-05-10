#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class Codin1App : public CWinApp
{
public:
	Codin1App();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern Codin1App theApp;
