//--------------------------------------------------------------//
//	"vn_sceneTable.cpp"											//
//		シーンテーブル											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

//起動時のシーン
eSceneTable initialScene = eSceneTable::Boot;
//現在のシーン
eSceneTable currentScene = (eSceneTable)-1;
//切り替え予約のシーン
eSceneTable reserveScene = initialScene;
//シーンのインスタンス
vnScene *pScene = NULL;


//各シーンの名前
WCHAR SceneName[(int)eSceneTable::SceneMax][32] =
{
	L"Boot",
	L"Action Test",
	L"Block Test",
	L"Sprite Test",
	L"Priority Test",
	L"Joystick Test",
	L"SE Test",
	L"Font Test",
	L"ActionGame",
	L"Title",
	L"GameClear",
	L"GameOver",
};

//シーン切り替え関数(予約)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

//シーン切り替え関数(実処理)
void switchScene()
{
	//切り替え予約のチェック
	if(currentScene == reserveScene)
	{
		return;
	}
	
	//シーン削除
	deleteScene();

	//シーンの作成
	switch(reserveScene)
	{
	case eSceneTable::Boot:			pScene = new SceneBoot();			break;
	case eSceneTable::ActionTest:	pScene = new SceneActionTest();		break;
	case eSceneTable::BlockTest:	pScene = new SceneBlockTest();		break;
	case eSceneTable::SpriteTest:	pScene = new SceneSpriteTest();		break;
	case eSceneTable::PriorityTest:	pScene = new ScenePriorityTest();	break;
	case eSceneTable::JoystickTest:	pScene = new SceneJoystickTest();	break;
	case eSceneTable::SeTest:		pScene = new SceneSeTest();			break;
	case eSceneTable::FontTest:		pScene = new SceneFontTest();		break;
	case eSceneTable::ActionGame:	pScene = new SceneGame();           break;
	case eSceneTable::Title:        pScene = new SceneTitle();          break;
	case eSceneTable::GameClear:    pScene = new SceneGameClear();      break;
	case eSceneTable::GameOver:     pScene = new SceneGameOver();       break;
	}
	
	//現在のシーンを設定
	currentScene = reserveScene;

	//シーンの初期化
	pScene->initialize();
}

//シーン削除
void deleteScene()
{
	if (pScene != NULL)
	{
		//シーンの終了
		pScene->terminate();
		delete pScene;
		pScene = NULL;
	}
}
