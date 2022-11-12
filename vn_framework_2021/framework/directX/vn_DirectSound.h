//--------------------------------------------------------------//
//	"vn_DirectSound.h"											//
//		DirectSoundä«óù											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnDirectSound
{
private:
	static IDirectSound8 *pDS8;
	static LPDIRECTSOUNDBUFFER lpPrimary;

public:
	static int initialize(void *hdl);
	static void terminate(void);

	static IDirectSound8 *getDevice(){return pDS8;}
};
