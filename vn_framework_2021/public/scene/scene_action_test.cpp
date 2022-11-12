#include "../../framework.h"
#include "../../framework/vn_environment.h"

//オブジェクトのタイプごとの描画優先順位
enum eRenderPriority
{
	BG = 0,
	Block = 100,
	Player = 200,
	UI = 500,
	Fade = 1000,
};

//初期化関数
bool SceneActionTest::initialize()
{
	//配列の数を計算
	BlockMax = sizeof(pBlock) / sizeof(vnSprite*);

	//配列の全要素をNULLクリア
	for (int i = 0; i < BlockMax; i++)
	{
		pBlock[i] = NULL;
	}

	//画面下に地面用にブロックを並べる
	for (int i = 0; i < 2560 / 64; i++)
	{
		if (i > 10 && i < 14)continue;

		if (i > 20 && i < 24)continue;

		pBlock[i] = new vnBlock2D(
			(float)(i*64+32), (float)(SCREEN_HEIGHT-32),
			64.0f, 64.0f,
			L"data/image/block.png",
			0.0f, 0.25f, 0.0f, 0.25f
		);
		pBlock[i]->setRenderPriority(eRenderPriority::Block);
		registerObject(pBlock[i]);
	}

	pBG = new vnBlock2D((float)SCREEN_WIDTH, (float)SCREEN_CENTER_Y,
		2560.0f, 720.0f,
		L"data/image/background.png");
	pBG->setRenderPriority(eRenderPriority::BG);
	registerObject(pBG);

	//ゴールオブジェクトの作成
	pItem[0] = new vnItem2D(2560.0f - 256.0f, SCREEN_HEIGHT - 64.0f - 256.0f,
		512.0f, 512.0f,
		L"data/image/goal_object.png");
	pItem[0]->setRenderPriority(eRenderPriority::Block);
	registerObject(pItem[0]);

	pItem[0]->setType(vnItem2D::eItemType::Goal);

	//中間オブジェクトの作成
	pItem[1] = new vnItem2D(vnItem2D::eItemType::Flag, (float)(17 * 64 + 32), (float)(SCREEN_HEIGHT - 32 - 64));
	pItem[1]->setRenderPriority(eRenderPriority::Block);
	registerObject(pItem[1]);

	//回復オブジェクトの作成
	pItem[2] = new vnItem2D(vnItem2D::eItemType::Heart, (float)(2 * 64 + 32), (float)(SCREEN_HEIGHT - 32 - 64 * 5));
	pItem[2]->setRenderPriority(eRenderPriority::Block);
	registerObject(pItem[2]);
	//汎用数値の設定(回復する残機数)
	pItem[2]->setValue(1.0f);

	//時計オブジェクトの作成
	pItem[3] = new vnItem2D(vnItem2D::eItemType::Clock, (float)(8 * 64 + 32), (float)(SCREEN_HEIGHT - 32 - 64));
	pItem[3]->setRenderPriority(eRenderPriority::Block);
	registerObject(pItem[3]);
	//汎用数値の設定(回復する制限時間)
	pItem[3]->setValue((float)(60 * 180));

	//鍵オブジェクトの作成
	pItem[4] = new vnItem2D(vnItem2D::eItemType::Key, (float)(5 * 64 + 32), (float)(SCREEN_HEIGHT - 32 - 64 * 3));
	pItem[4]->setRenderPriority(eRenderPriority::Block);
	registerObject(pItem[4]);

	//南京錠オブジェクトの作成
	pItem[5] = new vnItem2D(vnItem2D::eItemType::Close, (float)(17 * 64 + 32), (float)(SCREEN_HEIGHT - 32 - 64 * 4));
	pItem[5]->setRenderPriority(eRenderPriority::Block);
	registerObject(pItem[5]);

	//関連アイテムの結びつけ
	pItem[4]->setRelative(pItem[5]);	//鍵の取得で、南京錠 閉→開

	//ゲームスタートロゴの作成
	pGameStart = new vnSprite(SCREEN_CENTER_X, SCREEN_CENTER_Y, 512.0f, 256.0f,
		L"data/image/game_start.png");
	pGameStart->setRenderPriority(eRenderPriority::UI);
	registerObject(pGameStart);


	//ゲームクリアロゴの作成
	pGameClear = new vnSprite(SCREEN_CENTER_X, SCREEN_CENTER_Y, 512.0f, 256.0f,
		L"data/image/game_clear.png");
	pGameClear->setRenderPriority(eRenderPriority::UI);
	registerObject(pGameClear);

	pGameClear->setRenderEnable(false);	//非表示にする

	//ゲームオーバーロゴの作成
	pGameOver = new vnSprite(SCREEN_CENTER_X, SCREEN_CENTER_Y, 512.0f, 256.0f,
		L"data/image/game_over.png");
	pGameOver->setRenderPriority(eRenderPriority::UI);
	registerObject(pGameOver);

	pGameOver->setRenderEnable(false);	//非表示にする

	//フェード用スプライトの作成
	pFade = new vnSprite(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, L"");
	pFade->setRenderPriority(eRenderPriority::Fade);
	registerObject(pFade);

	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		pFade->vtx[i].r = 0.0f;
		pFade->vtx[i].g = 0.0f;
		pFade->vtx[i].b = 0.0f;
		pFade->vtx[i].a = 1.0f;
	}



	////ブロックの作成
	//pBlock[0] = new vnSprite(
	//	(float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y+100.0f,
	//	64.0f, 64.0f,
	//	L"data/image/block.png",
	//	0.0f, 0.25f, 0.0f, 0.25f
	//);
	//pBlock[1] = new vnSprite(
	//	(float)SCREEN_CENTER_X+100.0f, (float)SCREEN_CENTER_Y + 100.0f,
	//	64.0f, 64.0f,
	//	L"data/image/block.png",
	//	0.0f, 0.25f, 0.0f, 0.25f
	//);

	//プレイヤーの初期座標を設定
	startX = 640.0f;
	startY = 500.0f;
	//プレイヤーの再開座標を設定
	respawnX = startX;
	respawnY = startY;

	//プレイヤーオブジェクトの作成
	pPlayer = new vnPlayer2D(startX, startY,
		64.0f, 64.0f,
		L"data/image/face_icon.png",
		0.0f, 0.25f, 0.0f, 0.25f
	);

	//作成したスプライトを基底クラス(vnSceneクラス)に登録
	pPlayer->setRenderPriority(eRenderPriority::Player);
	registerObject(pPlayer);
	//registerObject(pBlock[0]);
	//registerObject(pBlock[1]);

	//フェードイン/アウトの情報
	fadeCount = 0.0f;
	fadeInTime = 60.0f;
	fadeOutTime = 60.0f;

	//ステージのスクロール情報
	offsetX = 0.0f;
	offsetY = 0.0f;

	//ステージの制限時間
	timeLimit = 60.0f;

	//ゲームの初期状態
	//gameState = eGameState::Start;
	gameState = eGameState::FadeOut;

	//プレイヤーが動かないようにしておく
	pPlayer->setExecuteEnable(false);




	/*

	//ステージの情報を出力(Visual Studio の出力パネル)

	//スタート位置
	vnFont::output(L"StartPosition : (%.0f, %.0f)\n", startX, startY);
	//制限時間
	vnFont::output(L"TimeLimit : (%.0f)\n", timeLimit);

	//ブロックの配置
	for (int i = 0, n=0; i < BlockMax; i++)
	{
		if (pBlock[i] == NULL)continue;

		vnFont::output(L"Block %d : (%.0f, %.0f)\n", n, pBlock[i]->posX, pBlock[i]->posY);
		n++;
	}

	//アイテムの配置
	for (int i = 0, n = 0; i < 4; i++)
	{
		if (pItem[i] == NULL)continue;

		switch (pItem[i]->getType())
		{
		case vnItem2D::eItemType::None:
			vnFont::output(L"Item(None) %d : (%.0f, %.0f)\n", n, pItem[i]->posX, pItem[i]->posY);
			break;
		case vnItem2D::eItemType::Flag:
			vnFont::output(L"Item(Flag) %d : (%.0f, %.0f)\n", n, pItem[i]->posX, pItem[i]->posY);
			break;
		case vnItem2D::eItemType::Goal:
			vnFont::output(L"Item(Goal) %d : (%.0f, %.0f)\n", n, pItem[i]->posX, pItem[i]->posY);
			break;
		case vnItem2D::eItemType::Heart:
			vnFont::output(L"Item(Heart) %d : (%.0f, %.0f)\n", n, pItem[i]->posX, pItem[i]->posY);
			break;
		case vnItem2D::eItemType::Clock:
			vnFont::output(L"Item(Clock) %d : (%.0f, %.0f)\n", n, pItem[i]->posX, pItem[i]->posY);
			break;
		}
		n++;

	}
	*/
	
	return true;
}

