#include "MyAudioControl.h"
#include <iostream>
using namespace std;

//���豸
bool THAudioControl::openDevice()
{
	MMRESULT rc;
	rc = mixerOpen(&hMixer, 0, 0, 0, 0);
	if(rc != MMSYSERR_NOERROR){
		cout<<"can't open mixer!\n";
		return false;
	}


	return true;
}

//�ر��豸
void THAudioControl::closeDevice()
{
	if(hMixer != NULL){
		mixerClose(hMixer);
	}else{
		cout<<"where is your mixer?\n";
	}
}
//��ʾ���е���Ƶ�豸
void THAudioControl::showDevices()
{
	MIXERLINE		line;
	MMRESULT		ret;

	for(int i=MIXERLINE_COMPONENTTYPE_SRC_FIRST; i<=MIXERLINE_COMPONENTTYPE_SRC_LAST; i++){
		ZeroMemory(&line, sizeof(line));
		line.cbStruct = sizeof(line);
		line.dwComponentType = i;
		ret = mixerGetLineInfo((HMIXEROBJ)hMixer, &line, MIXER_GETLINEINFOF_COMPONENTTYPE);
		if(ret == MMSYSERR_NOERROR){
			wcout<<i<<" "<<line.szName<<endl;
		}
	}
}


//���һ��line֮���ٻ�ø�line��controller
bool THAudioControl::getController()
{
	MIXERLINE			line;
	MMRESULT			rc;
	MIXERLINECONTROLS	lineControls;
	MIXERCONTROL		control;
	LINE_CONTROL		*controls;

	ZeroMemory(&line, sizeof(line));
	line.cbStruct = sizeof(line);
	line.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	rc = mixerGetLineInfo((HMIXEROBJ)hMixer, &line, MIXER_GETLINEINFOF_COMPONENTTYPE);
	if(rc != MMSYSERR_NOERROR){
		cout<<"can't get waveout\n";
		return false;
	}
	int size = sizeof(MIXERCONTROL) * (line.cControls - 1);
	controls = (LINE_CONTROL *)malloc(sizeof(LINE_CONTROL)+size);
	controls->size = line.cControls;

	//һ��line�����м���controller
	ZeroMemory(&lineControls, sizeof(lineControls));
	lineControls.cbStruct = sizeof(lineControls);
	lineControls.dwLineID = line.dwLineID;
	//lineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	lineControls.cControls = line.cControls;
	lineControls.cbmxctrl = sizeof(control);
	lineControls.pamxctrl = controls->control;
	rc = mixerGetLineControls((HMIXEROBJ)hMixer, &lineControls,
		MIXER_GETLINECONTROLSF_ALL);
	if(rc != MMSYSERR_NOERROR){
		cout<<"can't get line controllers\n";
	}

	//���controllers���ܵõ�control��Ȼ���ڻ�ø�control��detailֵ
	MIXERCONTROLDETAILS					detail;
	MIXERCONTROLDETAILS_UNSIGNED			volStruct;
	long volume;

	ZeroMemory(&detail, sizeof(detail));
	detail.cbStruct = sizeof(detail);
	detail.cbDetails = sizeof(volStruct);
	detail.dwControlID = controls->control[0].dwControlID;
	detail.paDetails = &volStruct;
	detail.cChannels = 1;
	rc = mixerGetControlDetails((HMIXEROBJ)hMixer, &detail,
		MIXER_GETCONTROLDETAILSF_VALUE);
	if(rc == MMSYSERR_ERROR){
		cout<<"can't get details from controller\n";
		return false;
	}
	volume = volStruct.dwValue;
//	cout<<"current volume:"<<volume<<endl;

	volStruct.dwValue = 0;
	rc = mixerSetControlDetails((HMIXEROBJ)hMixer, &detail,
		MIXER_SETCONTROLDETAILSF_VALUE);

	return true;
}