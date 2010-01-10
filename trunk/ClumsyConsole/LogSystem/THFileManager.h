/************************************************************************/
/* FileManager,不同的平台有不同的FileManager，这就是跨平台              */
/************************************************************************/
#ifndef THFILEMANAGER_H
#define THFILEMANAGER_H

#include "platform.h"
#include "THArchive.h"

enum ECopyCompress
{
	FILECOPY_Normal = 0x00,
	FILECOPY_Compress = 0x01,
	FILECOPY_Decompress = 0x02,
};
enum ECopyResult
{
	COPY_OK = 0x00,

};
enum EReadFile
{
	FILEREAD_NoFail = 0x01,
};
enum EFileWrite
{
	FILEWRITE_NoFail            = 0x01,
	FILEWRITE_NoReplaceExisting = 0x02,
	FILEWRITE_EvenIfReadOnly    = 0x04,
	FILEWRITE_Unbuffered        = 0x08,
	FILEWRITE_Append			= 0x10,
	FILEWRITE_AllowRead         = 0x20,
};
struct FCopyProgress
{
	virtual BOOL Poll(float Fraction) = 0;
};

///接口
class THFileManager
{
public:
	virtual void Init(BOOL startup){}
	virtual THArchive *CreateFileReader(const TCHAR *FileName, DWORD ReadFlag = 0) = 0;	//这里可以加一个参数，用于输出错误信息
	virtual THArchive *CreateFileWriter(const TCHAR *FileName, DWORD WriteFlag = 0) = 0;
	virtual INT FileSize(const TCHAR *FileName) = 0;
	virtual BOOL IsReadOnly(const TCHAR *FileName) = 0;
	virtual BOOL Delete(const TCHAR *FileName, BOOL RequireExits = 0, BOOL EvenReadOnly = 0) = 0;
	virtual DWORD Copy(const TCHAR *Dest, const TCHAR *Src, BOOL Replace = 1, BOOL EvenReadOnly = 0, BOOL Attributes = 0, DWORD Compress=FILECOPY_Normal, FCopyProgress *Progress=NULL) = 0;
	virtual BOOL Move(const TCHAR *Dest, const TCHAR *Src, BOOL Replace = 1, BOOL EvenReadOnly = 0, BOOL Attributes = 0) = 0;
	virtual BOOL MakeDirectory(const TCHAR *Path, BOOL Tree = 0) = 0;
	virtual BOOL DeleteDirectory(const TCHAR *Path, BOOL RequireExit = 0, BOOL Tree = 0) = 0;
	//virtual BOOL FindFiles(...);
};

class THFileManagerGeneric : public THFileManager
{

};


#endif