//終了関数
void SceneActionTest::terminate()
{
	//オブジェクトの削除
	deleteObject(pPlayer);
	for (int i = 0; i < BlockMax; i++)
	{
		deleteObject(pBlock[i]);
	}
	deleteObject(pBG);
	for (int i = 0; i < 6; i++)
	{
		deleteObject(pItem[i]);
	}
	deleteObject(pGameClear);
	deleteObject(pGameStart);
	deleteObject(pGameOver);
	deleteObject(pFade);
}

//処理関数
void SceneActionTest::execute()
{
	switch (gameState)
	{
	case eGameState::FadeOut:
		execute_fadeOut();
		vnFont::print(10, 10, L"GameState : FadeOut");
		break;
	case eGameState::Start:
		execute_start();
		vnFont::print(10, 10, L"GameState : Start");
		break;
	case eGameState::Play:
		execute_play();
		vnFont::print(10, 10, L"GameState : Play");
		break;
	case eGameState::Clear:
		execute_clear();
		vnFont::print(10, 10, L"GameState : Clear");
		break;
	case eGameState::Over:
		execute_over();
		vnFont::print(10, 10, L"GameState : Over");
		break;
	case eGameState::FadeIn:
		execute_fadeIn();
		vnFont::print(10, 10, L"GameState : FadeIn");
		break;
	}

	//デバッグ情報
	vnFont::print(10, 50, L"Player(Stage) : (%.3f, %.3f)", pPlayer->StageX, pPlayer->StageY);
	vnFont::print(10, 70, L"Player(Screen) : (%.3f, %.3f)", pPlayer->posX, pPlayer->posY);
	vnFont::print(10, 110, L"offsetX = %.3f", offsetX);
	vnFont::print(10, 150, L"Remain : %d", pPlayer->getRemain());
	vnFont::print(10, 190, L"Time Limit : %f", timeLimit);

	vnScene::execute();
}

