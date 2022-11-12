//--------------------------------------------------------------//
//	"vn_environment.h"											//
//		フレームワーク共通環境ヘッダー							//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//ライブラリ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

//共通ヘッダファイルのインクルード
#include <stdio.h>
#include <locale.h>
#include <assert.h>
#include <mmsystem.h>

//DirectXヘッダー
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#define	DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include <dsound.h>

#include <DirectXMath.h>
#include "directX/WICTextureLoader/WICTextureLoader.h"

//DirectXネームスペース
using namespace DirectX;

//一般マクロ
#define CLIENT_WIDTH	(1280)	//クライアント領域の幅(ピクセル)
#define CLIENT_HEIGHT	(720)	//クライアント領域の高さ(ピクセル)
#define SCREEN_WIDTH	CLIENT_WIDTH
#define SCREEN_HEIGHT	CLIENT_HEIGHT
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)

//リリース
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}

//フレームワークヘッダー
#include "directX/vn_shader.h"
#include "directX/vn_Direct3D.h"
#include "directX/vn_Direct2D.h"
#include "directX/vn_DirectInput.h"
#include "directX/vn_DirectSound.h"

#include "vn_mouse.h"
#include "vn_keyboard.h"
#include "vn_joystick.h"
#include "vn_sound.h"

#include "vn_Font.h"

#include "../public/vn_sprite.h"

#include "../public/scene/vn_sceneTable.h"

#include "vn_mainframe.h"
