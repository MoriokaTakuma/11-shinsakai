#pragma once

class SceneSpriteTest : public vnScene
{
private:
	vnSprite	*pSprite;

	vnSprite	*pPoint[4];

	//操作の種類
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
	//初期化
	bool initialize();

	//終了
	void terminate();

	//処理
	void execute();

	//描画
	void render();
};
