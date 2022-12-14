#pragma once
class SceneTitle : public vnScene
{
private:
	vnSprite* pTitle;
	//BGM・SE
	vnSound* pBgm_t;
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