#pragma once

class SceneSpriteTest : public vnScene
{
private:
	vnSprite	*pSprite;

	vnSprite	*pPoint[4];

	//‘€ì‚Ìí—Ş
	enum eOperation
	{
		PositionX,
		PositionY,
		ScaleX,
		ScaleY,
		Rotate,
		OperationMax,
	};

	int	Cursor;
	
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
