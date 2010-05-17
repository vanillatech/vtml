#include "gui.h"
#include "resource.h"
#include "main_frame.h"

namespace Odin
{

static INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);

HWND CreateMainFrame(GUI& gui)
{
	HWND hwnd = ::CreateDialog(
		::GetModuleHandle(0), MAKEINTRESOURCE(IDD_MAIN_FRAME), 0, DialogProc
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
	int width = rc.right-rc.left;
	int height = rc.bottom-rc.top;

	int border = 3;
	
	int x = border;
	int y = border;
	int w = width - 100;
	int h = 24;
	::MoveWindow(::GetDlgItem(hwnd, IDC_EDIT_INPUT), x, y, w, h, TRUE);

	x = x + w + border;
	w = width - border - x;
	::MoveWindow(::GetDlgItem(hwnd, IDC_BUTTON_GO), x, y, w, h, TRUE);

	y = y + h + border;
	x = 0;
	w = width;
	h = height - y - 20;
	::MoveWindow(::GetDlgItem(hwnd, IDC_LIST_LOG), x, y, w, h, TRUE);

	x = width - 120;
	y = height - 20;
	w = width - x - border;
	h = 20;
	::MoveWindow(::GetDlgItem(hwnd, IDC_CHECKBOX_SHOW_TREEVIEW), x, y, w, h, TRUE);
}

static void GetMinMaxInfo(MINMAXINFO* mmi)
{
	mmi->ptMinTrackSize.x = 400;
	mmi->ptMinTrackSize.y = 300;
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
			::SetFocus(::GetDlgItem(hwnd, IDC_EDIT_INPUT));
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

} //Odin
