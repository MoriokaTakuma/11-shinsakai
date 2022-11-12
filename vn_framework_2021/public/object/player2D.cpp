//--------------------------------------------------------------//
//	"player2D.cpp"												//
//		ブロック基底クラス										//
//													2021/11/11	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

#include "player2D.h"

//コンストラクタ
vnPlayer2D::vnPlayer2D(float x, float y, float width, float height, const WCHAR* texture_file, float left_u, float right_u, float top_v, float bottom_v) : vnBlock2D(x, y, width, height, texture_file, left_u, right_u, top_v, bottom_v)
{
	//各種変数の初期化
	move = 5.0f;

	velocity = 0.0f;

	initVelocity = -20.0f;

	gravity = 0.5f;

	jump = true;
	jumpMove = 0.0f;

	dash = false;

	animationCount = 0.0f;

	remain = 3;
}

//デストラクタ
vnPlayer2D::~vnPlayer2D()
{
}

//処理
void vnPlayer2D::execute()
{
	//キー入力による操作
	float velocityX = 0.0f;
	dash = false;
	if (jump == false)
	{	//ジャンプしていないとき
		if (vnKeyboard::on(DIK_A) || vnKeyboard::on(DIK_LEFT))
		{	//左移動
			if (vnKeyboard::on(DIK_RSHIFT))
			{	//ダッシュ
				StageX -= move * 2.0f;
				dash = true;
			}
			else
			{
				StageX -= move;
			}
			velocityX = -move;
		}
		if (vnKeyboard::on(DIK_D) || vnKeyboard::on(DIK_RIGHT))
		{	//右移動
			if (vnKeyboard::on(DIK_RSHIFT))
			{	//ダッシュ
				StageX += move * 2.0f;
				dash = true;
			}
			else
			{
				StageX += move;
			}
			velocityX = move;
		}
	}
	else
	{	//ジャンプ中
		if (vnKeyboard::on(DIK_A) || vnKeyboard::on(DIK_LEFT))
		{	//左移動
			StageX -= move * 0.5f;
			velocityX = -move * 0.5f;
		}
		if (vnKeyboard::on(DIK_D) || vnKeyboard::on(DIK_RIGHT))
		{	//右移動
			StageX += move * 0.5f;
			velocityX = move * 0.5f;
		}
	}
	if (vnKeyboard::trg(DIK_RETURN) && jump == false)
	{	//ジャンプの処理
		velocity = initVelocity;	//鉛直上方向に飛び上がる力
		jump = true;			//ジャンプ状態にする
		jumpMove = velocityX;	//ジャンプした瞬間の横方向の移動量
	}

	//鉛直方向の力
	StageY += velocity;
	//速度に重力がかかり続ける
	velocity += gravity;



	//基底クラスのexecuteを呼び出す
	//オーバーライドしているので、明示的に呼び出さないと実行されない
	vnBlock2D::execute();	//ステージ座標→スクリーン座標
}


//再開(情報を初期化する)
void vnPlayer2D::restart(float x, float y)
{
	//プレイヤーの座標をスタート位置にする
	StageX = x;
	StageY = y;
	//その他の情報をリセット
	jump = true;
	velocity = 0.0f;

	//ゲームオーバーの画像をデフォルトに戻す
	vtx[0].u = 0.00f;	vtx[0].v = 0.00f;
	vtx[1].u = 0.25f;	vtx[1].v = 0.00f;
	vtx[2].u = 0.00f;	vtx[2].v = 0.25f;
	vtx[3].u = 0.25f;	vtx[3].v = 0.25f;

	//ステージ座標→スクリーン座標
	vnBlock2D::execute();
}

//ブロックの上に乗った
void vnPlayer2D::landing()
{
	jump = false;
	velocity = 0.0f;
}

//ブロックの下から当たった
void vnPlayer2D::heading()
{
	velocity = 0.0f;
}

//残機数の取得
int vnPlayer2D::getRemain()
{
	return remain;
}

//残機数の設定
void vnPlayer2D::setRemain(int value)
{
	remain = value;
}

//ゲームオーバー
void vnPlayer2D::gameOver()
{
	vtx[0].u = 0.75f;	vtx[0].v = 0.25f;
	vtx[1].u = 1.00f;	vtx[1].v = 0.25f;
	vtx[2].u = 0.75f;	vtx[2].v = 0.50f;
	vtx[3].u = 1.00f;	vtx[3].v = 0.50f;
}
