#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化関数
bool SceneGameOver::initialize()
{
	 pGameOver= new vnSprite((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 1280.0f, 720.0f, L"data/image/GAMEOVER.png");
	//作成したスプライトを基底クラス（vnSceneクラス）に登録
	registerObject(pGameOver);
	return true;
}
//終了関数
void SceneGameOver::terminate()
{
	//スプライトの削除
	deleteObject(pGameOver);
}
//処理関数
void SceneGameOver::execute()
{
	//キー入力
	if (vnKeyboard::trg(DIK_SPACE))
	{
		switchScene(eSceneTable::Title);
	}
	vnScene::execute();
}
//描画関数	
void SceneGameOver::render()
{
	vnScene::render();
}