void SceneActionTest::execute_fadeOut()
{
	fadeCount += 1.0f;

	float alpha = fadeCount / fadeOutTime;	//fadeCount(0～60)→alpha(0～1)
	alpha = 1.0f - alpha;	//alpha(0～1)→alpha(1～0)

	if (fadeCount >= fadeOutTime)
	{	//一定時間が経過した
		gameState = eGameState::Start;

		fadeCount = 0.0f;

		alpha = 0.0f;

		pFade->setRenderEnable(false);
	}

	//フェード用スプライトの透明度の設定
	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		pFade->vtx[i].a = alpha;
	}
	/*
	* 線形補間(Linear Interpolation : Lerp)
	* count : 0 → 15   → 30  → 45   → 60
	* alpha : 1 → 0.75 → 0.5 → 0.25 → 0
	*/

	vnFont::print(10, 230, L"Fade Out : %.1f / %.1f", fadeCount, fadeOutTime);
}

void SceneActionTest::execute_start()
{
	//Enterキーを押すと、Play状態に遷移する
	if (vnKeyboard::trg(DIK_RETURN))
	{
		//ゲームの状態を"プレイ"にする
		gameState = eGameState::Play;

		//プレイヤーのexecuteを実行状態にして操作できるようにする
		pPlayer->setExecuteEnable(true);

		//ゲームスタートのロゴを非表示にする
		pGameStart->setRenderEnable(false);
	}
}

