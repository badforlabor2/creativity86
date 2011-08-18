// Utf8_16.h
// Copyright (C) 2002 Scott Kirkwood
//
// Permission to use, copy, modify, distribute and sell this code
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies or
// any derived copies.  Scott Kirkwood makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
//
// Notes: Used the UTF information I found at:
//   http://www.cl.cam.ac.uk/~mgk25/unicode.html
////////////////////////////////////////////////////////////////////////////////
// 
// Modificated 2006 Jens Lorenz
// 
// - Clean up the sources
// - Removing UCS-Bug in Utf8_Iter
// - Add convert function in Utf8_16_Write
////////////////////////////////////////////////////////////////////////////////

#ifndef UTF8_16_H


#ifdef _MSC_VER
#pragma warning(disable: 4514) // nreferenced inline function has been removed
#endif

#include <assert.h>
#include <stdio.h>

typedef unsigned char UCHAR;

enum UniMode {uni8Bit=0, uniUTF8=1, uni16BE=2, uni16LE=3, uniCookie=4, uni7Bit=5, uni16BE_NoBOM=6, uni16LE_NoBOM=7, uniEnd};

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







class Utf8_16 {
public:
	typedef unsigned short utf16; // 16 bits
	typedef UCHAR utf8; // 8 bits
	typedef UCHAR ubyte;
	static const utf8 k_Boms[uniEnd][3];
};

// Reads UTF-16 and outputs UTF-8
class Utf16_Iter : public Utf8_16 {
public:
	enum eState {
	    eStart,
	    e2Bytes2,
	    e3Bytes2,
	    e3Bytes3
	};

	Utf16_Iter();
	void reset();
	void set(const ubyte* pBuf, size_t nLen, UniMode eEncoding);
	utf8 get() const { return m_nCur; };
	void operator++();
	eState getState() { return m_eState; };
	operator bool() { return m_pRead <= m_pEnd; };

protected:
	void toStart(); // Put to start state, swap bytes if necessary

protected:
	UniMode m_eEncoding;
	eState m_eState;
	utf8 m_nCur;
	utf16 m_nCur16;
	const ubyte* m_pBuf;
	const ubyte* m_pRead;
	const ubyte* m_pEnd;
};

// Reads UTF-8 and outputs UTF-16
class Utf8_Iter : public Utf8_16 {
public:
	Utf8_Iter();
	void reset();
	void set(const ubyte* pBuf, size_t nLen, UniMode eEncoding);
	utf16 get() const {
#ifdef _DEBUG
		assert(m_eState == eStart);
#endif
		return m_nCur;
	}
	bool canGet() const { return m_eState == eStart; }
	void operator++();
	operator bool() { return m_pRead <= m_pEnd; }

protected:
	void swap();
	void toStart(); // Put to start state, swap bytes if necessary
	enum eState {
	    eStart,
	    e2Bytes_Byte2,
	    e3Bytes_Byte2,
	    e3Bytes_Byte3
	};
protected:
	UniMode m_eEncoding;
	eState m_eState;
	utf16 m_nCur;
	const ubyte* m_pBuf;
	const ubyte* m_pRead;
	const ubyte* m_pEnd;
};

// Reads UTF16 and outputs UTF8
enum u78 {utf8NoBOM=0, ascii7bits=1, ascii8bits=2};
class Utf8_16_Read : public Utf8_16 {
public:
	Utf8_16_Read();
	~Utf8_16_Read();

	size_t convert(char* buf, size_t len);
	char* getNewBuf() { return reinterpret_cast<char *>(m_pNewBuf); }

	UniMode getEncoding() const { return m_eEncoding; }
	size_t calcCurPos(size_t pos);
    static UniMode determineEncoding(const unsigned char *buf, int bufLen);

protected:
	void determineEncoding();

	u78 utf8_7bits_8bits();
private:
	UniMode    m_eEncoding;
	ubyte*          m_pBuf;
	ubyte*          m_pNewBuf;
	size_t          m_nBufSize;
	size_t			m_nSkip;
	bool            m_bFirstRead;
	size_t          m_nLen;
	Utf16_Iter      m_Iter16;
};

// Read in a UTF-8 buffer and write out to UTF-16 or UTF-8
class Utf8_16_Write : public Utf8_16 {
public:
	Utf8_16_Write();
	~Utf8_16_Write();

	void setEncoding(UniMode eType);

	FILE * fopen(const TCHAR *_name, const TCHAR *_type);
	size_t fwrite(const void* p, size_t _size);
	void   fclose();

	size_t convert(char* p, size_t _size);
	char* getNewBuf() { return reinterpret_cast<char*>(m_pNewBuf); }
	size_t calcCurPos(size_t pos);

protected:
	UniMode m_eEncoding;
	FILE* m_pFile;
	utf16* m_pBuf;
	ubyte* m_pNewBuf;
	size_t m_nBufSize;
	bool m_bFirstWrite;
};

#endif// UTF8_16_H