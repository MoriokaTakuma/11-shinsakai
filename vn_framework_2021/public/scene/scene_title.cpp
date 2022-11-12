#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化関数
bool SceneTitle::initialize()
{
	pTitle = new vnSprite((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 1280.0f, 720.0f, L"data/image/TITLE.png");
	//作成したスプライトを基底クラス（vnSceneクラス）に登録
	registerObject(pTitle);
	return true;
}
//終了関数
void SceneTitle::terminate()
{
	//スプライトの削除
	deleteObject(pTitle);
}
//処理関数
void SceneTitle::execute()
{
	//キー入力
	if (vnKeyboard::trg(DIK_SPACE))
	{
		switchScene(eSceneTable::ActionGame);
	}
	vnScene::execute();
}
//描画関数	
void SceneTitle::render()
{
	vnScene::render();
}