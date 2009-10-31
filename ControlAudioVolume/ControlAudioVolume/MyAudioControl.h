/************************************************************************/
/* 自己定义一个控制音量的类,根据需要慢慢扩展                            */
/* 调用windows的Audio Mixers接口										*/
/* 整个流程：															*/
/*		打开mixer-->获得一个line-->获得该line的controller-->操作		*/
/************************************************************************/
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")


class THAudioControl
{
public:
	THAudioControl()
	{
		//打开音频设备
		openDevice();
	}
	~THAudioControl()
	{
		//关闭音频设备
		closeDevice();
	}
	bool getController();
	void showDevices();
private:
	HMIXER hMixer;
	bool openDevice();
	void closeDevice();
};