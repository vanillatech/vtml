#include <stdexcept>
#include "main_frame.h"
#include "treeview_frame.h"
#include "resource.h"
#include "gui.h"

namespace Odin
{

static const UINT MSG_CALLBACK = WM_USER+22;

GUI::GUI(IModel<std::string>& model) :
	m_model(model),
	m_mainFrame(0),
	m_inputEdit(0),
	m_logListBox(0),
	m_showTreeViewCheckBox(0),
	m_treeViewFrame(0),
	m_treeViewWnd(0),
	m_started(false)
{
	::InitCommonControls();
}

GUI::~GUI()
{
}
		
int GUI::run()
{
	if (!createWindows()) {
		throw std::runtime_error("GUI initialization failed.");
	}
	m_started = true;
	return runMessageLoop();
}

void GUI::onCallback(ICallbackMsg* msg)
{
	if (!m_started || !msg)
		return;
	::PostMessage(m_mainFrame, MSG_CALLBACK, 0, (LPARAM)msg);
}

void GUI::processCallbackMsg(ICallbackMsg* msg)
{
	if (!msg)
		return;
	::SendMessage(m_logListBox, LB_ADDSTRING, 0, (LPARAM)(msg->message().c_str()));
	if (msg->type() == MSG_NEW_LINK) {
		CallbackMsg<MSG_NEW_LINK>* m = (CallbackMsg<MSG_NEW_LINK>*)msg;
		insertTreeItem(m->fromId, m->toId);
	}
	delete msg;
}

void GUI::showTreeView(bool show)
{
	::ShowWindow(m_treeViewFrame, show ? SW_SHOW : SW_HIDE);
	::CheckDlgButton(m_mainFrame, IDC_CHECKBOX_SHOW_TREEVIEW, show ? BST_CHECKED : BST_UNCHECKED);
}

void GUI::inputPattern()
{
	char buffer[1024];
	::GetWindowTextA(m_inputEdit, buffer, 1023);
	std::string pattern(buffer);
	if (!pattern.empty()) {
		m_model.input(pattern);
		::SetWindowTextA(m_inputEdit, "");
	}
	::SetFocus(m_inputEdit);
}

void GUI::insertTreeItem(int parentID, int ID)
{
	if (ID < 0) {
		return;
	}

	TVINSERTSTRUCT tvi;
	memset(&tvi, 0, sizeof(tvi));
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.mask = TVIF_TEXT;

	char buf[32];
	_itoa_s(ID, buf, 32, 10);
	tvi.item.pszText = buf;
	
	if (parentID < 0) {
		TTreeSync::iterator it = m_treeSync.find(ID);
		if (it != m_treeSync.end())
			return;
		HTREEITEM handle = TreeView_InsertItem(m_treeViewWnd, &tvi);
		if (!handle)
			return;
		m_treeSync.insert(std::make_pair(ID, THandleSet()));
		return;
	}

	TTreeSync::iterator pit = m_treeSync.find(parentID);
	if (pit == m_treeSync.end() || pit->second.empty())
		return;
	TTreeSync::iterator it = m_treeSync.find(ID);
	if (it == m_treeSync.end()) {
		it = m_treeSync.insert(std::make_pair(ID, THandleSet())).first;
	}
	THandleSet::iterator h = pit->second.begin();
	THandleSet::iterator end = pit->second.end();
	for (; h != end; ++h) {
		tvi.hParent = *h;
		HTREEITEM handle = TreeView_InsertItem(m_treeViewWnd, &tvi);
		if (handle) {
			it->second.insert(handle);
		}
	}
}

bool GUI::onWindowsMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
		case MSG_CALLBACK:
			processCallbackMsg((ICallbackMsg*)lp);
			break;
		case WM_COMMAND:
			switch (LOWORD(wp)) {
				case IDC_BUTTON_GO:
					inputPattern();
					break;
				case IDC_CHECKBOX_SHOW_TREEVIEW:
					UINT checked = ::IsDlgButtonChecked(m_mainFrame, IDC_CHECKBOX_SHOW_TREEVIEW);
					showTreeView(checked == BST_CHECKED);
					break;
			}
			break;
		case WM_CLOSE:
			if (hwnd == m_treeViewFrame) {
				showTreeView(false);
			}
			else if (hwnd == m_mainFrame) {
				m_started = false;
				DestroyWindow(m_treeViewFrame);
				::PostQuitMessage(0);
			}
			break;
		default:
			return false;
	}
	return true;
}

bool GUI::createWindows()
{
	m_mainFrame = CreateMainFrame(*this);
	if (!m_mainFrame) {
		return false;
	}
	m_inputEdit = ::GetDlgItem(m_mainFrame, IDC_EDIT_INPUT);
	if (!m_inputEdit) {
		return false;
	}
	m_logListBox = ::GetDlgItem(m_mainFrame, IDC_LIST_LOG);
	if (!m_logListBox) {
		return false;
	}
	m_showTreeViewCheckBox = ::GetDlgItem(m_mainFrame, IDC_CHECKBOX_SHOW_TREEVIEW);
	if (!m_showTreeViewCheckBox) {
		return false;
	}
	m_treeViewFrame = CreateTreeviewFrame(*this, m_mainFrame);
	if (!m_treeViewFrame) {
		return false;
	}
	m_treeViewWnd = ::GetDlgItem(m_treeViewFrame, IDC_TREEVIEW);
	if (!m_treeViewWnd) {
		return false;
	}

	showTreeView(false);
	::ShowWindow(m_mainFrame, SW_SHOW);

	return true;
}

int GUI::runMessageLoop()
{
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

} //Odin
