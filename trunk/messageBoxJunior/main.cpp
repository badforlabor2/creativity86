#include <windows.h>
#include "MessageBoxJunior.h"

extern "C" __declspec(dllexport) void Show(HINSTANCE hInstance, HWND hParent, HMODULE hSelf = NULL)
{
	MessageBoxJunior msg(TEXT("AAAAAAAAAAAAAAAA"), TEXT("BBBBBBBBBBBBBBBBBBBBB"), 10);
	msg.Show(hInstance, hParent, hSelf);
}