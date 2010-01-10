
#ifndef THOUTPUTDEVICEFILE_H
#define THOUTPUTDEVICEFILE_H

#include "platform.h"
#include "THOutputDevice.h"
#include "THFileManager.h"

extern THFileManager *gFileManager;

class THOutputDeviceFile : public THOutputDevice
{
public:
	THOutputDeviceFile()
		:	LogAr(NULL),Opened(0),Dead(0)
	{
		FileName[0]=0;
	}
	void TearDown()
	{
		if(LogAr){
			Log(reinterpret_cast<TCHAR *>("log file is closed at %s"), "00:00:00");	//at time!
			delete LogAr;
			LogAr = NULL;
		}
	}
	void Flush()
	{
		if(LogAr){
			LogAr->Flush();
		}
	}
	void Serialize(const TCHAR *Data)
	{
		static bool Entry = 0;	//用作排队
		if(Entry){

			//第一次的时候，建立一个日志文件LogAr
			if(!LogAr && !Dead){
				if(!FileName[0]){
					//创建默认文件的名字
					thStrcpy(FileName, thBaseDir());
					thStrcat(FileName, TEXT("log\\"));
					thStrcat(FileName, TEXT("stdout.log"));	//这个stdout.log应该是可以修改的一个全局变量
				}
				//创建一个THArchive赋值给LogAr
				LogAr = gFileManager->CreateFileWriter(FileName, FILEWRITE_AllowRead|FILEWRITE_Unbuffered|(Opened?FILEWRITE_Append:0));

				if(LogAr){
					Opened = 1;
					Log(reinterpret_cast<TCHAR *>("Log file open at: %s"), thTimestamp());
				}else{
					Dead = 1;
				}
			}

			if(LogAr){
				TCHAR ch[65536] = TEXT("");
				thStrcat(ch, TEXT("debug"));	//这里需把"debug"封装
				thStrcat(ch, TEXT(":"));
				thStrcat(ch, Data);
				thStrcat(ch, LINE_TERMINATOR);
				//这里需要把Unicode转化为ansi，因为Archive不一定将数据作为Unicode类型输出
				//序列化的时候肯定要一个字节一个字节的来
				LogAr->Serialize(ch, thStrlen(ch));
			}
		}else{
			Entry = 1;
			try{
				Serialize(Data);
			}catch(...)
			{

			}
			Entry = 0;
		}
	}
public:
	THArchive *LogAr;
	TCHAR FileName[1024];
private:
	bool Opened, Dead;
	void WriteRaw(const TCHAR *s)
	{
		LogAr->Serialize((void *)s, thStrlen(s) * sizeof(TCHAR));
	}
};

#endif