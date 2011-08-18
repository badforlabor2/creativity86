//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef M30_IDE_COMMUN_H
#define M30_IDE_COMMUN_H

#ifdef UNICODE
	#define NppMainEntry wWinMain
	#define generic_strtol wcstol
	#define generic_strncpy wcsncpy
	#define generic_stricmp wcsicmp
	#define generic_strncmp wcsncmp
	#define generic_strnicmp wcsnicmp
	#define generic_strncat wcsncat
	#define generic_strchr wcschr
	#define generic_atoi _wtoi
	#define generic_itoa _itow
	#define generic_atof _wtof
	#define generic_strtok wcstok
	#define generic_strftime wcsftime
	#define generic_fprintf fwprintf
	#define generic_sscanf swscanf
	#define generic_fopen _wfopen
	#define generic_fgets fgetws
	#define generic_stat _wstat
	#define generic_sprintf swprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESW
#else
	#define NppMainEntry WinMain
	#define generic_strtol strtol
	#define generic_strncpy strncpy
	#define generic_stricmp stricmp
	#define generic_strncmp strncmp
	#define generic_strnicmp strnicmp
	#define generic_strncat strncat
	#define generic_strchr strchr
	#define generic_atoi atoi
	#define generic_itoa itoa
	#define generic_atof atof
	#define generic_strtok strtok
	#define generic_strftime strftime
	#define generic_fprintf fprintf
	#define generic_sscanf sscanf
	#define generic_fopen fopen
	#define generic_fgets fgets
	#define generic_stat _stat
	#define generic_sprintf sprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESA
#endif

class WcharMbcsConvertor {
public:
	static WcharMbcsConvertor * getInstance() {return _pSelf;};
	static void destroyInstance() {delete _pSelf;};

	const wchar_t * char2wchar(const char *mbStr, UINT codepage);
	const wchar_t * char2wchar(const char *mbcs2Convert, UINT codepage, int *mstart, int *mend);
	const char * wchar2char(const wchar_t *wcStr, UINT codepage);
	const char * wchar2char(const wchar_t *wcStr, UINT codepage, long *mstart, long *mend);
	
	const char * encode(UINT fromCodepage, UINT toCodepage, const char *txt2Encode) {
        const wchar_t * strW = char2wchar(txt2Encode, fromCodepage);
        return wchar2char(strW, toCodepage);
    };

protected:
	WcharMbcsConvertor() : _multiByteStr(NULL), _wideCharStr(NULL), _multiByteAllocLen(0), _wideCharAllocLen(0), initSize(1024) {
	};
	~WcharMbcsConvertor() {
		if (_multiByteStr)
			delete [] _multiByteStr;
		if (_wideCharStr)
			delete [] _wideCharStr;
	};
	static WcharMbcsConvertor * _pSelf;

	const int initSize;
	char *_multiByteStr;
	size_t _multiByteAllocLen;
	wchar_t *_wideCharStr;
	size_t _wideCharAllocLen;

private:
	// Since there's no public ctor, we need to void the default assignment operator.
	WcharMbcsConvertor& operator= (const WcharMbcsConvertor&);
	
};

#endif //M30_IDE_COMMUN_H