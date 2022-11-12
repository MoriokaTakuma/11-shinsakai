#pragma once

class vnItem2D : public vnBlock2D
{
public:
	enum eItemType
	{
		None,	//タイプなし
		Goal,	//ゴールオブジェクト
		Flag,	//中間オブジェクト
		Heart,	//回復オブジェクト
		Clock,	//時計オブジェクト
		Key,	//鍵オブジェクト
		Open,	//南京錠(開)
		Close,	//南京錠(閉)
		Max,
	};

	struct stUV
	{
		float u;
		float v;
	};

private:
	eItemType type;

	//汎用的な変数(タイプごとに役割が異なる)
	vnItem2D*	pRelative;	//このアイテムに関連するアイテムのポインタ
	float		Value;		//アイテムごとに固有の値

	//アイテムタイプごとの規定のUV値
	//static(静的変数) : プログラム実行中に変わることのない値
	//オブジェクト自体を作成する/しないにかかわらず、常に一つだけ存在する変数
	//※static変数はcppでの再宣言が必要(一緒に初期化も)
	static stUV defaultUV[eItemType::Max];

public:
	//コンストラクタ
	vnItem2D(float x, float y, float width, float height, const WCHAR* texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	vnItem2D(eItemType t, float x, float y, float width=64.0f, float height=64.0f);

	//デストラクタ
	virtual ~vnItem2D();

	//アイテムタイプの取得
	eItemType getType();

	//アイテムタイプの設定
	void setType(eItemType t);

	//関連アイテムの取得
	vnItem2D* getRelative();

	//関連アイテムの設定
	void setRelative(vnItem2D* p);

	//汎用数値の取得
	float getValue();

	//汎用数値の設定
	void setValue(float v);

	//プレイヤーに触れた時の振る舞い
	void hit();
};