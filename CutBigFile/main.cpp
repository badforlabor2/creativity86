/*
	ѧϰʹ��File mapping.

	--- CreateFile
	--- CreateFileMapping
	--- MapViewOfFile
*/

#include <windows.h>
#include <iostream>
#include <TCHAR.h>

//�ļ���
const TCHAR* CONS_FileName = TEXT("D:\\GS-Crash����\\111213.ucc.7785.log-cut");
const TCHAR* CONS_SuffixCut = TEXT("-cut");
const TCHAR* CONS_SuffixSplit = TEXT("-split");
//��ȡ�ļ��ĺ�40M
const DWORD CONS_FileCutSize = 20 * 1024 * 1024;

#define CHECK_RETV(exp, ret) {if(!(exp)){ std::cout<<"Invalid '"<<#exp<<"' on "__FILE__<<"  "<<__LINE__<<std::endl; return ret;}}

int CutFile(const TCHAR* pFileName, DWORD CutSize)
{
	//�����׺һ������ôֱ�ӷ���
	if(_tcscmp(pFileName+_tcslen(pFileName)-_tcslen(CONS_SuffixCut), CONS_SuffixCut) == 0)
		return -1;

	HANDLE hFile = CreateFile(pFileName, FILE_ALL_ACCESS, FILE_SHARE_READ, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	CHECK_RETV(hFile != INVALID_HANDLE_VALUE, 0);

	//����ļ��Ĵ�С���������0����ô��map file�ķ���
	DWORD dzSize = GetFileSize(hFile, NULL);
	CHECK_RETV(dzSize > 0, 0);

	HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dzSize, NULL);
	CHECK_RETV(hMap != NULL, 0);

	//����ƫ��
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);

	//����ü��ĳߴ�С��ʣ��ߴ�
	DWORD nStartPos = dzSize < CutSize ? 0 : dzSize - CutSize;
	DWORD nCutSize = dzSize - nStartPos < CutSize ? dzSize : CutSize;
	DWORD nStart = nStartPos/SysInfo.dwAllocationGranularity*SysInfo.dwAllocationGranularity;
	DWORD nPerBlock = (nCutSize+SysInfo.dwAllocationGranularity)/SysInfo.dwAllocationGranularity*SysInfo.dwAllocationGranularity;

	void* pMapView = MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 
							nStart, 
							dzSize - nStart < nPerBlock ? 0 : nPerBlock);
	CHECK_RETV(pMapView != NULL, 0);

	char* pData = (char*)pMapView + (nStartPos - nStart);
	DWORD dwDataSize = nCutSize;

	//������д�뵽�ļ���
	TCHAR szTargetFileName[1024] = {0};
	wsprintf(szTargetFileName, TEXT("%s%s"), pFileName, CONS_SuffixCut);
	HANDLE hWriteFile = CreateFile(szTargetFileName, FILE_GENERIC_WRITE, 0, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CHECK_RETV(hWriteFile!=INVALID_HANDLE_VALUE, 0);

	WriteFile(hWriteFile, pData, dwDataSize, &dwDataSize, NULL);

	CloseHandle(hWriteFile);
	UnmapViewOfFile(pMapView);
	CloseHandle(hFile);

	return 0;
}

int SplitFile(const TCHAR* pFileName, DWORD CutSize, DWORD StartPos)
{
	//�����׺һ������ôֱ�ӷ���
	if(_tcscmp(pFileName+_tcslen(pFileName)-_tcslen(CONS_SuffixCut), CONS_SuffixCut) == 0)
		return -1;

	HANDLE hFile = CreateFile(pFileName, FILE_ALL_ACCESS, FILE_SHARE_READ, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	CHECK_RETV(hFile != INVALID_HANDLE_VALUE, 0);

	//����ļ��Ĵ�С���������0����ô��map file�ķ���
	DWORD dzSize = GetFileSize(hFile, NULL);
	CHECK_RETV(dzSize > 0, 0);

	HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dzSize, NULL);
	CHECK_RETV(hMap != NULL, 0);

	//����ƫ��
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);

	//�ָ�
	int cnt = 0;
	while(dzSize > CutSize*cnt)
	{
		DWORD nStartPos = StartPos+CutSize*cnt;
		DWORD nCutSize = dzSize - nStartPos < CutSize ? dzSize-nStartPos : CutSize;
		DWORD nStart = (nStartPos)/SysInfo.dwAllocationGranularity*SysInfo.dwAllocationGranularity;
		DWORD nPerBlock = (nCutSize+SysInfo.dwAllocationGranularity)/SysInfo.dwAllocationGranularity*SysInfo.dwAllocationGranularity;

		void* pMapView = MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 
								nStart, 
								dzSize - nStart < nPerBlock ? 0 : nPerBlock);
		CHECK_RETV(pMapView != NULL, 0);

		char* pData = (char*)pMapView + nStartPos - nStart;
		DWORD dwDataSize = nCutSize;

		//������д�뵽�ļ���
		TCHAR szTargetFileName[1024] = {0};
		wsprintf(szTargetFileName, TEXT("%s%s.%d"), pFileName, CONS_SuffixSplit, cnt+1);
		HANDLE hWriteFile = CreateFile(szTargetFileName, FILE_GENERIC_WRITE, 0, NULL,
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		CHECK_RETV(hWriteFile!=INVALID_HANDLE_VALUE, 0);

		WriteFile(hWriteFile, pData, dwDataSize, &dwDataSize, NULL);

		CloseHandle(hWriteFile);
		UnmapViewOfFile(pMapView);

		cnt++;
	}
	CloseHandle(hFile);

	return 0;
}
int main(int argc, char** argv)
{
	
	//��������
	TCHAR szCmd[128] = {0};
	int split = -1;	//Ĭ�����и�
	if(argc > 1)
	{
		::MultiByteToWideChar(CP_ACP, 0, argv[1], -1, szCmd, 128);
		if(argc > 2)
			split = atoi(argv[2]);
	}
	else
	{
		_tcscpy(szCmd, TEXT("*.log"));
	}

	TCHAR szRootPath[1024] = {0};
	TCHAR szFullPath[1024] = {0};
	GetCurrentDirectory(1024, szRootPath);

	WIN32_FIND_DATA fd;
	//_tcscpy(szFullPath, szRootPath);
	//_tcscat(szFullPath, TEXT("\\"));
	//_tcscat(szFullPath, szCmd);
	//_tcscpy(szCmd, szFullPath);
	HANDLE hFind = FindFirstFile(szCmd, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(_tcscmp(fd.cFileName, TEXT(".")) == 0
					|| _tcscmp(fd.cFileName, TEXT("..")) == 0)
				continue;
			
			_tcscpy(szFullPath, szRootPath);
			_tcscat(szFullPath, TEXT("\\"));
			_tcscat(szFullPath, fd.cFileName);
			int ret = 0;
			if(split == -1)
				ret = CutFile(szFullPath, CONS_FileCutSize);
			else
				ret = SplitFile(szFullPath, CONS_FileCutSize, split);
			if(ret != -1)
				wprintf(TEXT("%s\n"), szFullPath);
			//std::cout<<std::endl;
			//hFind = ;
		}
		while(FindNextFile(hFind, &fd));
	}



	system("pause");
	return 0;
}


