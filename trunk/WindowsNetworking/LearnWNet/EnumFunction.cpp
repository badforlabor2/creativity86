#include <windows.h>
#include <iostream>
using namespace std;
#include <winnetwk.h>

#pragma comment(lib, "Mpr.lib");	//系统库，必须调用,WNetOpenEnum的函数要用

bool enumFunction();

int main()
{
	enumFunction();

	return 0;
}

bool enumFunction()
{
	HANDLE hEnum;
	DWORD dwResult, cbBuffer = 16384, cEntries;
	LPNETRESOURCE lpnrLocal;
	dwResult = WNetOpenEnum(RESOURCE_GLOBALNET,
		RESOURCETYPE_ANY,
		0,
		NULL,
		&hEnum);
	if(dwResult != NO_ERROR){
		return false;
	}
	lpnrLocal = (LPNETRESOURCE)GlobalAlloc(GPTR, cbBuffer);
	if(lpnrLocal == NULL){
		return false;
	}
	do{
		ZeroMemory(lpnrLocal, cbBuffer);
		dwResult = WNetEnumResource(hEnum,
			&cEntries,
			lpnrLocal,
			&cbBuffer);
		if(dwResult == NO_ERROR){
			for(int i = 0; i < int(cEntries); i++){
				cout<<lpnrLocal[i].lpLocalName<<endl;
			}
		}
	}while(dwResult != ERROR_NO_MORE_ITEMS);
	WNetCloseEnum(hEnum);
	return true;
}