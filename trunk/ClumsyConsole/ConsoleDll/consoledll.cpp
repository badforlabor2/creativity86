/************************************************************************/
/* 主要实现可视化显示日志，并且保存日志到文件中                         */
/************************************************************************/
#include <windows.h>
#include <iostream>
using namespace std;

#define CONSOLEDLL extern "C" _declspec(dllexport)
#include "console.h"

void CLOG(char *msg)
{
	;
}
void WLOG(wchar_t *msg)
{
	;
}