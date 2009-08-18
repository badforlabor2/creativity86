//**********************************************************
//
//�����ҵĵ��Ի��߸���������λ��
//
//**********************************************************
#pragma

#include<windows.h>
#include<iostream>
#include<tchar.h>
using namespace std;

void traverseDir(PCTSTR dir, PCTSTR );
void log(PCTSTR e, ...);
int main(int argc, char *argv[])
{
	//GetCurrentDirectory();
	traverseDir(TEXT("E:\\liubo\\�������"), TEXT("  "));
	system("pause");
	return 0;
}

void traverseDir(PCTSTR dir, PCTSTR pad)
{
	WIN32_FIND_DATA data;
	TCHAR dire[MAX_PATH];
	TCHAR pade[MAX_PATH];
	//strcpy(dire, dir);
	_tcscpy_s(dire, MAX_PATH, dir);
	_tcscat_s(dire, MAX_PATH, TEXT("\\*"));
	_tcscpy_s(pade, MAX_PATH, pad);//��Ŀ¼����padֵ
	_tcscat_s(pade, MAX_PATH, TEXT("   "));
	HANDLE hTraverse = FindFirstFile(dire, &data);
	if(hTraverse == INVALID_HANDLE_VALUE){
		log(TEXT("Can't find directory"), dire);
		return;
	}
	do{
		switch(data.dwFileAttributes){
			case FILE_ATTRIBUTE_DIRECTORY:
				if(_tcscmp(data.cFileName, TEXT(".")) == 0
					|| _tcscmp(data.cFileName, TEXT("..")) == 0)	//�ų���ǰĿ¼�͸�Ŀ¼
					break;
				//�鵽����Ŀ¼
				TCHAR dir2[MAX_PATH];
				_tcscpy_s(dir2, MAX_PATH, dir);
				_tcscat_s(dir2, MAX_PATH, TEXT("\\"));
				_tcscat_s(dir2, MAX_PATH, data.cFileName);
				log(pade, dir2, TEXT(":"));
				traverseDir(dir2, pade);	//�ݹ�����Ŀ¼
				break;
			default:
				//�鵽�����ļ�
				log(pade, dire, TEXT("\\"), data.cFileName);
				break;
		}
	}while(FindNextFile(hTraverse, &data));
}

void log(PCTSTR e, ...)
{
	TCHAR chMsg[MAX_PATH];
	va_list pArg;
	
	va_start(pArg, e);
	wsprintf(chMsg, e, pArg);	//�������б�pArg��Ϊ�ַ����νӵ�����
	va_end(pArg);

	cout<<chMsg<<endl;
}