void SceneActionTest::execute_play()
{
	//制限時間の減少
	timeLimit -= 1.0f;
	//時間が無くなればゲームオーバーになる
	if (timeLimit <= 0.0f)
	{
		gameOver();
	}

	//ステージ下から落ちたら、スタート位置から再開する
	if (pPlayer->StageY > SCREEN_HEIGHT + pPlayer->sizeY * 0.5f)
	{
		//プレイヤーの残機数を取得
		int remain = pPlayer->getRemain();

		//残機数を1減らす
		remain--;

		if (remain <= 0)
		{
			gameOver();
		}
		else
		{
			//リスタート位置に戻して再開
			pPlayer->restart(respawnX, respawnY);
		}
		//プレイヤーの残機数を再設定
		pPlayer->setRemain(remain);
	}
	//ステージの左端に来たら、それ以上行けないようにする
	if (pPlayer->StageX < 0.0f + pPlayer->sizeX * 0.5f)
	{
		pPlayer->StageX = 0.0f + pPlayer->sizeX * 0.5f;
	}
	//ステージの右端に来たら、それ以上行けないようにする
	if (pPlayer->StageX > 2560.0f - pPlayer->sizeX * 0.5f)
	{
		pPlayer->StageX = 2560.0f - pPlayer->sizeX * 0.5f;
	}

	//プレイヤーとブロックの衝突
	for (int i = 0; i < BlockMax; i++)
	{
		if (pBlock[i] == NULL)continue;
		eCollideState hit = isCollide(pPlayer, pBlock[i]);
		switch (hit)
		{
		case FromLeft:	break;
		case FromRight:	break;
		case FromTop:
			pPlayer->landing();
			break;
		case FromBottom:
			pPlayer->heading();
			break;
		}
	}

	//アイテムの衝突とタイプごとの振る舞い
	for (int i = 0; i < 6; i++)
	{
		if (pItem[i]->isRenderEnable() == false)continue;

		eCollideState hit = isCollide(pPlayer, pItem[i], false);
		if (hit == eCollideState::None)continue;

		//enum eItemType
		//{
		//	None,	//タイプなし
		//	Goal,	//ゴールオブジェクト
		//	Flag,	//中間オブジェクト
		//	Heart,	//回復オブジェクト
		//	Clock,	//時計オブジェクト
		//	Max,
		//};
		pItem[i]->hit();

		switch (pItem[i]->getType())
		{
		case vnItem2D::eItemType::Goal:
			//ゲームの状態を"クリア"にする
			gameState = eGameState::Clear;
			//プレイヤーのexecuteを止めて、動かないようにする
			pPlayer->setExecuteEnable(false);
			//ゲームクリアのロゴを表示する
			pGameClear->setRenderEnable(true);
			break;
		case vnItem2D::eItemType::Flag:
			//再開座標を中間オブジェクトの座標に更新する
			respawnX = pItem[i]->StageX;
			respawnY = pItem[i]->StageY;
			break;
		case vnItem2D::eItemType::Heart:
			//プレイヤーの残機数を1増やす
			pPlayer->setRemain(pPlayer->getRemain() + (int)pItem[i]->getValue());
			break;
		case vnItem2D::eItemType::Clock:
			//制限時間を増やす
			timeLimit += pItem[i]->getValue();
			break;
		case vnItem2D::eItemType::Key:
		{	//関連アイテムが変化する(南京錠 閉→開)
			vnItem2D* p = pItem[i]->getRelative();	//鍵アイテムに結びつけられた南京錠オブジェクトを取得
			if (p == NULL)break;
			p->setType(vnItem2D::eItemType::Open);
			break;
		}
		}
	}


	//プレイヤーとゴールオブジェクトの衝突(めり込みの解消はしない)
	//eCollideState hit = isCollide(pPlayer, pItem[0], false);
	//if (hit != eCollideState::None)
	//{	//衝突があれば、ゲームの状態を"クリア"にする
	//	gameState = eGameState::Clear;

	//	//プレイヤーのexecuteを止めて、動かないようにする
	//	pPlayer->setExecuteEnable(false);

	//	//ゲームクリアのロゴを表示する
	//	pGameClear->setRenderEnable(true);
	//}

	////プレイヤーと中間オブジェクトの衝突(めり込みの解消はしない)
	//if (pItem[1]->isRenderEnable() == true)
	//{
	//	hit = isCollide(pPlayer, pItem[1], false);
	//	if (hit != eCollideState::None)
	//	{	//再開座標を中間オブジェクトの座標に更新する
	//		respawnX = pItem[1]->StageX;
	//		respawnY = pItem[1]->StageY;

	//		//再度取得できないように非表示にする
	//		pItem[1]->setRenderEnable(false);
	//	}
	//}

	////プレイヤーと回復オブジェクトの衝突(めり込みの解消はしない)
	//if(pItem[2]->isRenderEnable() == true)
	//{
	//	hit = isCollide(pPlayer, pItem[2], false);
	//	if (hit != eCollideState::None)
	//	{	//プレイヤーの残機数を1増やす
	//		pPlayer->setRemain(pPlayer->getRemain()+1);

	//		//再度取得できないように非表示にする
	//		pItem[2]->setRenderEnable(false);
	//	}
	//}

	////プレイヤーと時計オブジェクトの衝突(めり込みの解消はしない)
	//if (pItem[3]->isRenderEnable() == true)
	//{
	//	hit = isCollide(pPlayer, pItem[3], false);
	//	if (hit != eCollideState::None)
	//	{	//制限時間を増やす
	//		timeLimit += 60.0f*180.0f;

	//		//再度取得できないように非表示にする
	//		pItem[3]->setRenderEnable(false);
	//	}
	//}



	//プレイヤーのスクリーン座標を常に画面中央(640)に保つために、
	//オフセットを計算
	offsetX = 640.0f - pPlayer->StageX;

	//ステージ左端の処理
	if (offsetX > 0.0f)
	{
		offsetX = 0.0f;
	}
	//ステージ右端の処理
	if (offsetX < -1280.0f)
	{
		offsetX = -1280.0f;
	}

	//ソフトウェアリセット
	if (vnKeyboard::trg(DIK_R))
	{
		gameReset();
	}

	//ステージ座標からスクリーン座標へ変換
	//画面上の位置 = ステージ上の位置 + オフセット量
	vnBlock2D::setOffset(offsetX, offsetY);

}

