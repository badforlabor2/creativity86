/************************************************************************/
/* ��Ҫʵ�ֿ��ӻ���ʾ��־�����ұ�����־���ļ���                         */
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