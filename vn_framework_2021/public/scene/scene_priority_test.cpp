#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化関数
bool ScenePriorityTest::initialize()
{
	//作成できるスプライトの最大数(配列の数)
	//配列全体のバイト数を要素一つのバイト数で割る
	SpriteMax = sizeof(pSprite) / sizeof(vnSprite*);

	//ポインタの初期化(NULLクリア)
	for (int i = 0; i < SpriteMax; i++)
	{
		pSprite[i] = NULL;
	}

	//作成したスプライトの数
	SpriteNum = 0;

	return true;
}

//終了関数
void ScenePriorityTest::terminate()
{
	//生成したスプライトの削除
	for (int i = 0; i < SpriteMax; i++)
	{
		if (pSprite[i] == NULL)continue;
		deleteObject(pSprite[i]);
	}
}

//処理関数
void ScenePriorityTest::execute()
{
	vnFont::print(10, 10, L"Priority Test : %d", SpriteNum);
	/*
	
	マウスを左クリックしたところに、スプライトが生成される。
	スプライトの画像は、ランダムで0～9の数字のテクスチャが張られる。

	[追加仕様]ドラッグでつかんで移動させたり、右クリックで消したり...
	*/

	//マウスの左クリック
	if (vnMouse::trg() == true)
	{
		int disp = rand() % 10;	//0～9までのランダム値を得る

		//0～9の画像に対応するuvを計算
		float interval_u = 1.0f / 4.0f;	//画像ごとの間隔(u方向)
		float interval_v = 1.0f / 4.0f;	//画像ごとの間隔(v方向)
		float start_u = (float)(disp % 4) * interval_u;	//設定したいu値の左側の値
		float start_v = (float)(disp / 4) * interval_v;	//設定したいv値の上側の値

		//マウスカーソルの座標を取得
		float mx = (float)vnMouse::getX();
		float my = (float)vnMouse::getY();
		//スプライトを生成
		pSprite[SpriteNum] = new vnSprite(mx, my, 64.0f, 64.0f, 
			L"data/image/number_icon.png", 
			start_u, start_u+interval_u, start_v, start_v+interval_v);
		//描画優先順位の設定
		pSprite[SpriteNum]->setRenderPriority(10-disp);
		
		//作成したスプライトを登録
		registerObject(pSprite[SpriteNum]);
		
		//スプライトの数を1増やす(increment)
		SpriteNum++;
	}

	vnScene::execute();
}

//描画関数
void ScenePriorityTest::render()
{
	vnScene::render();
}
