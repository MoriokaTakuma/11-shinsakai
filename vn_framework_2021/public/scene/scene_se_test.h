//--------------------------------------------------------------//
//	"scene_se_test.h"											//
//		シーン：SEテスト										//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class SceneSeTest : public vnScene
{
private:

	//ファイルの数
	int fileNum;

	//wavファイルを鳴らすオブジェクト(1ファイル、1音 = 1オブジェクト)
	vnSound	**pSound;
	
public:
	//初期化
	bool initialize();

	//終了
	void terminate();

	//処理
	virtual void execute();

	//描画
	virtual void render();
};
