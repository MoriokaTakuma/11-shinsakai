#include "../../framework.h"
#include "../../framework/vn_environment.h"

#include "item2D.h"

//静的メンバ変数の初期化
vnItem2D::stUV vnItem2D::defaultUV[eItemType::Max] =
{
	{0.0f,  0.0f},		//None,		//タイプなし
	{0.0f,  0.0f},		//Goal,		//ゴールオブジェクト
	{0.0f,  0.0f},		//Flag,		//中間オブジェクト
	{0.25f, 0.0f},		//Heart,	//回復オブジェクト
	{0.5f,  0.0f},		//Clock,	//時計オブジェクト
	{0.0f,  0.25f},		//Key,		//鍵オブジェクト
	{0.25f, 0.25f},		//Open,		//南京錠(開)
	{0.5f,  0.25f},		//Close,	//南京錠(閉)
};


//コンストラクタ
vnItem2D::vnItem2D(float x, float y, float width, float height, const WCHAR* texture_file, float left_u, float right_u, float top_v, float bottom_v) : vnBlock2D(x, y, width, height, texture_file, left_u, right_u, top_v, bottom_v)
{
	type = eItemType::None;

	//汎用変数の初期化
	pRelative = NULL;
	Value = 0.0f;
}

//コンストラクタ(オーバーロード)
vnItem2D::vnItem2D(eItemType t, float x, float y, float width, float height) : vnBlock2D(x, y, width, height, L"data/image/item_icon.png", 0.0f, 1.0f, 0.0f, 1.0f)
{
	//タイプの設定とUVの設定
	setType(t);

	//汎用変数の初期化
	pRelative = NULL;
	Value = 0.0f;
}


//デストラクタ
vnItem2D::~vnItem2D()
{
}

//アイテムタイプの取得
vnItem2D::eItemType vnItem2D::getType()
{
	return type;
}

//アイテムタイプの設定
void vnItem2D::setType(eItemType t)
{
	type = t;

	//タイプ毎の画像のUVを設定
	float su = defaultUV[type].u;	//Start(左側)のu座標
	float sv = defaultUV[type].v;	//Start(上側)のv座標
	float du = 0.25f;	//一つ当たりのu座標の差分(4x4の画像)
	float dv = 0.25f;	//一つ当たりのu座標の差分(4x4の画像)
	//ポリゴンの頂点のuvを設定
	vtx[0].u = su;		vtx[0].v = sv;		//左上
	vtx[1].u = su + dv;	vtx[1].v = sv;		//右上
	vtx[2].u = su;		vtx[2].v = sv + dv;	//左下
	vtx[3].u = su + dv;	vtx[3].v = sv + dv;	//右下
}


//関連アイテムの取得
vnItem2D* vnItem2D::getRelative()
{
	return pRelative;
}

//関連アイテムの設定
void vnItem2D::setRelative(vnItem2D* p)
{
	pRelative = p;
}

//汎用数値の取得
float vnItem2D::getValue()
{
	return Value;
}

//汎用数値の設定
void vnItem2D::setValue(float v)
{
	Value = v;
}



//プレイヤーに触れた時の振る舞い
void vnItem2D::hit()
{
	switch (type)
	{
	case Goal:
		break;
	case Flag:
	case Heart:
	case Clock:
		setRenderEnable(false);
		break;
	}
}

