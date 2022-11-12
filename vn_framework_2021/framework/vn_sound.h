//--------------------------------------------------------------//
//	"vn_sound.h"												//
//		WAVEファイル再生クラス									//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnSound
{
private:
	IDirectSoundBuffer *pDSBuffer;
	
public:
	//コンストラクタ
	vnSound(const WCHAR *path);
	//デストラクタ
	~vnSound();
	
	bool play(bool loop=false);	//再生
	bool stop();				//停止
	
	bool isPlaying();	//再生中か調べる
	bool isStopped();	//停止中か調べる
	
	//各種設定
	bool setVolume(LONG lVolume);			//減衰 : DSBVOLUME_MAX (減衰なし:0) から DSBVOLUME_MIN (無音:-10,000)
	bool setPan(LONG lPan);					//左右のチャンネルの相対ボリューム : DSBPAN_LEFT(-10,000) から DSBPAN_RIGHT(10,000)
	bool setFrequency(DWORD dwFrequency);	//オーディオ バッファが再生されている周波数(Hz 単位) / デフォルト値 : DSBFREQUENCY_ORIGINAL
	
	//各種取得
	LONG getVolume();
	LONG getPan();
	DWORD getFrequency();

};

