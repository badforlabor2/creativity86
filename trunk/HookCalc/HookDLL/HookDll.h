
#ifndef HOOKDLL
#define HOOKDLL extern "C" _declspec(dllimport)
#endif


HOOKDLL bool setCalcHook(DWORD dwThreadID, HWND hwnd);