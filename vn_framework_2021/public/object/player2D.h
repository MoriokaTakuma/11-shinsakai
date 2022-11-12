//--------------------------------------------------------------//
//	"player2D.h"												//
//		ブロック基底クラス										//
//													2021/11/11	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once	//複数のcppでインクルードされても、1回のみしかインクルードされないようにする
				//インクルードガード

class vnPlayer2D : public vnBlock2D
{
private:
	float move;			//横移動の速度

	float velocity;		//速度(鉛直方向)
	float initVelocity;	//初速度

	float gravity;		//重力加速度

	bool jump;			//ジャンプ中
	float jumpMove;		//ジャンプ中の左右移動量

	bool dash;			//ダッシュ中

	float animationCount;	//アニメーション(画像切り替えのカウンタ)

	int remain;			//残機数

public:
	//コンストラクタ
	vnPlayer2D(float x, float y, float width, float height, const WCHAR* texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	//デストラクタ
	virtual ~vnPlayer2D();

	//処理(基底クラスの関数をオーバーライド)
	virtual void execute();

	//再開(情報を初期化する)
	void restart(float x, float y);

	//ブロックの上に乗った
	void landing();

	//ブロックの下から当たった
	void heading();

	//残機数の取得
	int getRemain();

	//残機数の設定
	void setRemain(int value);

	//ゲームオーバー
	void gameOver();

};
