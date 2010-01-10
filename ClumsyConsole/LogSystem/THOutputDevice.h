/************************************************************************/
/* Base OutputSystem                                                    */
/************************************************************************/
/************************************************************************/
/* �µĹ淶��¯�����еĵ�������ĸҪ��Щ����������������һ�㿪ͷ����д	*/
/* ���ǣ����Ե�ǰ������ĸ��ʾ�������ͣ�������Сд��ĸ��ͷ				*/
/************************************************************************/
#ifndef THOUTPUTDEVICE_H
#define THOUTPUTDEVICE_H

#include "platform.h"
#include "THArchive.h"


//�����ֵ�����ڳ�ʼ����ʱ�����¸���

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