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
	HWND hwnd = FindWindow(TEXT("pdgwindow"), TEXT("水浒Q传Online Release 31038983 1.2.78.20090711 [网通五区] 石将军 天下第三媒婆"));
	if(FindWindow(TEXT("pdgwindow"), TEXT("水浒Q传Online Release 31038983 1.2.78.20090711 [网通五区] 石将军 天下第三媒婆")))
		cout<<"find window"<<endl;
	assert(IsWindow(hwnd));

	setCalcHook(GetWindowThreadProcessId(hwnd, NULL), hwnd);
	SendMessage(hwnd, WM_KEYDOWN, 0x41, 0);
	cout<<"abcde"<<endl;
	system("pause");
	setCalcHook(0, 0);
	return 0;
}
