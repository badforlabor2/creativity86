#include <windows.h>
#include <windowsX.h>
#include <assert.h>
#include <tchar.h>
#include <iostream>
using namespace std;
#include "../HookDLL/HookDll.h"

#pragma comment(lib, "../debug/HookDll.lib")

int main()
{
	HWND hwnd = FindWindow(TEXT("pdgwindow"), TEXT("ˮ�Q��Online Release 31038983 1.2.78.20090711 [��ͨ����] ʯ���� ���µ���ý��"));
	if(FindWindow(TEXT("pdgwindow"), TEXT("ˮ�Q��Online Release 31038983 1.2.78.20090711 [��ͨ����] ʯ���� ���µ���ý��")))
		cout<<"find window"<<endl;
	assert(IsWindow(hwnd));

	setCalcHook(GetWindowThreadProcessId(hwnd, NULL), hwnd);
	SendMessage(hwnd, WM_KEYDOWN, 0x41, 0);
	cout<<"abcde"<<endl;
	system("pause");
	setCalcHook(0, 0);
	return 0;
}
