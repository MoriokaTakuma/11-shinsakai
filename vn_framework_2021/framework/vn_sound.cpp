//--------------------------------------------------------------//
//	"vn_sound.cpp"												//
//		WAVEファイル再生クラス									//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

vnSound::vnSound(const WCHAR *path)
{
	HRESULT hr;
    MMCKINFO mSrcWaveFile;
    MMCKINFO mSrcWaveFmt;
    MMCKINFO mSrcWaveData;
    LPWAVEFORMATEX wf;

	pDSBuffer = NULL;

	// WAVファイルをロード
    HMMIO hSrc;
    hSrc = mmioOpen( (LPWSTR)path, NULL,MMIO_ALLOCBUF|MMIO_READ|MMIO_COMPAT );
    if( !hSrc ) {
    //    DEBUG( "WAVファイルロードエラー\n","" );
        return;
    }

    // 'WAVE'チャンクチェック
    ZeroMemory( &mSrcWaveFile,sizeof(mSrcWaveFile) );
	hr = mmioDescend( hSrc,&mSrcWaveFile,NULL,MMIO_FINDRIFF );
    if( mSrcWaveFile.fccType!=mmioFOURCC('W','A','V','E') ) {
    //    DEBUG( "WAVEチャンクチェックエラー\n","" );
        mmioClose( hSrc,0 );
        return;
    }

    // 'fmt 'チャンクチェック
    ZeroMemory( &mSrcWaveFmt,sizeof(mSrcWaveFmt) );
	hr = mmioDescend( hSrc,&mSrcWaveFmt,&mSrcWaveFile,MMIO_FINDCHUNK );
    if( mSrcWaveFmt.ckid!=mmioFOURCC('f','m','t',' ') ) {
    //    DEBUG( "fmt チャンクチェックエラー\n","" );
        mmioClose( hSrc,0 );
        return;
    }

    // ヘッダサイズの計算
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if( iSrcHeaderSize<sizeof(WAVEFORMATEX) )
        iSrcHeaderSize=sizeof(WAVEFORMATEX);

    // ヘッダメモリ確保
    wf = (LPWAVEFORMATEX)malloc( iSrcHeaderSize );
    if( !wf ) {
    //    DEBUG( "メモリ確保エラー\n","" );
        mmioClose( hSrc,0 );
        return;
    }
    ZeroMemory( wf,iSrcHeaderSize );

    // WAVEフォーマットのロード
	hr = mmioRead( hSrc,(char*)wf,mSrcWaveFmt.cksize );
    if( FAILED(hr) ) {
     //   DEBUG( "WAVEフォーマットロードエラー\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return;
    }
 //   DEBUG( "チャンネル数       = %d\n",wf->nChannels );
 //   DEBUG( "サンプリングレート = %d\n",wf->nSamplesPerSec );
 //   DEBUG( "ビットレート       = %d\n",wf->wBitsPerSample );


    // fmtチャンクに戻る
    mmioAscend( hSrc,&mSrcWaveFmt,0 );

    // dataチャンクを探す
    while(1) {
        // 検索
		hr = mmioDescend( hSrc,&mSrcWaveData,&mSrcWaveFile,0 );
        if( FAILED(hr) ) {
         //   DEBUG( "dataチャンクが見つからない\n","" );
            free( wf );
            mmioClose( hSrc,0 );
            return;
        }
        if( mSrcWaveData.ckid==mmioStringToFOURCC(L"data",0) )
            break;
        // 次のチャンクへ
		hr = mmioAscend( hSrc,&mSrcWaveData,0 );
    }
 //   DEBUG( "データサイズ       = %d\n",mSrcWaveData.cksize );


    // サウンドバッファの作成
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
	dsdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
    dsdesc.dwBufferBytes = mSrcWaveData.cksize;
    dsdesc.lpwfxFormat = wf;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	hr = vnDirectSound::getDevice()->CreateSoundBuffer( &dsdesc,&pDSBuffer,NULL );
    if( FAILED(hr) )
	{
     //   DEBUG( "サウンドバッファの作成エラー\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return;
    }

    // ロック開始
    LPVOID pMem1,pMem2;
    DWORD dwSize1,dwSize2;
	hr = pDSBuffer->Lock( 0,mSrcWaveData.cksize,&pMem1,&dwSize1,&pMem2,&dwSize2,0 );
    if( FAILED(hr) )
	{
    //    DEBUG( "ロック失敗\n","" );
        free( wf );
        mmioClose( hSrc,0 );
        return;
    }

    // データ書き込み
    mmioRead( hSrc,(char*)pMem1,dwSize1 );
    mmioRead( hSrc,(char*)pMem2,dwSize2 );

    // ロック解除
    pDSBuffer->Unlock( pMem1,dwSize1,pMem2,dwSize2 );

    // ヘッダ用メモリを開放
    free( wf );

    // WAVを閉じる
    mmioClose( hSrc,0 );

	pDSBuffer->SetCurrentPosition(0);
}

