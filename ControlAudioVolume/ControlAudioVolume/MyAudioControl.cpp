#include "MyAudioControl.h"
#include <iostream>
using namespace std;

//打开设备
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

//关闭设备
void THAudioControl::closeDevice()
{
	if(hMixer != NULL){
		mixerClose(hMixer);
	}else{
		cout<<"where is your mixer?\n";
	}
}
//显示所有的音频设备
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


//获得一个line之后再获得该line的controller
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

	//一个line可能有几个controller
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

	//获得controllers就能得到control，然后在获得该control的detail值
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