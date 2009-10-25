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

//���һ��line֮���ٻ�ø�line��controller
bool THAudioControl::getController()
{
	MIXERLINE			line;
	MMRESULT			rc;
	MIXERLINECONTROLS	lineControls;
	MIXERCONTROL		control;
	
	ZeroMemory(&line, sizeof(line));
	line.cbStruct = sizeof(line);
	line.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	rc = mixerGetLineInfo((HMIXEROBJ)hMixer, &line, MIXER_GETLINEINFOF_COMPONENTTYPE);
	if(rc != MMSYSERR_NOERROR){
		cout<<"can't get waveout\n";
		return false;
	}

	//һ��line�����м���controller
	ZeroMemory(&lineControls, sizeof(lineControls));
	lineControls.cbStruct = sizeof(lineControls);
	lineControls.dwLineID = line.dwLineID;
	lineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	lineControls.cControls = 1;
	lineControls.cbmxctrl = sizeof(control);
	lineControls.pamxctrl = &control;
	rc = mixerGetLineControls((HMIXEROBJ)hMixer, &lineControls,
		MIXER_GETLINECONTROLSF_ONEBYTYPE);
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
	detail.dwControlID = control.dwControlID;
	detail.paDetails = &volStruct;
	detail.cChannels = 1;
	rc = mixerGetControlDetails((HMIXEROBJ)hMixer, &detail,
		MIXER_GETCONTROLDETAILSF_VALUE);
	if(rc == MMSYSERR_ERROR){
		cout<<"can't get details from controller\n";
		return false;
	}
	volume = volStruct.dwValue;
	cout<<"current volume:"<<volume<<endl;

	volStruct.dwValue = 0;
	rc = mixerSetControlDetails((HMIXEROBJ)hMixer, &detail,
		MIXER_SETCONTROLDETAILSF_VALUE);

	return true;
}