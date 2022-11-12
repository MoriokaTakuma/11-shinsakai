//--------------------------------------------------------------//
//	"vn_mainframe.h"											//
//		メインフレーム											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnMainFrame
{
public:

	static float clearColor[4];		//画面のクリアカラー(RGBA:0.0～1.0)

	//初期化
	static bool initialize(void *hWnd, void *hInst);

	//終了
	static void terminate();

	//実行
	static void execute();
};
