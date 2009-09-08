#include <windows.h>
#include <iostream>
using namespace std;
#include <winnetwk.h>

#pragma comment(lib, "Mpr.lib");	//ϵͳ�⣬�������,WNetOpenEnum�ĺ���Ҫ��

bool enumFunction();
bool enumFunction2();

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

bool enumFunction2()
{
	DWORD dwResult;
	NETRESOURCE *Res1, *Res2, *Res3;
	HANDLE hEnum;
	LPVOID buf1 = new char [4096],
		buf2 = new char [4096],
		buf3 = new char [4096];
	dwResult = WNetOpenEnum(RESOURCE_GLOBALNET,
		RESOURCETYPE_ANY,
		RESOURCEUSAGE_CONTAINER,
		NULL,
		&hEnum);
	DWORD dwSize1=4096, dwCount1=-1;
	dwResult = WNetEnumResource(hEnum, &dwCount1, buf, &dwSize1);
	Res1 = (NETRESOURCE *)buf1;
	for(DWORD i=0; i<dwCount1; i++, Res1++){
		DWORD dwSize2=4096, dwCount2=-1;
		dwResult = WNetOpenEnum(RESOURCE_GLOBALNET,
			RESOURCETYPE_ANY,
			RESOURCEUSAGE_CONTAINER,
			Res,
			&hEnum);
		dwCount2=-1;
		///ע�⣬dwCount��ֵҪΪ-1��dwSize2Ҫ�㹻���������Ҫ������
		dwResult = WNetEnumResource(hEnum, &dwCount2, buf2, &dwSize2);
		Res2 = (NETRESOURCE *)buf2;
		for(DWORD j=0; j<dwCount2; j++, Res2++){
			DWORD dwCount3=0xffffffff, dwSize3=4096;
			dwResult = WNetOpenEnum(RESOURCE_GLOBALNET,
				RESOURCETYPE_ANY,
				RESOURCEUSAGE_CONTAINER,
				Res2,
				&hEnum);
			dwResult = WNetEnumResource(hEnum, 
				&dwCount3, buf3, &dwSize3);
			Res3 = (NETRESOURCE *)buf3;
			for(DWORD k=0; k<dwCount3; k++, Res3++){
				cout<<Res3->lpRemoteName<<endl;
			}
		}
	}
	delete Res1;
	delete Res2;
	delete Res3;
	WNetCloseEnum(hEnum);
	return true;
}
