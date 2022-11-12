//--------------------------------------------------------------//
//	"vn_DirectSound.cpp"										//
//		DirectSound管理											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../vn_environment.h"

IDirectSound8 *vnDirectSound::pDS8=NULL;
LPDIRECTSOUNDBUFFER vnDirectSound::lpPrimary=NULL;

int vnDirectSound::initialize(void *hdl)
{
	HRESULT ret;
	//サウンドデバイス作成
	DirectSoundCreate8(NULL, &pDS8, NULL);
	
	//協調レベル設定
	pDS8->SetCooperativeLevel((HWND)hdl, DSSCL_NORMAL); 
	WAVEFORMATEX wf;
	
    // プライマリサウンドバッファの作成
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
    dsdesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
    dsdesc.dwBufferBytes = 0;
    dsdesc.lpwfxFormat = NULL;
    ret = pDS8->CreateSoundBuffer( &dsdesc,&lpPrimary,NULL );
    if( FAILED(ret) ) {
    //    DEBUG( "プライマリサウンドバッファ作成失敗\n","" );
        return 0;
    }
    
    //バッファ オブジェクトの能力を取得する。
    DSCAPS caps;
    pDS8->GetCaps(&caps);
	
    // プライマリバッファのステータスを決定
    wf.cbSize = sizeof( WAVEFORMATEX );
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 2;
    wf.nSamplesPerSec = 44100;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    ret = lpPrimary->SetFormat( &wf );
    if( FAILED(ret) ) {
     //   DEBUG( "プライマリバッファのステータス失敗\n","" );
        return 0;
    }
	
	return 1;
}

void vnDirectSound::terminate(void)
{
	SAFE_RELEASE(lpPrimary);
	SAFE_RELEASE(pDS8);
}
