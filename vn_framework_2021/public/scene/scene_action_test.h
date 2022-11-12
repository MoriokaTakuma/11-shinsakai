#pragma once

#include "../object/block2D.h"
#include "../object/player2D.h"
#include "../object/item2D.h"

class SceneActionTest : public vnScene
{
private:
	//ゲームの状態
	enum eGameState
	{
		FadeOut,//フェードアウト
		Start,	//スタート
		Play,	//プレイ中
		Clear,	//ゲームクリア
		Over,	//ゲームオーバー
		FadeIn,	//フェードイン
	};
	eGameState gameState;

	vnSprite* pGameStart;	//ゲームスタートのロゴ
	vnSprite* pGameClear;	//ゲームクリアのロゴ
	vnSprite* pGameOver;	//ゲームオーバーのロゴ

	vnSprite* pFade;		//フェード用スプライト

	vnBlock2D* pBG;			//背景画像

	//フェードイン/アウトの情報
	float fadeCount;	//フェードが始まってからの時間
	float fadeInTime;	//フェードインに要する時間
	float fadeOutTime;	//フェードアウトに要する時間
	
	//ステージのスクロール情報
	float offsetX;
	float offsetY;

	//ステージの制限時間(フレーム)
	float timeLimit;

	vnBlock2D* pBlock[64];	//ブロックオブジェクト
	int	BlockMax;			//ブロック配列の数

	vnPlayer2D* pPlayer;	//プレイヤーオブジェクト

	vnItem2D* pItem[6];		//アイテムオブジェクトの配列

	//vnItem2D* pGoal;		//ゴールオブジェクト

	//vnItem2D* pFlag;		//中間オブジェクト

	//vnItem2D* pHeart;		//回復オブジェクト(残機数アップ)

	//vnItem2D* pClock;		//時計オブジェクト(制限時間増加)

	float startX;		//プレイヤーの初期座標(X)
	float startY;		//プレイヤーの初期座標(Y)
	float respawnX;		//プレイヤーの再開座標(X)
	float respawnY;		//プレイヤーの再開座標(Y)

	enum eCollideState	//衝突の状態を表す列挙子(enum)
	{
		None,		//衝突していない
		FromLeft,	//左側から衝突
		FromRight,	//右側から衝突
		FromTop,	//上側から衝突
		FromBottom,	//下側から衝突
	};
	//スプライト同士の衝突を検出する
	//戻り値 eCollideState : 衝突の状態
	//引数
	//vnBlock2D *p1 : 衝突を検出する1つ目のオブジェクト
	//vnBlock2D *p2 : 衝突を検出する2つ目のオブジェクト
	//bool resolve : 衝突の解消を行う(p1を動かして、p2は固定する)
	eCollideState isCollide(vnBlock2D* p1, vnBlock2D* p2, bool resolve = true);

	//ゲームの状態ごとのexecute関数
	void execute_fadeOut();
	void execute_start();
	void execute_play();
	void execute_clear();
	void execute_over();
	void execute_fadeIn();

	//ゲームオーバーの処理
	void gameOver();

	//ゲームのリセット
	void gameReset();

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
