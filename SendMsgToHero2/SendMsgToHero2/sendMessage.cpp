#include<windows.h>
#include<iostream>
using namespace std;
			//��������һ��windows���̡����Ҫ���win32����̨���̣�����ֱ��ʹ��int main()
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR lpCmdLine, int nShowCmd){						
	std::cout<<"�ڽ�����5�������л�����Ϸ���ڣ��Ա��һ����ȷ����Ϸ����"<<endl;
	::Sleep(5000);
	std::cout<<"��û���ھ��:"<<endl;
	HWND hGameWnd = ::GetForegroundWindow();
	TCHAR gameName[80];
	if(GetClassName(hGameWnd, gameName, 80)){
		std::cout<<"��õ���Ϸ���ڵ�����Ϊ��"<<gameName;
	}
	int i;
	while(true){
		i=0;
		if(GetClassName(hGameWnd, gameName, 80)){
			while(i++<1000){
				//std::cout<<"��õ���Ϸ���ڵ�����Ϊ��"<<gameName;
				//SendMessage(hGameWnd, WM_SYSKEYDOWN, VK_MENU, 540540929);	//����alt��
				//SendMessage(hGameWnd, WM_SYSKEYDOWN, VK_MENU, 1614282753);	//��סalt��
				SendMessage(hGameWnd, WM_KEYDOWN, 0x41, 0);		//����a��
				SendMessage(hGameWnd, WM_KEYUP, 0x41, 0);
				//SendMessage(hGameWnd, WM_SYSKEYUP, VK_MENU, -1070078783);	//����alt��
				::Sleep(600);
			}
		}
	}
	return 0;
}