void SceneActionTest::execute_clear()
{
	//Enterキーを押すと、Start状態に遷移する
	if (vnKeyboard::trg(DIK_RETURN))
	{
		gameState = eGameState::FadeIn;

		pFade->setRenderEnable(true);
//		gameReset();
	}
}

void SceneActionTest::execute_over()
{
	//Enterキーを押すと、Start状態に遷移する
	if (vnKeyboard::trg(DIK_RETURN))
	{
		gameState = eGameState::FadeIn;

		pFade->setRenderEnable(true);
		//		gameReset();
	}
}

void SceneActionTest::execute_fadeIn()
{
	fadeCount += 1.0f;

	float alpha = fadeCount / fadeInTime;	//fadeCount(0～60)→alpha(0～1)

	if (fadeCount >= fadeInTime)
	{	//一定時間が経過した
		gameState = eGameState::FadeOut;

		fadeCount = 0.0f;

		alpha = 1.0f;

		gameReset();
	}

	//フェード用スプライトの透明度の設定
	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		pFade->vtx[i].a = alpha;
	}

	vnFont::print(10, 230, L"Fade In : %.1f / %.1f", fadeCount, fadeInTime);
}



//描画関数
void SceneActionTest::render()
{
	vnScene::render();
}


//ゲームオーバーの処理
void SceneActionTest::gameOver()
{
	//ゲームの状態を"ゲームオーバー"にする
	gameState = eGameState::Over;

	//プレイヤーのexecuteを止めて、動かないようにする
	pPlayer->setExecuteEnable(false);
	pPlayer->gameOver();

	//ゲームオーバーのロゴを表示する
	pGameOver->setRenderEnable(true);
}

