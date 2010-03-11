#ifndef MESSAGEBOXJUNIOR_H
#define MESSAGEBOXJUNIOR_H
#include <TCHAR.h>
#include <windows.h>
#include "resource.h"
class MessageBoxJunior
{
public:
	//MessageBoxJunior()
	//{
	//	Title[0] = '\0';
	//	WarningInfo[0] = '\0';
	//	WaitTime = 0;
	//}
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
	void Show(HINSTANCE hInstance, HWND parent, HMODULE hMod)
	{
		int ret;
		//FindResource()
		HGLOBAL hRes;
		hMod = ::GetModuleHandle("msg.dll");
		hRes = ::LoadResource(hMod, ::FindResource(hMod, TEXT("#101"), RT_DIALOG));
		if(hRes == NULL){
			//ret = GetLastError();
			return;
		}
		ret = ::DialogBoxIndirectParam(hInstance, (LPCDLGTEMPLATE)hRes, parent, (DLGPROC)DlgProc, (LPARAM)this);
		//if(ret == -1){
		//	ret = GetLastError();
		//}
		::FreeResource(hRes);
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
			{
				MessageBoxJunior *msgBox = (MessageBoxJunior *)lParam;
				msgBox->hSelf = hwnd;
				::SetTimer(hwnd, 0, msgBox->WaitTime * 1000, (TIMERPROC)OnTimer);
				::SetWindowText(hwnd, msgBox->Title);
				::SetDlgItemText(hwnd, -1, msgBox->WarningInfo);
				//::EnableWindow(::GetDlgItem(hwnd, IDOK), FALSE);
				::EnableMenuItem(::GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
				return TRUE;
			}
			case WM_COMMAND:
			{
				switch (wParam)
				{
					case IDOK:
						::EndDialog(hwnd, IDOK);
						return TRUE;
					default:
						break;
				}
				break;
			}
			case WM_CLOSE:
				DestroyWindow(hwnd);
				return TRUE;
			default:
				break;
		}
		return FALSE;
	}

private:
	TCHAR Title[512];
	TCHAR WarningInfo[512];
	int WaitTime;
	HWND hSelf;
};

#endif