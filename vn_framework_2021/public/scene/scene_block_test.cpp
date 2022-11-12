#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化
bool SceneBlockTest::initialize()
{
	//プレイヤーの作成
	pPlayer = new vnSprite(
		(float)SCREEN_CENTER_X/2, (float)SCREEN_CENTER_Y,
		64.0f, 64.0f,
		L"data/image/block.png",
		0.0f, 0.25f, 0.0f, 0.25f
	);
	//ブロックの作成
	pSprite = new vnSprite(
		(float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 
		64.0f, 64.0f, 
		L"data/image/block.png",
		0.0f, 0.25f, 0.0f, 0.25f
	);

	//作成したスプライトを基底クラス(vnSceneクラス)に登録
	registerObject(pPlayer);
	registerObject(pSprite);

	Cursor = 0;

	return true;
}

//終了
void SceneBlockTest::terminate()
{
	//スプライトの削除
	deleteObject(pPlayer);
	deleteObject(pSprite);
}

//処理関数
void SceneBlockTest::execute()
{
	float value_p = 1.0f;
	float value_s = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;

	//プレイヤーの操作
	if (vnKeyboard::on(DIK_W))
	{
		pPlayer->posY -= value_p;
	}
	if (vnKeyboard::on(DIK_S))
	{
		pPlayer->posY += value_p;
	}
	if (vnKeyboard::on(DIK_A))
	{
		pPlayer->posX -= value_p;
	}
	if (vnKeyboard::on(DIK_D))
	{
		pPlayer->posX += value_p;
	}

	//ブロックの操作
	if (vnKeyboard::trg(DIK_UP) && --Cursor<0)
	{
		Cursor = OperationMax-1;
	}
	else if (vnKeyboard::trg(DIK_DOWN) && ++Cursor >= OperationMax)
	{
		Cursor = 0;
	}
	else if (vnKeyboard::on(DIK_RIGHT))
	{
		switch (Cursor)
		{
		case PositionX:	pSprite->posX += value_p;	break;
		case PositionY:	pSprite->posY += value_p;	break;
		case ScaleX:	pSprite->scaleX += value_s;	break;
		case ScaleY:	pSprite->scaleY += value_s;	break;
		}
	}
	else if (vnKeyboard::on(DIK_LEFT))
	{
		switch (Cursor)
		{
		case PositionX:	pSprite->posX -= value_p;	break;
		case PositionY:	pSprite->posY -= value_p;	break;
		case ScaleX:	pSprite->scaleX -= value_s;	break;
		case ScaleY:	pSprite->scaleY -= value_s;	break;
		}
	}
	else if (vnKeyboard::on(DIK_SPACE))
	{
		switch (Cursor)
		{
		case PositionX:	pSprite->posX = SCREEN_CENTER_X;	break;
		case PositionY:	pSprite->posY = SCREEN_CENTER_Y;	break;
		case ScaleX:	pSprite->scaleX = 1.0f;	break;
		case ScaleY:	pSprite->scaleY = 1.0f;	break;
		}
	}

	//スプライトの衝突を検出する(めり込みの解消も行う)
	bool hit = isCollide(pPlayer, pSprite);

	//結果のデバッグ表示
	if (hit == true)
	{
		vnFont::print(640, 20, L"hit : true");
	}
	else
	{
		vnFont::print(640, 20, L"hit : false");
	}


	float x = 30.0f;
	float y = 10.0f;
	int line = 0;
	int interval = 16;
	vnFont::print(x-20.0f, (float)(y + Cursor * interval), L"→");
	vnFont::print(x, y + (float)(line++ * interval), L"Position X : %.3f", pSprite->posX);
	vnFont::print(x, y + (float)(line++ * interval), L"Position Y : %.3f", pSprite->posY);
	vnFont::print(x, y + (float)(line++ * interval), L"Scale X    : %.3f", pSprite->scaleX);
	vnFont::print(x, y + (float)(line++ * interval), L"Scale Y    : %.3f", pSprite->scaleY);

	vnScene::execute();
}

//描画関数
void SceneBlockTest::render()
{
	//シーン基底クラスの描画
	vnScene::render();
}



//スプライト同士の衝突を検出する
//戻り値 true : 衝突している、false : 衝突していない
//引数
//vnSprite *p1 : 衝突を検出する1つ目のスプライト
//vnSprite *p2 : 衝突を検出する2つ目のスプライト
//bool resolve : 衝突の解消を行う(p1を動かして、p2は固定する)
bool SceneBlockTest::isCollide(vnSprite* p1, vnSprite* p2, bool resolve)
{
	//引数のエラーチェック
	if (p1 == NULL || p2 == NULL)
	{
		return false;
	}

	//戻り値用の変数
	bool hit = false;

	//衝突の検出
	float rx = 0.0f;	//重なっている長方形(Rectangle)の幅
	float ry = 0.0f;	//重なっている長方形(Rectangle)の高さ

	//横(X)方向

	//スプライトの中心同士の座標の差分(differencial)
	float dx = fabsf(p1->posX - p2->posX);
	//2つのスプライトの幅(半分)をスケールの合計
	float sx = p1->sizeX * 0.5f * p1->scaleX + p2->sizeX * 0.5f * p2->scaleX;

	if (dx < sx)	//差分が幅より小さければ横(X)方向で当たっている
	{
		//縦(Y)方向

		//スプライトの中心同士の座標の差分(differencial)
		float dy = fabsf(p1->posY - p2->posY);
		//2つのスプライトの高さ(半分)をスケールの合計
		float sy = p1->sizeY * 0.5f * p1->scaleY + p2->sizeY * 0.5f * p2->scaleY;

		if (dy < sy)	//差分が高さより小さければ縦(Y)方向で当たっている
		{
			hit = true;

			//重なっている長方形のサイズを保持
			rx = sx - dx;
			ry = sy - dy;

			vnFont::print(640, 40, L"r : %.3f, %.3f", rx, ry);
		}
	}

	//衝突がある場合、めり込みを解消する
	//p1を動かして、p2は動かさない
	if (hit == true && resolve == true)
	{
		if (rx < ry)
		{	//重なった領域(長方形)が縦長→左右から衝突
			if (p1->posX < p2->posX)
			{	//p1が左から衝突
				vnFont::print(640, 60, L"from Left");
				p1->posX -= rx;
				//p2のUVで矢印を表示(←)
				p2->vtx[0].u = 0.5f;	p2->vtx[0].v = 0.25f;
				p2->vtx[1].u = 0.75f;	p2->vtx[1].v = 0.25f;
				p2->vtx[2].u = 0.5f;	p2->vtx[2].v = 0.5f;
				p2->vtx[3].u = 0.75f;	p2->vtx[3].v = 0.5f;
			}
			else
			{	//p1が右から衝突
				vnFont::print(640, 60, L"from Right");
				p1->posX += rx;
				//p2のUVで矢印を表示(→)
				p2->vtx[0].u = 0.0f;	p2->vtx[0].v = 0.25f;
				p2->vtx[1].u = 0.25f;	p2->vtx[1].v = 0.25f;
				p2->vtx[2].u = 0.0f;	p2->vtx[2].v = 0.5f;
				p2->vtx[3].u = 0.25f;	p2->vtx[3].v = 0.5f;
			}
		}
		else
		{	//重なった領域(長方形)が横長→上下から衝突
			if (p1->posY < p2->posY)
			{	//p1が上から衝突
				vnFont::print(640, 60, L"from Top");
				p1->posY -= ry;
				//p2のUVで矢印を表示(↑)
				p2->vtx[0].u = 0.75f;	p2->vtx[0].v = 0.25f;
				p2->vtx[1].u = 1.0f;	p2->vtx[1].v = 0.25f;
				p2->vtx[2].u = 0.75f;	p2->vtx[2].v = 0.5f;
				p2->vtx[3].u = 1.0f;	p2->vtx[3].v = 0.5f;
			}
			else
			{	//p1が下から衝突
				vnFont::print(640, 60, L"from Bottom");
				p1->posY += ry;
				//p2のUVで矢印を表示(↓)
				p2->vtx[0].u = 0.25f;	p2->vtx[0].v = 0.25f;
				p2->vtx[1].u = 0.5f;	p2->vtx[1].v = 0.25f;
				p2->vtx[2].u = 0.25f;	p2->vtx[2].v = 0.5f;
				p2->vtx[3].u = 0.5f;	p2->vtx[3].v = 0.5f;
			}
		}
	}
	else
	{
		//p2のUVで無地のブロックを表示
		p2->vtx[0].u = 0.0f;	p2->vtx[0].v = 0.0f;
		p2->vtx[1].u = 0.25f;	p2->vtx[1].v = 0.0f;
		p2->vtx[2].u = 0.0f;	p2->vtx[2].v = 0.25f;
		p2->vtx[3].u = 0.25f;	p2->vtx[3].v = 0.25f;
	}


	return hit;
}
