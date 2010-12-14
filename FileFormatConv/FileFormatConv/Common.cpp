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

#include <windows.h>
#include "Common.h"

WcharMbcsConvertor * WcharMbcsConvertor::_pSelf = new WcharMbcsConvertor;

const wchar_t * WcharMbcsConvertor::char2wchar(const char * mbcs2Convert, UINT codepage)
{
	if (!_wideCharStr)
	{
		_wideCharStr = new wchar_t[initSize];
		_wideCharAllocLen = initSize;
	}

	int len = MultiByteToWideChar(codepage, 0, mbcs2Convert, -1, _wideCharStr, 0);
	if (len > 0)
	{
		if ((size_t)len > _wideCharAllocLen)
		{
			delete [] _wideCharStr;
			_wideCharAllocLen = len;
			_wideCharStr = new wchar_t[_wideCharAllocLen];
		}
		MultiByteToWideChar(codepage, 0, mbcs2Convert, -1, _wideCharStr, len);
	}
	else
		_wideCharStr[0] = 0;

	return _wideCharStr;
}

// "mstart" and "mend" are pointers to indexes in mbcs2Convert,
// which are converted to the corresponding indexes in the returned wchar_t string.
const wchar_t * WcharMbcsConvertor::char2wchar(const char * mbcs2Convert, UINT codepage, int *mstart, int *mend)
{
	if (!_wideCharStr)
	{
		_wideCharStr = new wchar_t[initSize];
		_wideCharAllocLen = initSize;
	}

	int len = MultiByteToWideChar(codepage, 0, mbcs2Convert, -1, _wideCharStr, 0);
	if (len > 0)
	{
		if (len > int(_wideCharAllocLen))
		{
			delete [] _wideCharStr;
			_wideCharAllocLen = len;
			_wideCharStr = new wchar_t[_wideCharAllocLen];
		}
		len = MultiByteToWideChar(codepage, 0, mbcs2Convert, -1, _wideCharStr, len);

		if ((size_t)*mstart < strlen(mbcs2Convert) && (size_t)*mend <= strlen(mbcs2Convert))
		{
			*mstart = MultiByteToWideChar(codepage, 0, mbcs2Convert, *mstart, _wideCharStr, 0);
			*mend   = MultiByteToWideChar(codepage, 0, mbcs2Convert, *mend, _wideCharStr, 0);
			if (*mstart >= len || *mend >= len)
			{
				*mstart = 0;
				*mend = 0;
			}
		}
	}
	else
	{
		_wideCharStr[0] = 0;
		*mstart = 0;
		*mend = 0;
	}
	return _wideCharStr;
} 

const char * WcharMbcsConvertor::wchar2char(const wchar_t * wcharStr2Convert, UINT codepage) 
{
	if (!_multiByteStr)
	{
		_multiByteStr = new char[initSize];
		_multiByteAllocLen = initSize;
	}

	int len = WideCharToMultiByte(codepage, 0, wcharStr2Convert, -1, _multiByteStr, 0, NULL, NULL);
	if (len > 0)
	{
		if ((size_t)len > _multiByteAllocLen)
		{
			delete [] _multiByteStr;
			_multiByteAllocLen = len;
			_multiByteStr = new char[_multiByteAllocLen];
		}
		WideCharToMultiByte(codepage, 0, wcharStr2Convert, -1, _multiByteStr, len, NULL, NULL);
	}
	else
		_multiByteStr[0] = 0;

	return _multiByteStr;
}

const char * WcharMbcsConvertor::wchar2char(const wchar_t * wcharStr2Convert, UINT codepage, long *mstart, long *mend) 
{
	if (!_multiByteStr)
	{
		_multiByteStr = new char[initSize];
		_multiByteAllocLen = initSize;
	}

	int len = WideCharToMultiByte(codepage, 0, wcharStr2Convert, -1, _multiByteStr, 0, NULL, NULL);
	if (len > 0)
	{
		if ((size_t)len > _multiByteAllocLen)
		{
			delete [] _multiByteStr;
			_multiByteAllocLen = len;
			_multiByteStr = new char[_multiByteAllocLen];
		}
		len = WideCharToMultiByte(codepage, 0, wcharStr2Convert, -1, _multiByteStr, len, NULL, NULL); // not needed?

        if ((int)*mstart < lstrlenW(wcharStr2Convert) && (int)*mend < lstrlenW(wcharStr2Convert))
        {
			*mstart = WideCharToMultiByte(codepage, 0, wcharStr2Convert, *mstart, _multiByteStr, 0, NULL, NULL);
			*mend = WideCharToMultiByte(codepage, 0, wcharStr2Convert, *mend, _multiByteStr, 0, NULL, NULL);
			if (*mstart >= len || *mend >= len)
			{
				*mstart = 0;
				*mend = 0;
			}
		}
	}
	else
		_multiByteStr[0] = 0;

	return _multiByteStr;
}

