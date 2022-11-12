//--------------------------------------------------------------//
//	"vn_scene.h"												//
//		シーン基底クラス										//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#define vnOBJECT2D_MAX	(256)	//登録できる2Dオブジェクトの最大数

class vnScene
{
private:
	//登録用オブジェクト配列
	vnSprite* pObject2D_Array[vnOBJECT2D_MAX];

	static bool UpdateRenderPriority;	//描画優先順位が変更されたので、ソートをする必要がある

public:
	//コンストラクタ
	vnScene();

	//デストラクタ
	virtual ~vnScene();
	
	//初期化
	virtual bool initialize()=0;

	//終了
	virtual void terminate()=0;

	//処理
	virtual void execute();

	//描画
	virtual void render();

	//描画優先順位が変更されたので、ソートをする必要がある
	static void setUpdateRenderPriority();

	//2Dオブジェクトの登録(配列の空きに入れる)
	bool registerObject(vnSprite* p);

	//2Dオブジェクトの破棄(配列から削除)
	void deleteObject(vnSprite* p);
};