vnSound::~vnSound()
{
	SAFE_RELEASE(pDSBuffer);
}

bool vnSound::play(bool loop)
{
	if(pDSBuffer)
	{
		HRESULT hr = pDSBuffer->Play(0, 0, loop==true?DSBPLAY_LOOPING:0);
		return hr==DS_OK ;
	}
	return false;
}

bool vnSound::stop()
{
	if(pDSBuffer)
	{
		HRESULT hr = pDSBuffer->Stop();
		return hr==DS_OK ;
	}
	return false;
}

bool vnSound::isPlaying(void)
{
	if(pDSBuffer)
	{
		DWORD stat;
		pDSBuffer->GetStatus(&stat);
		return (stat&DSBSTATUS_PLAYING)!=0;
	}
	return false;
}

bool vnSound::isStopped()
{
	return !isPlaying();
}

bool vnSound::setVolume(LONG lVolume)
{
	if(pDSBuffer)
	{
		if(lVolume>DSBVOLUME_MAX || lVolume<DSBVOLUME_MIN)return false;
		HRESULT hr = pDSBuffer->SetVolume(lVolume);
		return hr==DS_OK;
	}
	return false;
}

bool vnSound::setPan(LONG lPan)
{
	if(pDSBuffer)
	{
		if(lPan<DSBPAN_LEFT || lPan>DSBPAN_RIGHT)return false;
		HRESULT hr = pDSBuffer->SetPan(lPan);
		switch(hr)
		{
		case DS_OK:break;
		case DSERR_CONTROLUNAVAIL:	OutputDebugString(L"onSound::volume::set DSERR_CONTROLUNAVAIL\n");break;
		case DSERR_GENERIC:			OutputDebugString(L"onSound::volume::set DSERR_GENERIC\n");break;
		case DSERR_INVALIDPARAM:	OutputDebugString(L"onSound::volume::set DSERR_INVALIDPARAM\n");break;
		case DSERR_PRIOLEVELNEEDED:	OutputDebugString(L"onSound::volume::set DSERR_PRIOLEVELNEEDED\n");break;
		default:					OutputDebugString(L"onSound::volume::set Unknown\n");break;
		}
		return hr==DS_OK;
	}
	return false;
}

bool vnSound::setFrequency(DWORD dwFrequency)
{
	if(pDSBuffer)
	{
		HRESULT hr = pDSBuffer->SetFrequency(dwFrequency);
		return hr==DS_OK;
	}
	return false;
}

LONG vnSound::getVolume()
{
	if(pDSBuffer)
	{
		LONG ret=0;
		HRESULT hr = pDSBuffer->GetVolume(&ret);
		return ret;
	}
	return 0;
}

LONG vnSound::getPan()
{
	if(pDSBuffer)
	{
		LONG ret=0;
		HRESULT hr = pDSBuffer->GetPan(&ret);
		switch(hr)
		{
		case DS_OK:break;
		case DSERR_CONTROLUNAVAIL:	OutputDebugString(L"onSound::volume::set DSERR_CONTROLUNAVAIL\n");break;
		case DSERR_GENERIC:			OutputDebugString(L"onSound::volume::set DSERR_GENERIC\n");break;
		case DSERR_INVALIDPARAM:	OutputDebugString(L"onSound::volume::set DSERR_INVALIDPARAM\n");break;
		case DSERR_PRIOLEVELNEEDED:	OutputDebugString(L"onSound::volume::set DSERR_PRIOLEVELNEEDED\n");break;
		default:					OutputDebugString(L"onSound::volume::set Unknown\n");break;
		}
		return ret;
	}
	return 0;
}

DWORD vnSound::getFrequency()
{
	if(pDSBuffer)
	{
		DWORD ret=0;
		HRESULT hr = pDSBuffer->GetFrequency(&ret);
		return ret;
	}
	return 0;
}

