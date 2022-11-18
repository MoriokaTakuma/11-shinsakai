#pragma once

#define ENEMY_NUM 3
#define ENEMY_TYPE_NUM 3

#define ITEM_NUM	3

#define PL_HP_NUM	5
#define pl_tama_num 5

class SceneGame : public vnScene
{
private:
	vnSprite* pBg;
	vnSprite* pPlayer;
	vnSprite* pEnemy[ENEMY_NUM];
	vnSprite* pItem[ITEM_NUM];
	vnSprite* pitem2;
	vnSprite* pHp[PL_HP_NUM];
	vnSprite* pShadow;
	vnSprite* pPause;
	vnSprite* pPauseBlack;
	vnSprite* pWeapon;
	vnSprite* pGauge;
	vnSprite* pGaugeShitaji;
	vnSprite* pYa[pl_tama_num];

	//BGMÅESE
	vnSound* pBGM;
	vnSound* pAttackSE;

public:
	//èâä˙âª
	bool initialize();
	//èIóπ
	void terminate();
	//èàóù
	void execute();
	//ï`âÊ
	void render();
};

enum GameSpritePri
{
	PRIO_BG,
	PRIO_ENEMY_SHADOW_BACK,
	PRIO_ENEMY_BACK,
	PRIO_PLAYER_SHADOW,
	PRIO_PLAYER,
	PRIO_ENEMY_SHADOW_FRONT,
	PRIO_ENEMY_FRONT,
	PRIO_ENEMY_PAUSE,
};