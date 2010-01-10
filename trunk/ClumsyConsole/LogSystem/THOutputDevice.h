/************************************************************************/
/* Base OutputSystem                                                    */
/************************************************************************/
/************************************************************************/
/* 新的规范出炉，所有的单词首字母要大些，这样方法和属性一般开头都大写	*/
/* 除非，属性的前几个字母表示变量类型，参数则小写字母开头				*/
/************************************************************************/
#ifndef THOUTPUTDEVICE_H
#define THOUTPUTDEVICE_H

#include "platform.h"
#include "THArchive.h"


//这个的值可以在初始化的时候重新复制

class THOutputDevice
{
public:
	virtual void Serialize(const TCHAR *s)=0;
	virtual void Flush(){};
	
	// close output device and clear up, this can't happened in destructor
	// as we might have to call "delete" which can't be done for static/global
	// object
	virtual void TearDown(){};
	
	void Log(const TCHAR *s)
	{
		Serialize(s);
	}
	void Log(const TCHAR *s, ...)
	{
	
	}
};
class THOutputDeviceStdOut : public THOutputDevice
{
	void Serialize(const TCHAR *s)
	{
#if UNICODE
		wcout<<s;
#else
		cout<<s;
#endif
	}
};


#endif