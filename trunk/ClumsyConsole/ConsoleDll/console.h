/************************************************************************/
/* liubo,a clumsy console for dubug                                     */
/************************************************************************/

#ifndef CONSOLEDLL
#define CONSOLEDLL extern "C" _declspec(dllimport)
#endif

CONSOLEDLL void CLOG(char *msg);
CONSOLEDLL void WLOG(wchar_t *msg);