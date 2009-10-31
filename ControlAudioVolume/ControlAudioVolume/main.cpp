// ControlAudioVolume.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyAudioControl.h"
#include <iostream>
using namespace std;


void audioVolume()
{
	MMRESULT rc;              // Return code.
	HMIXER hMixer;            // Mixer handle used in mixer API calls.
	MIXERCONTROL mxc;         // Holds the mixer control data.
	MIXERLINE mxl;            // Holds the mixer line data.
	MIXERLINECONTROLS mxlc;   // Obtains the mixer control.
	
	// Open the mixer. This opens the mixer with a deviceID of 0. If you
	// have a single sound card/mixer, then this will open it. If you have
	// multiple sound cards/mixers, the deviceIDs will be 0, 1, 2, and
	// so on.
	rc = mixerOpen(&hMixer, 0,0,0,0);
	if (MMSYSERR_NOERROR != rc) {
		// Couldn't open the mixer.
	}

	// Initialize MIXERLINE structure.
	ZeroMemory(&mxl,sizeof(mxl));
	mxl.cbStruct = sizeof(mxl);

	// Specify the line you want to get. You are getting the input line
	// here. If you want to get the output line, you need to use
	// MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT.
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

	rc = mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl,
		MIXER_GETLINEINFOF_COMPONENTTYPE);
	if (MMSYSERR_NOERROR == rc) {
		// Couldn't get the mixer line.
	}

	// Get the control.
	ZeroMemory(&mxlc, sizeof(mxlc));
	mxlc.cbStruct = sizeof(mxlc);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_PEAKMETER;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(mxc);
	mxlc.pamxctrl = &mxc;
	ZeroMemory(&mxc, sizeof(mxc));
	mxc.cbStruct = sizeof(mxc);
	rc = mixerGetLineControls((HMIXEROBJ)hMixer,&mxlc,
		MIXER_GETLINECONTROLSF_ONEBYTYPE);
	if (MMSYSERR_NOERROR != rc) {
		// Couldn't get the control.
	}

	// After successfully getting the peakmeter control, the volume range
	// will be specified by mxc.Bounds.lMinimum to mxc.Bounds.lMaximum.

	MIXERCONTROLDETAILS mxcd;             // Gets the control values.
	MIXERCONTROLDETAILS_SIGNED volStruct; // Gets the control values.
	long volume;                          // Holds the final volume value.

	// Initialize the MIXERCONTROLDETAILS structure
	ZeroMemory(&mxcd, sizeof(mxcd));
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.cbDetails = sizeof(volStruct);
	mxcd.dwControlID = mxc.dwControlID;
	mxcd.paDetails = &volStruct;
	mxcd.cChannels = 1;

	// Get the current value of the peakmeter control. Typically, you
	// would set a timer in your program to query the volume every 10th
	// of a second or so.
	rc = mixerGetControlDetails((HMIXEROBJ)hMixer, &mxcd,
		MIXER_GETCONTROLDETAILSF_VALUE);
	if (MMSYSERR_NOERROR == rc) {
		// Couldn't get the current volume.
	}
	volume = volStruct.lValue;

	// Get the absolute value of the volume.
	if (volume < 0)
		volume = -volume;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//DWORD 4�ֽ�
	//WORD  2�ֽ�
	THAudioControl c;
	c.showDevices();




	system("pause");
	return 0;
}

