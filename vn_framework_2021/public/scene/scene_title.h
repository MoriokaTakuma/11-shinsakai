#pragma once
class SceneTitle : public vnScene
{
private:
	vnSprite* pTitle;

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