#pragma once
#include <map>
#include <set>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include "../odin.h"

namespace Odin
{

class GUI : public ICallback
{
	public:
		explicit GUI(IModel<std::string>& model);
		~GUI();
		
		int run();

		bool onWindowsMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		virtual void onCallback(ICallbackMsg* msg);
		
		bool createWindows();
		int runMessageLoop();
		void showTreeView(bool show);
		void inputPattern();
		void processCallbackMsg(ICallbackMsg* msg);
		void insertTreeItem(int parentID, int ID);

		typedef std::set<HTREEITEM> THandleSet;
		typedef std::map<int, THandleSet> TTreeSync;

		TTreeSync m_treeSync;
		IModel<std::string>& m_model;
		HWND m_mainFrame;
		HWND m_inputEdit;
		HWND m_logListBox;
		HWND m_showTreeViewCheckBox;
		HWND m_treeViewFrame;
		HWND m_treeViewWnd;
		bool m_started;
};

} //Odin
