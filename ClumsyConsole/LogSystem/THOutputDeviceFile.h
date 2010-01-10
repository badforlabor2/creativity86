
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
		static bool Entry = 0;	//try...catch���ƿ�
		if(Entry){

			//��һ�ε�ʱ�򣬽���һ����־�ļ�LogAr
			if(!LogAr && !Dead){
				if(!FileName[0]){
					//����Ĭ���ļ�������
					thStrcpy(FileName, thBaseDir());
					thStrcat(FileName, TEXT("log\\"));
					thStrcat(FileName, TEXT("stdout.log"));	//���stdout.logӦ���ǿ����޸ĵ�һ��ȫ�ֱ���
				}
				//����һ��THArchive��ֵ��LogAr
				LogAr = gFileManager->CreateFileWriter(FileName, FILEWRITE_AllowRead|FILEWRITE_Unbuffered|(Opened?FILEWRITE_Append:0));

				if(LogAr){
					Opened = 1;
					Log(reinterpret_cast<TCHAR *>("Log file open at: %s"), thTimestamp());
				}else{
					Dead = 1;
				}
			}

			if(LogAr){
#ifdef FORCE_ANSI_LOG
				TCHAR ch[65536] = TEXT("");
				thStrcat(ch, TEXT("debug"));	//�������"debug"��װ
				thStrcat(ch, TEXT(":"));
				thStrcat(ch, Data);
				thStrcat(ch, LINE_TERMINATOR);
				ANSICHAR ACh[65536];
				INT i;
				//ת��ΪANSI��ʽ������0xff�ģ��滻��Ĭ��ֵ
				for( i=0; ch[i]; i++ )
					ACh[i] = ToAnsi(ch[i] );
				ACh[i] = 0;
				LogAr->Serialize(ACh, i);
#else
				WriteRaw(TEXT("debug"));
				WriteRaw(TEXT(": "));
				WriteRaw(Data);
				WriteRaw(LINE_TERMINATOR);
#endif
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