//ゲームのリセット
void SceneActionTest::gameReset()
{
	//ゲームの状態を"フェードアウト"にする
	gameState = eGameState::FadeOut;
	//ゲームスタートのロゴを表示する
	pGameStart->setRenderEnable(true);

	//ゲームオーバーのロゴを非表示にする
	pGameOver->setRenderEnable(false);

	//ゲームクリアのロゴを非表示にする
	pGameClear->setRenderEnable(false);

	//画面のオフセットを再設定
	offsetX = 640.0f - startX/*pPlayer->StageX*/;

	//再開座標を開始座標に戻しておく
	respawnX = startX;
	respawnY = startY;

	//ステージ座標からスクリーン座標へ変換
	//画面上の位置 = ステージ上の位置 + オフセット量
	vnBlock2D::setOffset(offsetX, offsetY);

	//プレイヤーにリスタートするよう伝える
	//(先にオフセットの設定を済ませておく)
	pPlayer->restart(startX, startY);

	//残機数を初期状態にする
	pPlayer->setRemain(3);

	//プレイヤーのexecuteを止めて、動かないようにする
	pPlayer->setExecuteEnable(false);

	for (int i = 0; i < 6; i++)
	{
		pItem[i]->setRenderEnable(true);
	}

	//南京錠オブジェクトを閉じた状態に戻す
	pItem[5]->setType(vnItem2D::eItemType::Close);

	//中間オブジェクトを復帰
//	pFlag->setRenderEnable(true);

	//回復オブジェクトを復帰
//	pHeart->setRenderEnable(true);

	//時計オブジェクトを復帰
//	pClock->setRenderEnable(true);

	//制限時間のリセット
	timeLimit = 60.0f;
}



