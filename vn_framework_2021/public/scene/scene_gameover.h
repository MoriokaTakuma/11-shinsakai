#pragma once
class SceneGameOver : public vnScene
{
private:
	vnSprite* pGameOver;
public:
	//‰Šú‰»
	bool initialize();
	//I—¹
	void terminate();
	//ˆ—
	void execute();
	//•`‰æ
	void render();
};