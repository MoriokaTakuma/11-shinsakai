//--------------------------------------------------------------//
//	"vn_mainframe.cpp"											//
//		メインフレーム											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

//起動時のシーン
extern eSceneTable initialScene;
//シーンのインスタンス
extern vnScene* pScene;

//固定フレームレートのためのパフォーマンスカウンター
bool performanceCounter;
LARGE_INTEGER freq, startCount, finishCount;
float count;

//静的変数
float vnMainFrame::clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//画面のクリアカラー

//メインフレームの初期化
bool vnMainFrame::initialize(void *hWnd, void *hInst)
{
	//パフォーマンスカウンタの初期化
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&finishCount, 0, sizeof(finishCount));
	count = 0.0f;

	performanceCounter = QueryPerformanceFrequency(&freq);

	//各種コンポーネントの初期化
	vnDirect3D::initialize(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

	vnDirect2D::initialize(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

	vnDirectInput::initialize(hInst);

	vnDirectSound::initialize(hWnd);

	vnShader::initialize();

	vnFont::initialize();

	vnSprite::initializeCommon();

	//シーンの作成
	switchScene();

	return true;
}

//メインフレームの終了
void vnMainFrame::terminate()
{
	//シーンの削除
	deleteScene();

	//各種コンポーネントの終了
	vnSprite::terminateCommon();

	vnFont::terminate();

	vnShader::terminate();

	vnDirectSound::terminate();

	vnDirectInput::terminate();

	vnDirect2D::terminate();

	vnDirect3D::terminate();
}

//メインフレームの処理
void vnMainFrame::execute()
{
	if (performanceCounter)
	{	//フレームの開始時間を取得
		QueryPerformanceCounter(&startCount);
	}

	//入力管理
	vnDirectInput::KeyManager();

	//通常処理開始
	pScene->execute();
	//通常処理終了

	//HOMEボタンで起動時のステージに戻る。
	if (vnKeyboard::trg(DIK_HOME))
	{
		switchScene(initialScene);
	}

	//レンダーターゲットをクリア
	vnDirect3D::clear();

	//ビューポートの設定
	D3D11_VIEWPORT	Viewport;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = (float)SCREEN_WIDTH;
	Viewport.Height = (float)SCREEN_HEIGHT;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	vnDirect3D::getDeviceContext()->RSSetViewports(1, &Viewport);

	//描画処理開始
	pScene->render();

	vnDirect2D::render();
	//描画処理終了

	//シーンの切り替え
	switchScene();

	if (performanceCounter)
	{	//フレームの終了時間を取得
		static float frame_msec = 1.0f / 60.0f;
		QueryPerformanceCounter(&finishCount);
		count = ((float)(finishCount.QuadPart - startCount.QuadPart) / (float)freq.QuadPart);

		if (count < frame_msec)
		{	//1/60秒が経つまで待つ
			DWORD wait = (DWORD)((frame_msec - count) * 1000.0f - 0.5f);
			timeBeginPeriod(1);
			Sleep(wait);
			timeEndPeriod(1);
		}
	}

	//プレゼンテーション
	vnDirect3D::present();
}
