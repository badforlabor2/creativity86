#include <windows.h>
#include <assert.h>
#define HOOKDLL extern "C" _declspec(dllexport)
#include "HookDll.h"
#include <iostream>
using namespace std;

#pragma data_seg("Shared")
HHOOK g_hhook = NULL;
DWORD g_dwThreadID = 0;
#pragma data_seg()

#pragma comment(linker, "/section:Shared,rws")

HINSTANCE g_hinst = NULL;
HWND	  g_hwnd = NULL;
LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID fImpLoad) {
	switch(fdwReason){
		case DLL_PROCESS_ATTACH:
			g_hinst = hInstance;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;	
	}
	return TRUE;
}

bool setCalcHook(DWORD dwThreadID, HWND hwnd)
{
	bool bOK = false;

	if(dwThreadID != 0){
		assert(g_hhook == NULL);
		g_dwThreadID = GetCurrentThreadId();
		
		g_hhook = SetWindowsHookEx(WH_KEYBOARD, GetMsgProc, g_hinst, dwThreadID);
		g_hwnd = hwnd;
		bOK = (g_hhook != NULL);
		if(bOK){
			//bOK = PostThreadMessage(dwThreadID, WM_NULL, 0, 0);
			BYTE keyState[256];
			GetKeyboardState(keyState);
			//keyState[18] = 1;
			//keyState[0x41] = !keyState[0x41];
			//SetKeyboardState(keyState);
			//SHORT a = GetKeyState('A');
			//a = GetKeyState(VK_MENU);
			//cout<<a<<endl;
		}
	}else{
		assert(g_hhook != NULL);
		bOK = UnhookWindowsHookEx(g_hhook);
		g_hhook = NULL;
	}
	
	return bOK;
}

LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(((DWORD)lParam&0x80000000) && (HC_ACTION==nCode)){
		switch(wParam){
			case '1':
				BYTE keyState[256];
				GetKeyboardState(keyState);
				keyState[VK_MENU] = 0x80;
				SetKeyboardState(keyState);
				//SendMessage(g_hwnd, WM_KEYDOWN, 'a', 0);
				//Sleep(100);
				SendMessage(g_hwnd, WM_KEYUP, 0x41, 0);
				Sleep(100);
				//SendMessage(g_hwnd, WM_SYSKEYUP, VK_MENU, 0);
				Sleep(300);
				if(GetKeyState(VK_MENU))
					MessageBox(NULL, TEXT("B"), TEXT("A"), MB_OK);		
				//keyState[VK_MENU] = 0;
				//SetKeyboardState(keyState);
				break;
			case '2':
				cout<<GetKeyState(VK_MENU)<<endl;
				MessageBox(NULL, TEXT("A"), NULL, MB_OK);
				break;
			case VK_MENU:
				MessageBox(NULL, TEXT("MENU"), TEXT("MENU"), MB_OK);
				break;
			case 'a':
				if(GetKeyState(VK_MENU))
					MessageBox(NULL, TEXT("MENU"), NULL, MB_OK);
				break;

		}	
	}
		/*else
		MessageBox(NULL, LPCWSTR(nCode), NULL, MB_OK);*/
	//return CallNextHookEx(g_hhook, nCode, wParam, lParam);
	return 0;
}  