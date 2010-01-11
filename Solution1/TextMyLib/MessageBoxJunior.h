#ifndef MESSAGEBOXJUNIOR_H
#define MESSAGEBOXJUNIOR_H
#include <TCHAR.h>
#include <windows.h>
#include "resource.h"
class MessageBoxJunior
{
public:
	MessageBoxJunior(TCHAR *title, TCHAR *warnInfo, int waitTime)
	{
		if(_tcslen(title) > 511)
			title[511] = '\0';
		_tcscpy(Title, title);
		if(_tcslen(warnInfo) > 511)
			warnInfo[511] = '\0';
		_tcscpy(WarningInfo, warnInfo);
		WaitTime = waitTime;
	}
	void Show(HINSTANCE hInstance, HWND parent)
	{
		::DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), parent, (DLGPROC)DlgProc);
	}
	static void CALLBACK OnTimer(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		::KillTimer(hwnd, wParam);
		::DestroyWindow(hwnd);
	}
	static LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg){
			case WM_INITDIALOG:
				::SetTimer(hwnd, 0, WaitTime * 1000, (TIMERPROC)OnTimer);
				::SetWindowText(hwnd, Title);
				::SetDlgItemText(hwnd, -1, WarningInfo);
				::EnableWindow(::GetDlgItem(hwnd, IDOK), FALSE);
				::EnableMenuItem(::GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
				break;
			case WM_CLOSE:
				DestroyWindow(hwnd);
				return TRUE;
			default:
				break;
		}
		return FALSE;
	}

private:
	static TCHAR Title[512];
	static TCHAR WarningInfo[512];
	static int WaitTime;
};

#endif