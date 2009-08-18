//**********************************************************
//
//遍历我的电脑或者给定的任意位置
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
	traverseDir(TEXT("E:\\liubo\\工作相关"), TEXT("  "));
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
	_tcscpy_s(pade, MAX_PATH, pad);//子目录扩成pad值
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
					|| _tcscmp(data.cFileName, TEXT("..")) == 0)	//排除当前目录和父目录
					break;
				//查到的是目录
				TCHAR dir2[MAX_PATH];
				_tcscpy_s(dir2, MAX_PATH, dir);
				_tcscat_s(dir2, MAX_PATH, TEXT("\\"));
				_tcscat_s(dir2, MAX_PATH, data.cFileName);
				log(pade, dir2, TEXT(":"));
				traverseDir(dir2, pade);	//递归搜索目录
				break;
			default:
				//查到的是文件
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
	wsprintf(chMsg, e, pArg);	//将参数列表pArg作为字符串衔接到后面
	va_end(pArg);

	cout<<chMsg<<endl;
}