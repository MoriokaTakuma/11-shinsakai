//--------------------------------------------------------------//
//	"block2D.h"													//
//		ブロック基底クラス										//
//													2021/11/09	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once	//複数のcppでインクルードされても、1回のみしかインクルードされないようにする
				//インクルードガード

class vnBlock2D : public vnSprite
{
//protected:	//自身と、派生先のクラスのみ参照可能
public:
	float	StageX;	//ステージ内での座標(X)
	float	StageY;	//ステージ内での座標(Y)

private:	//自身のみ参照可能

	//※staticをつけて、全vnBlock2Dオブジェクトに共有する一つだけの変数にする
	static float	OffsetX;	//ステージのスクロール量(X)
	static float	OffsetY;	//ステージのスクロール量(Y)

public:	//どこからでも参照可能

	//コンストラクタ
	vnBlock2D(float x, float y, float width, float height, const WCHAR* texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	//デストラクタ
	virtual ~vnBlock2D();

	//処理(基底クラスの関数をオーバーライド)
	virtual void execute();

	//オフセット量を設定する関数(※static変数に設定するので、この関数自身もstaticにする)
	static void setOffset(float ofsX, float ofsY);

};
