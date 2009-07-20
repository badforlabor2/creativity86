#include<windows.h>
#include<iostream>
using namespace std;
			//这样算是一个windows工程。如果要变成win32控制台工程，可以直接使用int main()
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR lpCmdLine, int nShowCmd){						
	std::cout<<"在接下来5秒内请切换到游戏窗口，以便我获得正确的游戏窗口"<<endl;
	::Sleep(5000);
	std::cout<<"获得活动窗口句柄:"<<endl;
	HWND hGameWnd = ::GetForegroundWindow();
	TCHAR gameName[80];
	if(GetClassName(hGameWnd, gameName, 80)){
		std::cout<<"获得的游戏窗口的类名为："<<gameName;
	}
	int i;
	while(true){
		i=0;
		if(GetClassName(hGameWnd, gameName, 80)){
			while(i++<1000){
				//std::cout<<"获得的游戏窗口的类名为："<<gameName;
				//SendMessage(hGameWnd, WM_SYSKEYDOWN, VK_MENU, 540540929);	//按下alt键
				//SendMessage(hGameWnd, WM_SYSKEYDOWN, VK_MENU, 1614282753);	//按住alt键
				SendMessage(hGameWnd, WM_KEYDOWN, 0x41, 0);		//按下a键
				SendMessage(hGameWnd, WM_KEYUP, 0x41, 0);
				//SendMessage(hGameWnd, WM_SYSKEYUP, VK_MENU, -1070078783);	//按上alt键
				::Sleep(600);
			}
		}
	}
	return 0;
}