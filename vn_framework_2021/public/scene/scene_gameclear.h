#pragma once
#pragma once
class SceneGameClear : public vnScene
{
private:
	vnSprite* pGameClear;
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