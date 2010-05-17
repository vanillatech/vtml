#include "gui.h"
#include "resource.h"
#include "treeview_frame.h"

namespace Odin
{

static INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);

HWND CreateTreeviewFrame(GUI& gui, HWND mainFrame)
{
	HWND hwnd = ::CreateDialog(
		::GetModuleHandle(0), MAKEINTRESOURCE(IDD_TREEVIEW_FRAME), mainFrame, DialogProc
	);
	if (!hwnd) {
		return 0;
	}
	::SetWindowLongPtr(hwnd, DWLP_USER, (LONG_PTR)&gui);
	return hwnd;
}

static void AdjustSize(HWND hwnd)
{
	RECT rc;
	::GetClientRect(hwnd, &rc);
	::MoveWindow(::GetDlgItem(hwnd, IDC_TREEVIEW), 0, 0, rc.right-rc.left, rc.bottom-rc.top, TRUE);
}

static void GetMinMaxInfo(MINMAXINFO* mmi)
{
	mmi->ptMinTrackSize.x = 200;
	mmi->ptMinTrackSize.y = 150;
}

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	GUI* gui = (GUI*)::GetWindowLongPtr(hwnd, DWLP_USER);
	if (gui && gui->onWindowsMessage(hwnd, message, wp, lp)) {
		return TRUE;
	}
	switch (message) {
		case WM_GETMINMAXINFO:
			GetMinMaxInfo((MINMAXINFO*)lp);
			break;
		case WM_SIZE:
			AdjustSize(hwnd);
			break;
		case WM_INITDIALOG:
			AdjustSize(hwnd);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

} //Odin
