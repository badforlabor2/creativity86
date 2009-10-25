/************************************************************************/
/* �Լ�����һ��������������,������Ҫ������չ                            */
/* ����windows��Audio Mixers�ӿ�										*/
/* �������̣�															*/
/*		��mixer-->���һ��line-->��ø�line��controller-->����		*/
/************************************************************************/
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

class THAudioControl
{
public:
	THAudioControl()
	{
		//����Ƶ�豸
		openDevice();
	}
	~THAudioControl()
	{
		//�ر���Ƶ�豸
		closeDevice();
	}
	bool getController();

private:
	HMIXER hMixer;
	bool openDevice();
	void closeDevice();
};