//スプライト同士の衝突を検出する
//戻り値 eCollideState : 衝突の状態
//引数
//vnBlock2D *p1 : 衝突を検出する1つ目のオブジェクト
//vnBlock2D *p2 : 衝突を検出する2つ目のオブジェクト
//bool resolve : 衝突の解消を行う(p1を動かして、p2は固定する)
SceneActionTest::eCollideState SceneActionTest::isCollide(vnBlock2D* p1, vnBlock2D* p2, bool resolve)
{
	//引数のエラーチェック
	if (p1 == NULL || p2 == NULL)
	{
		return eCollideState::None;
	}

	//戻り値用の変数
	eCollideState hit = eCollideState::None;

	//衝突の検出
	float rx = 0.0f;	//重なっている長方形(Rectangle)の幅
	float ry = 0.0f;	//重なっている長方形(Rectangle)の高さ

	//横(X)方向

	//スプライトの中心同士の座標の差分(differencial)
	float dx = fabsf(p1->StageX - p2->StageX);
	//2つのスプライトの幅(半分)をスケールの合計
	float sx = p1->sizeX * 0.5f * p1->scaleX + p2->sizeX * 0.5f * p2->scaleX;

	if (dx < sx)	//差分が幅より小さければ横(X)方向で当たっている
	{
		//縦(Y)方向

		//スプライトの中心同士の座標の差分(differencial)
		float dy = fabsf(p1->StageY - p2->StageY);
		//2つのスプライトの高さ(半分)をスケールの合計
		float sy = p1->sizeY * 0.5f * p1->scaleY + p2->sizeY * 0.5f * p2->scaleY;

		if (dy < sy)	//差分が高さより小さければ縦(Y)方向で当たっている
		{
			//当たっていないという情報から当たったという情報に書き換える
			hit = eCollideState::FromLeft;	

			//重なっている長方形のサイズを保持
			rx = sx - dx;
			ry = sy - dy;

			vnFont::print(640, 40, L"r : %.3f, %.3f", rx, ry);
		}
	}

	//衝突がある場合、めり込みを解消する
	//p1を動かして、p2は動かさない
	if (hit != eCollideState::None && resolve == true)
	{
		if (rx < ry)
		{	//重なった領域(長方形)が縦長→左右から衝突
			if (p1->StageX < p2->StageX)
			{	//p1が左から衝突
				vnFont::print(640, 60, L"from Left");
				p1->StageX -= rx;
				hit = eCollideState::FromLeft;
				//p2のUVで矢印を表示(←)
				//p2->vtx[0].u = 0.5f;	p2->vtx[0].v = 0.25f;
				//p2->vtx[1].u = 0.75f;	p2->vtx[1].v = 0.25f;
				//p2->vtx[2].u = 0.5f;	p2->vtx[2].v = 0.5f;
				//p2->vtx[3].u = 0.75f;	p2->vtx[3].v = 0.5f;
			}
			else
			{	//p1が右から衝突
				vnFont::print(640, 60, L"from Right");
				p1->StageX += rx;
				hit = eCollideState::FromRight;
				//p2のUVで矢印を表示(→)
				//p2->vtx[0].u = 0.0f;	p2->vtx[0].v = 0.25f;
				//p2->vtx[1].u = 0.25f;	p2->vtx[1].v = 0.25f;
				//p2->vtx[2].u = 0.0f;	p2->vtx[2].v = 0.5f;
				//p2->vtx[3].u = 0.25f;	p2->vtx[3].v = 0.5f;
			}
		}
		else
		{	//重なった領域(長方形)が横長→上下から衝突
			if (p1->StageY < p2->StageY)
			{	//p1が上から衝突
				vnFont::print(640, 60, L"from Top");
				p1->StageY -= ry;
				hit = eCollideState::FromTop;
				//p2のUVで矢印を表示(↑)
				//p2->vtx[0].u = 0.75f;	p2->vtx[0].v = 0.25f;
				//p2->vtx[1].u = 1.0f;	p2->vtx[1].v = 0.25f;
				//p2->vtx[2].u = 0.75f;	p2->vtx[2].v = 0.5f;
				//p2->vtx[3].u = 1.0f;	p2->vtx[3].v = 0.5f;
			}
			else
			{	//p1が下から衝突
				vnFont::print(640, 60, L"from Bottom");
				p1->StageY += ry;
				hit = eCollideState::FromBottom;
				//p2のUVで矢印を表示(↓)
				//p2->vtx[0].u = 0.25f;	p2->vtx[0].v = 0.25f;
				//p2->vtx[1].u = 0.5f;	p2->vtx[1].v = 0.25f;
				//p2->vtx[2].u = 0.25f;	p2->vtx[2].v = 0.5f;
				//p2->vtx[3].u = 0.5f;	p2->vtx[3].v = 0.5f;
			}
		}
	}
	else
	{
		//p2のUVで無地のブロックを表示
		//p2->vtx[0].u = 0.0f;	p2->vtx[0].v = 0.0f;
		//p2->vtx[1].u = 0.25f;	p2->vtx[1].v = 0.0f;
		//p2->vtx[2].u = 0.0f;	p2->vtx[2].v = 0.25f;
		//p2->vtx[3].u = 0.25f;	p2->vtx[3].v = 0.25f;
	}


	return hit;
}
