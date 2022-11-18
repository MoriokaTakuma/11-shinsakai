#pragma once
class SceneTitle : public vnScene
{
private:
	vnSprite* pTitle;
	//BGMESE
	vnSound* pBgm_t;
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