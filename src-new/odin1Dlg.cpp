#include "stdafx.h"
#include "odin1.h"
#include "odin1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Codin1Dlg::Codin1Dlg(Odin::Model& model) : 
	CDialog(Codin1Dlg::IDD, 0),
	m_model(model)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Codin1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_inputBox);
	DDX_Control(pDX, IDC_LIST1, m_logWnd);
}

BEGIN_MESSAGE_MAP(Codin1Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &Codin1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


BOOL Codin1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);
	return TRUE;
}

void Codin1Dlg::OnBnClickedButton1()
{
	char buffer[1024];
	m_inputBox.GetWindowText(buffer, 1024);
	std::string str(buffer);
	m_model.input(str);
}

void Codin1Dlg::onCallback(Odin::ICallbackMsg& msg)
{
	m_logWnd.AddString(msg.message().c_str());
}
