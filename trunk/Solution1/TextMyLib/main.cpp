#include "resource.h"
#include <windows.h>
#include <iostream>
using namespace std;

VOID CALLBACK OnTimer(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	::KillTimer(hwnd, int(wParam));
	::DestroyWindow(hwnd);
}
LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
		case WM_PAINT:
			//
			break;
		case WM_INITDIALOG:
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return TRUE;
		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;
		default:
			break;

	}
	return FALSE;
}
//警告信息，停留时间，标题
void SetWarning(const TCHAR *warningInfo, int sec, const TCHAR *title = TEXT("警告"))
{
	MSG msg;
	HWND hwnd = ::CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);

	::SetDlgItemText(hwnd, -1, warningInfo);
	::ShowWindow(hwnd, TRUE);

	::SetWindowText(hwnd, title);
	::SetTimer(hwnd, 2, sec*1000, (TIMERPROC)OnTimer);

	::EnableWindow(::GetDlgItem(hwnd, IDOK), FALSE);
	::EnableMenuItem(::GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED) ;
	RECT rect;
	::GetWindowRect(hwnd, &rect);
	int xScreen = GetSystemMetrics(SM_CXSCREEN);
	int yScreen = GetSystemMetrics(SM_CYSCREEN);
	int wClient = rect.right - rect.left;
	int hClient = rect.bottom - rect.top;
	::SetWindowPos(hwnd, 
		HWND_NOTOPMOST,
		xScreen/2-wClient/2, 
		yScreen/2-hClient/2,
		wClient,
		hClient,
		SWP_SHOWWINDOW
		);
	::UpdateWindow(hwnd);
	while(::GetMessage(&msg, NULL, 0, 0)){
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);		
	}

}

LRESULT CALLBACK DialogProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
		case WM_PAINT:
			//
			break;
		case WM_INITDIALOG:
			SetTimer(hwnd, 2, 10000, TIMERPROC(OnTimer));
			::SetWindowText(hwnd, TEXT("警告"));
			::EnableWindow(::GetDlgItem(hwnd, IDOK), FALSE);
			::EnableMenuItem(::GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED) ;
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			return TRUE;
		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;
		default:
			break;

	}
	return FALSE;
}
void SetWarningByModel(IN HINSTANCE hInstance)
{
	::DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), GetActiveWindow(), (DLGPROC)DialogProc2);
}
int main()
{
//INT WINAPI WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
//{
	cout<<"abc--------------"<<endl;

	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	SetWarningByModel(NULL);
//	SetWarning(TEXT("9999999999999999"), 10/*, TEXT("---title-----!!Warning!!----------")*/);
	cout<<"abcde"<<endl;
	system("pause");
	return 0;
}