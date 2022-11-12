#pragma once
class SceneGameOver : public vnScene
{
private:
	vnSprite* pGameOver;
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