#pragma once
#include "callbacks.h"
#include "prozess10/model.h"

// Codin1Dlg dialog
class Codin1Dlg : public CDialog, public Odin::Callbacks
{
public:
	Codin1Dlg(Odin::Model& model);

	enum { IDD = IDD_ODIN1_DIALOG };

private:
	DECLARE_MESSAGE_MAP()

	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	virtual void onCallback(Odin::ICallbackMsg& msg);

	CEdit m_inputBox;
	CListBox m_logWnd;
	Odin::Model& m_model;
};
