#pragma once
#pragma once
class SceneGameClear : public vnScene
{
private:
	vnSprite* pGameClear;
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