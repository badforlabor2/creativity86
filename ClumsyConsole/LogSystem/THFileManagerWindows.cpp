#include "THFileManagerWindows.h"

THArchive *THFileManagerWindows::CreateFileReader(const TCHAR *FileName, DWORD ReadFlag)
{
	DWORD Access = GENERIC_READ;
	DWORD WinFlag = FILE_SHARE_READ;
	DWORD Create = OPEN_EXISTING;
	HANDLE Handle= CreateFile(FileName, Access, WinFlag, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(Handle == INVALID_HANDLE_VALUE){
		if(ReadFlag & FILEREAD_NoFail){
			//输出错误信息，可以用全局函数的方法，或者在函数的参数中加入一个函数指针
			return NULL;
		}
	}
	return new THArchiveFileReader(Handle, ::GetFileSize(Handle, NULL));
}
THArchive *THFileManagerWindows::CreateFileWriter(const TCHAR *FileName, DWORD Flags)
{
	if(FileSize(FileName) && (Flags & FILEWRITE_EvenIfReadOnly)){
		::SetFileAttributes(FileName, Flags);
	}
	DWORD Access = GENERIC_WRITE;
	DWORD WinFlags = (Flags & FILEWRITE_AllowRead) ? FILE_SHARE_READ : 0;
	DWORD Create = (Flags & FILEWRITE_Append) ? OPEN_ALWAYS : (Flags & FILEWRITE_NoReplaceExisting) ? CREATE_NEW : CREATE_ALWAYS;
	HANDLE Handle = ::CreateFile(FileName, Access, WinFlags, NULL, Create, FILE_ATTRIBUTE_NORMAL, NULL);
	INT Pos = 0;
	if(Handle == INVALID_HANDLE_VALUE){
		if(Flags & FILEWRITE_NoFail){
			//显示错误
			return NULL;
		}
	}
	if(Flags & FILEWRITE_Append){
		Pos = ::SetFilePointer(Handle, 0, 0, FILE_END);
	}
	return new THArchiveFileWriter(Handle, Pos);
}
INT THFileManagerWindows::FileSize(const TCHAR *FileName)
{
	HANDLE Handle = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(Handle == INVALID_HANDLE_VALUE){
		//显示错误
		return -1;
	}
	DWORD Result = ::GetFileSize(Handle, NULL);
	CloseHandle(Handle);
	return Result;
}