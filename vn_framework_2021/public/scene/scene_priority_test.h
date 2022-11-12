#pragma once

class ScenePriorityTest : public vnScene
{
private:
	vnSprite	*pSprite[256];	//スプライトの配列
	int			SpriteMax;		//作成できるスプライトの最大数
	int			SpriteNum;		//作成したスプライトの数

public:
	//初期化
	bool initialize();
	//終了
	void terminate();

	//処理
	void execute();
	//描画
	void render();
};
