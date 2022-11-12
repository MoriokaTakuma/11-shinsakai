#pragma once

class SceneBlockTest : public vnScene
{
private:
	vnSprite	*pPlayer;	//プレイヤー
	vnSprite	*pSprite;	//ブロック

	//操作の種類
	enum eOperation
	{
		PositionX,
		PositionY,
		ScaleX,
		ScaleY,
		OperationMax,
	};

	int	Cursor;

	//スプライト同士の衝突を検出する
	//戻り値 true : 衝突している、false : 衝突していない
	//引数
	//vnSprite *p1 : 衝突を検出する1つ目のスプライト
	//vnSprite *p2 : 衝突を検出する2つ目のスプライト
	//bool resolve : 衝突の解消を行う(p1を動かして、p2は固定する)
	bool isCollide(vnSprite* p1, vnSprite* p2, bool resolve=true);
	
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
