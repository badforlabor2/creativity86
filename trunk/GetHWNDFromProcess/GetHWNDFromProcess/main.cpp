#include<windows.h>
#include<tlhelp32.h>
#include<iostream>
using namespace std;

HWND nhWnd[10];
int count;

BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwID ;

	GetWindowThreadProcessId(hwnd, &dwID) ;

	if (dwID == (DWORD)lParam)// && IsWindowVisible(hwnd) && GetParent(hwnd) == NULL)
	{
		nhWnd[count++] = hwnd;
	}

	return (count == 10) ? false : true;
}

void GetProcessTopWindows(DWORD dwPID)
{
	count = 0;
	if(EnumWindows((WNDENUMPROC)EnumWndProc, (LPARAM)dwPID)){
		for(int i=0; i<count; i++)
			std::cout<<"---------:"<<nhWnd[i]<<endl;
	}
}
void getProcessIDs()
{
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 ps;
	ps.dwSize = sizeof(PROCESSENTRY32);
	BOOL bo;
	bo = ::Process32First(handle, &ps);
	while(bo){
		std::cout<<"you got a pid:"<<ps.th32ProcessID<<endl;
		GetProcessTopWindows(ps.th32ProcessID);
		bo = ::Process32Next(handle, &ps);
	}
	::CloseHandle(handle);
}

int main()
{
	int pid=0;
	getProcessIDs();
	cin>>pid;

	GetProcessTopWindows(pid);
	return 0;
}