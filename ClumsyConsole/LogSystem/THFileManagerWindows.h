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
//���������ר��windows������һ���ļ��ȽϺ�
//���ڶ��ļ�����������Ҫ���پʹ��ļ��ж����١�
//���磬��Ҫһ�ֽڣ���ô�ʹ��ļ��ж�ȡ����һ�ֽڣ�̫�˷���
//����Ҫ��һ������Buffer��һ�ζ�ȡ������������buffer��ô�ࡣ
//������Ҫ�����ݿ��Դ�buffer��ȡ�á�
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
						//�������
					}
					Pos += length;
					BufferBase += length;
					return;
				}		
				PreCache(0x7FFFFFFF);
				Copy = min(length, BufferBase+BufferCount-Pos);
				if(Copy <= 0){
					//��ʱ�Ѿ������ļ�ĩβ
					ArIsError = true;
					//�������
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
		//Ҫʵ�ֿ�ƽ̨���Ͳ�����ĳ��ƽ̨�µĶ���������������ƽ̨�µĶ���
		//���ԣ�minҪ�ĳ��Լ�����ĺ���
		BufferCount = min(min(hintCount, int(ARRAY_COUNT(Buffer)-(Pos&(ARRAY_COUNT(Buffer)-1)))), Size-Pos);
		INT Count = 0;
		::ReadFile(Handle, Buffer, BufferCount, (DWORD *)&Count, NULL);
		if(Count != BufferCount){
			//���������Ϣ
			ArIsError = false;
		}
	}
	void Seek(INT pos)
	{
		if(pos>=0 && pos<=Size){
			if(::SetFilePointer(Handle, pos, 0, FILE_BEGIN) == 0xFFFFFFFF){
				ArIsError = false;
				//���������Ϣ
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
	INT BufferCount;	//���ж���buffer����
	INT BufferBase;		//���Ǹ�λ�ÿ�ʼ��buffer
	BYTE Buffer[1024]; 
};

//���������ר��windows������һ���ļ��ȽϺ�
//����д�ļ������������ж��پ����ļ�д���١�
//���磬��һ���ֽڣ�Ҳ����д�ļ�ô�����������̫�˷���
//����Ҫ��һ������Buffer
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
			//�������
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
			//�������
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
				//���������Ϣ
			}
		}
		BufferCount = 0;
	}
protected:
	HANDLE Handle;
	INT Size;
	INT Pos;
	INT BufferCount;	//���ж���buffer����
	INT BufferBase;		//���Ǹ�λ�ÿ�ʼ��buffer
	BYTE Buffer[1024]; 
};

#endif