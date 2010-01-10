#ifndef THFILEMANAGERWINDOWS_H
#define THFILEMANAGERWINDOWS_H

#include "platform.h"
#include "THArchive.h"
#include "THFileManager.h"


class THFileManagerWindows : public THFileManager
{
public:
	THArchive *CreateFileReader(const TCHAR *FileName, DWORD ReadFlag = 0);
	THArchive *CreateFileWriter(const TCHAR *FileName, DWORD WriteFlag = 0);
	INT FileSize(const TCHAR *FileName);

	virtual void Init(BOOL startup){}
	virtual BOOL IsReadOnly(const TCHAR *FileName) 
	{
		return 0;
	}
	virtual BOOL Delete(const TCHAR *FileName, BOOL RequireExits = 0, BOOL EvenReadOnly = 0) 
	{
		return 0;
	}
	virtual DWORD Copy(const TCHAR *Dest, const TCHAR *Src, BOOL Replace = 1, BOOL EvenReadOnly = 0, BOOL Attributes = 0, DWORD Compress=FILECOPY_Normal, FCopyProgress *Progress=NULL) 
	{
		return 0;
	}
	virtual BOOL Move(const TCHAR *Dest, const TCHAR *Src, BOOL Replace = 1, BOOL EvenReadOnly = 0, BOOL Attributes = 0) 
	{
		return 0;
	}
	virtual BOOL MakeDirectory(const TCHAR *Path, BOOL Tree = 0) 
	{
		return 0;
	}
	virtual BOOL DeleteDirectory(const TCHAR *Path, BOOL RequireExit = 0, BOOL Tree = 0) 
	{
		return 0;
	}

};
//这个类属于专属windows，单独一个文件比较好
//对于都文件操作，不能要多少就从文件中读多少。
//比如，就要一字节，那么就从文件中读取出来一字节？太浪费了
//所以要做一个缓冲Buffer，一次读取的内容至少是buffer这么多。
//而你需要的内容可以从buffer中取得。
class THArchiveFileReader : public THArchive
{
public:
	THArchiveFileReader(HANDLE h, INT size)
		:	Handle(h), Size(size), Pos(0), 
			BufferBase(0), BufferCount(0)
	{

	}
	void Serialize(void *v, INT length)
	{
		while(length > 0){
			INT Copy = min(length, BufferBase+BufferCount-Pos);			
			if(Copy == 0){
				if(length >= ARRAY_COUNT(Buffer)){
					INT Count = 0;
					ReadFile(Handle, v, length, (DWORD *)&Count, NULL);
					if(Count != length){
						ArIsError = true;
						//输出错误
					}
					Pos += length;
					BufferBase += length;
					return;
				}		
				PreCache(0x7FFFFFFF);
				Copy = min(length, BufferBase+BufferCount-Pos);
				if(Copy <= 0){
					//此时已经读到文件末尾
					ArIsError = true;
					//输出错误
				}
				if(ArIsError)
					return;
			}
			memcpy(v, Buffer+Pos-BufferBase, Copy);
			Pos += Copy;
			length -= Copy;
			v = (BYTE *)v + Copy;
		}

	}
	void PreCache(INT hintCount)
	{
		BufferBase = Pos;
		//要实现跨平台，就不能用某个平台下的东西，而是用所有平台下的东西
		//所以，min要改成自己定义的函数
		BufferCount = min(min(hintCount, int(ARRAY_COUNT(Buffer)-(Pos&(ARRAY_COUNT(Buffer)-1)))), Size-Pos);
		INT Count = 0;
		::ReadFile(Handle, Buffer, BufferCount, (DWORD *)&Count, NULL);
		if(Count != BufferCount){
			//输出错误信息
			ArIsError = false;
		}
	}
	void Seek(INT pos)
	{
		if(pos>=0 && pos<=Size){
			if(::SetFilePointer(Handle, pos, 0, FILE_BEGIN) == 0xFFFFFFFF){
				ArIsError = false;
				//输出错误信息
			}
			Pos = pos;
			BufferBase = Pos;
			BufferCount = 0;
		}
	}
	INT Tell()
	{
		return Pos;
	}
	INT TotalSize()
	{
		return Size;
	}
	BOOL Close()
	{
		if(Handle)
			::CloseHandle(Handle);
		Handle = NULL;
		return !ArIsError;
	}

protected:
	HANDLE Handle;
	INT Size;
	INT Pos;
	INT BufferCount;	//共有多少buffer数据
	INT BufferBase;		//从那个位置开始做buffer
	BYTE Buffer[1024]; 
};

//这个类属于专属windows，单独一个文件比较好
//对于写文件操作，不能有多少就往文件写多少。
//比如，就一个字节，也立即写文件么？如果这样就太浪费了
//所以要做一个缓冲Buffer
class THArchiveFileWriter : public THArchive
{
public:
	THArchiveFileWriter(HANDLE handle, INT pos)
		:	Handle(handle), Pos(pos), BufferCount(0)
	{
		ArIsSaving = ArIsPersistent = 1;
	}
	~THArchiveFileWriter()
	{
		if(Handle)
			::CloseHandle(Handle);
		Handle = NULL;
	}
	void Seek(INT pos)
	{
		Flush();
		if(::SetFilePointer(Handle, pos, 0, FILE_BEGIN) == 0xFFFFFFFF){
			ArIsError = true;
			//输出错误
		}
		Pos = pos;
	}
	INT Tell()
	{
		return Pos;
	}
	BOOL Close()
	{
		Flush();
		if(Handle && !::CloseHandle(Handle)){
			ArIsError = true;
			//输出错误
		}
		Handle = NULL;
		return !ArIsError;
	}
	void Serialize(void *v, INT length)
	{
		Pos += length;
		INT Copy;
		while(length > (Copy = ARRAY_COUNT(Buffer)-BufferCount)){
			thMemcpy(Buffer+BufferCount, v, Copy);
			BufferCount += Copy;
			length -= Copy;
			v = (BYTE *)v + Copy;
			Flush();
		}
		if(length){
			thMemcpy(Buffer+BufferCount, v, length);
			BufferCount += length;
		}
	}
	void Flush()
	{
		if(BufferCount){
			INT Result = 0;
			if(!::WriteFile(Handle, Buffer, BufferCount, (DWORD *)&Result, NULL)){
				ArIsError = 1;
				//输出错误信息
			}
		}
		BufferCount = 0;
	}
protected:
	HANDLE Handle;
	INT Size;
	INT Pos;
	INT BufferCount;	//共有多少buffer数据
	INT BufferBase;		//从那个位置开始做buffer
	BYTE Buffer[1024]; 
};

#endif