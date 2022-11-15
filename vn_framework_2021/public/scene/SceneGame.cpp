#include "../../framework.h"
#include "../../framework/vn_environment.h"
//ゲーム関連の設定など
#define PI              3.14159265359

//ゲームのバランス調整用パラメータ
#define HIT_DISTANCE_X 40.0f
#define HIT_DISTANCE_Y 20.0f

#define ITEM_HIT_DISTANCE_X 40.0f
#define ITEM_HIT_DISTANCE_Y 30.0f		

//BG関係
#define MOVE_L_LIMIT    50.0f
#define MOVE_H_LIMIT	10.0f

float bg_offset_x;
float bg_offset_y;
//PLAYER関連
#define PL_MOVE_LIMIT_U 100.0f
#define PL_MOVE_LIMIT_D 650.0f
#define PL_DASH_MAX		15.0f
#define PL_MOVE_X_SPD   5.0f
#define PL_MOVE_Y_SPD   (PL_MOVE_X_SPD/2.0f)
#define PL_DASH_SPD     0.2
#define PL_JUMP_SPD     5.0f
#define PL_JUMP_HIGHT   100.0f
#define PL_HP_MAX		(PL_HP_NUM * 2)
#define PL_SHADOW_OFS	60.0f
#define PL_WEAPON_OFS   40.0f;
#define PL_ATTACK_DIS	60.0f;

int   pl_mode;
float player_global_x;
float player_global_y;
bool  pl_jump_f;
bool  pl_muteki_f;
bool  pl_dash;
float pl_jump_r;
int   pl_add_y;
int   pl_vec;
int   pl_anim_pat;
int   pl_muteki_cnt;
int   pl_hp;


//武器関係
bool  pl_attack;
int attck_anim ;
int pl_weapon_rot;
int pl_weapon_rot_add;

//オブジェクト関係
int obj_offset_y = 470;

//エネミー関連
#define EN_SHADOW_OFS	30.0f

float enemy_global_x[ENEMY_NUM];
float enemy_global_y[ENEMY_NUM];
int   enemy_type[ENEMY_NUM];
int	  enemy_anim;
int   enemy_vec;

//エネミーの初期位置
float enemy_init_pos[ENEMY_NUM][3] = {{1000.0f,650.0f,0},
									  {1500.0f,650.0f,1},
									  {2000.0f,650.0f,2}};

//アイテム関連
float item_global_x[ITEM_NUM];
float item_global_y[ITEM_NUM];
int   item_type    [ITEM_NUM];

//HP関連
#define HP_POS_X	    1000
#define HP_POS_X_DIS	60
#define HP_POS_Y		50
//アイテムの初期位置
float item_init_pos[ITEM_NUM][3] = {{1300.0f,650.0f,0},
									{1800.0f,650.0f,0},
									{2300.0f,650.0f,0}};

//PAUSE関連
bool game_pause_f;

//プロトタイプ宣言
void  GameInit(void);
float AngToRad(float);
bool HitCheck(float x1, float y1, float x2, float y2, float dis_x, float dis_y);

//初期化関数
bool SceneGame::initialize()
{

	srand(time(NULL));

	GameInit();

	pBg = new vnSprite(bg_offset_x,bg_offset_y, 2560.0f, 1440.0f, L"data/image/bg.png");
	pBg->setRenderPriority(GameSpritePri::PRIO_BG);

	pPlayer = new vnSprite(player_global_x,player_global_y, 128.0f, 128.0f, L"data/image/chara.png",0.0f,0.125f,0.0f,0.125);
	pPlayer->setRenderPriority(GameSpritePri::PRIO_PLAYER);
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		pEnemy[i] = new vnSprite(enemy_global_x[i],enemy_global_y[i], 64.0, 64.0f, L"data/image/face_icon.png", 0.0f, 0.25f, 0.25f, 0.5f);
		pEnemy[i]->setRenderPriority(GameSpritePri::PRIO_ENEMY_BACK);
	}
	for (int i = 0; i < ITEM_NUM; i++)
	{pItem[i] = new vnSprite(item_global_x[i], item_global_y[i], 64.0f, 64.0f, L"data/image/number_icon.png", 0.0f, 0.25f, 0.0f, 0.25);}
	for (int i = 0; i < PL_HP_NUM; i++)
	{pHp[i] = new vnSprite(HP_POS_X + HP_POS_X_DIS * i,HP_POS_Y, 64.0f, 64.0f, L"data/image/item.png", 0.4f, 0.5f, 0.5f, 0.75);}
	pShadow = new vnSprite(player_global_x, (float)SCREEN_CENTER_X + PL_SHADOW_OFS, 64.0f, 64.0f, L"data/image/shadow.png");
	pPauseBlack = new vnSprite(SCREEN_CENTER_X, SCREEN_CENTER_Y, CLIENT_WIDTH, CLIENT_HEIGHT, L"data/image/black.png");
	pPauseBlack->setRenderPriority(GameSpritePri::PRIO_ENEMY_PAUSE);
	pPauseBlack->vtx[0].a =
	pPauseBlack->vtx[1].a =
	pPauseBlack->vtx[2].a =
	pPauseBlack->vtx[3].a = 0.75f;
	pPauseBlack->setRenderEnable(false);
	pPause = new vnSprite(SCREEN_CENTER_X, SCREEN_CENTER_Y, 512.0f, 512.0f, L"data/image/game_start.png");
	pPause->setRenderPriority(GameSpritePri::PRIO_ENEMY_PAUSE);
	pPause->setRenderEnable(false);
	pWeapon = new vnSprite(player_global_x, (float)SCREEN_CENTER_Y, 64.0f,32.0f,L"data/image/weapon.png", 0.0f, 0.1f, 0.5f, 0.6f);
	pWeapon->setRenderPriority(pPlayer->getRenderPriority() + 1);

	//BGM・SE
	pAttackSE = new vnSound(L"data/sound/attack.wav");
	pBGM = new vnSound(L"data/sound/BGM.wav");
	

	//作成したスプライトを基底クラス（vnSceneクラス）に登録
	registerObject(pBg);
	registerObject(pPlayer);
	for (int i = 0; i < ENEMY_NUM; i++)
	{registerObject(pEnemy[i]);}
	for (int i = 0; i < ITEM_NUM; i++)
	{registerObject(pItem[i]);}
	for (int i = 0; i < PL_HP_NUM; i++)
	{registerObject(pHp[i]);}
	registerObject(pShadow);
	registerObject(pPauseBlack);
	registerObject(pPause);
	registerObject(pWeapon);

	//敵の種類番号が2であれば、最初は非行動・非表示
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy_type[i] == 2)
		{
			pEnemy[i]->setRenderEnable(false);
			pEnemy[i]->setExecuteEnable(false);
		}

		//敵の種類により、表示テクスチャを変更する
		pEnemy[i]->vtx[0].u =
		pEnemy[i]->vtx[2].u = 0.0f;
		pEnemy[i]->vtx[1].u =
		pEnemy[i]->vtx[3].u = 0.25f;

		pEnemy[i]->vtx[0].v =
		pEnemy[i]->vtx[1].v = 0.0f + 0.25 * (float)enemy_type[i];
		pEnemy[i]->vtx[2].v =
		pEnemy[i]->vtx[3].v = 0.25 + 0.25f * (float)enemy_type[i];
	}

	return true;
}

//終了関数
void SceneGame::terminate()
{  //スプライトの削除
	deleteObject(pBg);
	deleteObject(pPlayer);
	for (int i = 0; i < ENEMY_NUM; i++)
	{deleteObject(pEnemy[i]);}
	for (int i = 0; i < ITEM_NUM; i++)
	{deleteObject(pItem[i]);}
	for (int i = 0; i < PL_HP_NUM; i++)
	{deleteObject(pHp[i]);}
	deleteObject(pShadow);
	deleteObject(pWeapon);

	//BGM・SE
	delete pBGM;
	delete pAttackSE;
}

//処理関数
void SceneGame::execute()
{
	vnFont::print(10, 70, L"Player(Pos) : (%.3f, %.3f)", pPlayer->posX, pPlayer->posY);
	//vnFont::print(10, 90, L"global_pos_x,y : %d, %d"), player_global_x, player_global_y;
	vnFont::print(10, 110, L"attack_anim : %d", attck_anim);
	vnFont::print(10, 130, L"pl_attack:%d", pl_attack);
	vnFont::print(10, 150, L"pl_anim_pat:%d", pl_anim_pat);
	vnFont::print(10, 170, L"pl_mode:%d", pl_mode);

	//BGM処理
	if (pBGM->isStopped()) { pBGM->play(); }

	//キー入力
	if (vnKeyboard::trg(DIK_P)) //ポーズ
	{
		if (game_pause_f) 
		{
			game_pause_f = false;
			pPauseBlack->setRenderEnable(false);
			pPause->setRenderEnable(false);
		}
		else
		{
			game_pause_f = true;
			pPauseBlack->setRenderEnable(true);
			pPause->setRenderEnable(true);
		}
	}

	if (game_pause_f) return; //もしgame_pause_f状態なら処理を止める。

		//画面遷移
	if (vnKeyboard::trg(DIK_Q))
	{
		switchScene(eSceneTable::GameClear);
	}
	if (vnKeyboard::trg(DIK_E))
	{
		switchScene(eSceneTable::GameOver);
	}
	if (vnKeyboard::on(DIK_B))
	{
		switchScene(eSceneTable::Boot);
	}

	//移動関連
	pl_vec = 0;
	if (vnKeyboard::on(DIK_A))
	{
		player_global_x -= PL_MOVE_X_SPD;
		pl_vec = -1;
		if (vnKeyboard::on(DIK_LSHIFT))
		{
			player_global_x -= PL_MOVE_X_SPD/2;
			pl_vec = -1;
		}
	}
	if (vnKeyboard::on(DIK_D))
	{
		player_global_x += PL_MOVE_X_SPD;
		pl_vec = 1;
		if (vnKeyboard::on(DIK_LSHIFT))
		{
			player_global_x += PL_MOVE_X_SPD/2;
			pl_vec = 1;
		}
	}
	if (vnKeyboard::on(DIK_LSHIFT))//ダッシュフラグ
	{
		if (!pl_dash)
			pl_dash = true;
	}
	else pl_dash = false;
	/*
	if (vnKeyboard::on(DIK_UP))
	{
		player_global_y -= PL_MOVE_Y_SPD;

	}
	if (vnKeyboard::on(DIK_DOWN))
	{
		player_global_y += PL_MOVE_Y_SPD;
	}
	*/
	if (vnKeyboard::trg(DIK_SPACE))
	{
		if (!pl_jump_f)
		{
			pl_jump_f = true;
			pl_jump_r = 0.0f;
		}
	}
	
	if (vnKeyboard::trg(DIK_F))
	{
		if (!pl_attack)  //   攻撃アニメーション	 テスト
		{
			pl_attack = true;
			/*
			if (pPlayer->scaleX > 0.0f)
			{
				pl_weapon_rot = 240;
				pl_weapon_rot_add = 10;
			}
			else
			{
				pl_weapon_rot = 300;
				pl_weapon_rot_add = -10;
			}
			*/
			pAttackSE->play();
		}
	}
	
	switch (pl_mode)
	{
	case 0:
	
		//攻撃アニメーション
		if (pl_attack)
		{
			attck_anim++;

			pPlayer->vtx[0].u =                                                     //vtx[0].u = 左上のuの座標
				pPlayer->vtx[2].u = 0.5f + (float)(attck_anim / 8 % 8 % 4) * 0.125f;    //vtx[2].u = 左下のuの座標
			pPlayer->vtx[1].u =                                                     //vtx[1].u = 右上のuの座標
				pPlayer->vtx[3].u = 0.625f + (float)(attck_anim / 8 % 8 % 4) * 0.125f;  //vtx[3].u = 右下のuの座標

			pPlayer->vtx[0].v =                                                     //vtx[0].v = 左上のvの座標
				pPlayer->vtx[1].v = 0.0f + (float)(attck_anim / 8 % 8 % 4) + 0.125f;;   //vtx[1].v = 右上のvの座標
			pPlayer->vtx[2].v =                                                     //vtx[2].v = 左下のvの座標
				pPlayer->vtx[3].v = 0.125f + (float)(attck_anim / 8 % 8 % 4) + 0.125f;  //vtx[3].v = 右下のvの座標
			if (attck_anim > 30)
			{
				pl_attack = false;
				attck_anim = 0;
			}
		}
		else//立ち絵
		{
			pPlayer->vtx[0].u =
			pPlayer->vtx[2].u = 0.25f;
			pPlayer->vtx[1].u =
			pPlayer->vtx[3].u = 0.375f;

			pPlayer->vtx[0].v =
			pPlayer->vtx[1].v = 0.5f;
			pPlayer->vtx[2].v =
			pPlayer->vtx[3].v = 0.625;
		}
		break;
	case 1:
		if (pl_attack)
		{
			attck_anim++;

			pPlayer->vtx[0].u =                                                         //vtx[0].u = 左上のuの座標
				pPlayer->vtx[2].u = 0.5f + (float)(attck_anim / 8 % 8 % 4) * 0.125f;    //vtx[2].u = 左下のuの座標
			pPlayer->vtx[1].u =                                                         //vtx[1].u = 右上のuの座標
				pPlayer->vtx[3].u = 0.625f + (float)(attck_anim / 8 % 8 % 4) * 0.125f;  //vtx[3].u = 右下のuの座標

			pPlayer->vtx[0].v =                                                         //vtx[0].v = 左上のvの座標
				pPlayer->vtx[1].v = 0.25f + (float)(attck_anim / 8 % 8 % 4) + 0.125f;;  //vtx[1].v = 右上のvの座標
			pPlayer->vtx[2].v =                                                         //vtx[2].v = 左下のvの座標
				pPlayer->vtx[3].v = 0.365f + (float)(attck_anim / 8 % 8 % 4) + 0.125f;  //vtx[3].v = 右下のvの座標
			if (attck_anim > 30)
			{
				pl_attack = false;
				attck_anim = 0;
			}
		}
		else//立ち絵
		{
			pPlayer->vtx[0].u =
			pPlayer->vtx[2].u = 0.25f;
			pPlayer->vtx[1].u =
			pPlayer->vtx[3].u = 0.375f;

			pPlayer->vtx[0].v =
			pPlayer->vtx[1].v = 0.5f;
			pPlayer->vtx[2].v =
			pPlayer->vtx[3].v = 0.625;
		}
	}
	
	/**/

	if (pl_vec != 0)//移動アニメーション
	{																									   
		pPlayer->vtx[0].u =                                                      //vtx[0].u = 左上のuの座標
	    pPlayer->vtx[2].u = 0.0f + (float)(pl_anim_pat / 8 % 8 % 8) * 0.125f;    //vtx[2].u = 左下のuの座標
		pPlayer->vtx[1].u =                                                      //vtx[1].u = 右上のuの座標
		pPlayer->vtx[3].u = 0.125f + (float)(pl_anim_pat / 8 % 8 % 8) * 0.125;   //vtx[3].u = 右下のuの座標
																										   
		pPlayer->vtx[0].v =                                                      //vtx[0].v = 左上のvの座標      
		pPlayer->vtx[1].v = 0.375f + (float)(pl_anim_pat / 8 % 8 % 8) + 0.125f;  //vtx[1].v = 右上のvの座標
		pPlayer->vtx[2].v =                                                      //vtx[2].v = 左下のvの座標
	    pPlayer->vtx[3].v = 0.5f + (float)(pl_anim_pat / 8 % 8 % 8) + 0.125f;    //vtx[3].v = 右下のvの座標
		pPlayer->scaleX = (float)pl_vec;
		if (pl_dash)//ダッシュアニメーション
		{
			pPlayer->vtx[0].u =                                                      //vtx[0].u = 左上のuの座標
			pPlayer->vtx[2].u = 0.0f + (float)(pl_anim_pat / 8 % 8 % 4) * 0.125f;    //vtx[2].u = 左下のuの座標
			pPlayer->vtx[1].u =                                                      //vtx[1].u = 右上のuの座標
			pPlayer->vtx[3].u = 0.125f + (float)(pl_anim_pat / 8 % 8 % 4) * 0.125;   //vtx[3].u = 右下のuの座標

			pPlayer->vtx[0].v =                                                      //vtx[0].v = 左上のvの座標      
			pPlayer->vtx[1].v = 0.0f + (float)(pl_anim_pat / 8 % 8 % 4) + 0.125;	 //vtx[1].v = 右上のvの座標
			pPlayer->vtx[2].v =                                                      //vtx[2].v = 左下のvの座標
			pPlayer->vtx[3].v = 0.125f + (float)(pl_anim_pat / 8 % 8 % 4) + 0.125;   //vtx[3].v = 右下のvの座標
			pPlayer->scaleX = (float)pl_vec;
		}
	}
	pl_anim_pat++;

	//ワールド座標で左右の端を超えた際の処理
	if ((player_global_x - pPlayer->sizeX / 2.0f) < 0.0f)
	{
		player_global_x = pPlayer->sizeX / 2.0f;
	}
	if ((player_global_x + pPlayer->sizeX / 2.0f) > pBg->sizeX)
	{
		player_global_x = pBg->sizeX - pPlayer->sizeX / 2.0f;
	}
	
	/*ワールド座標で上下の端を越えた際の処理*/
	if (player_global_y < PL_MOVE_LIMIT_U) { player_global_y = PL_MOVE_LIMIT_U ;}
	if (player_global_y > PL_MOVE_LIMIT_D) { player_global_y = PL_MOVE_LIMIT_D; }
	

	float tmp_pl_pos_x = player_global_x + bg_offset_x - pBg->sizeX / 2.0f;
	float tmp_pl_pos_y = player_global_y + bg_offset_y;

	if (tmp_pl_pos_x < MOVE_L_LIMIT && bg_offset_x < pBg->sizeX / 2.0f - PL_MOVE_X_SPD)
	{
		bg_offset_x += PL_MOVE_X_SPD * 1.5;
	}
	if (tmp_pl_pos_x > SCREEN_CENTER_X && bg_offset_x > PL_MOVE_X_SPD)
	{
		bg_offset_x -= PL_MOVE_X_SPD;
	}
	
	/*
	if (tmp_pl_pos_y > MOVE_H_LIMIT)
	{
		bg_offset_y -= PL_MOVE_Y_SPD;
	}
	if (tmp_pl_pos_y < 750 )
	{
		bg_offset_y += PL_MOVE_Y_SPD;
	}
	*/


	//プレイヤー位置情報更新
	pPlayer->posX = player_global_x + bg_offset_x - pBg->sizeX / 2.0f;
	pPlayer->posY = player_global_y + bg_offset_y;

	pBg->posX = bg_offset_x;
	pBg->posY = bg_offset_y;


	//ジャンプ処理
	if (pl_jump_f)
	{
		pl_jump_r += PL_JUMP_SPD;
		pl_add_y = (int)(sinf(AngToRad(pl_jump_r)) * PL_JUMP_HIGHT);
		if (pl_jump_r > 180.0f)
		{
			pl_jump_f = false;
			pl_add_y = 0;
		}
		pPlayer->vtx[0].u =
		pPlayer->vtx[2].u = 0.375f;
		pPlayer->vtx[1].u =
		pPlayer->vtx[3].u = 0.5f;

		pPlayer->vtx[0].v =
		pPlayer->vtx[1].v = 0.75f;
		pPlayer->vtx[2].v =
		pPlayer->vtx[3].v = 0.875;
	}
	
	pPlayer->posY = player_global_y - pl_add_y;
	pShadow->posX = pPlayer->posX;
	pShadow->posY = player_global_y + PL_SHADOW_OFS;
	pShadow->scaleX =
	pShadow->scaleY = fabsf(cosf(AngToRad(pl_jump_r)))/2.0 + 0.5f;

	/*
	//武器のアニメーション
	pWeapon->posX = pPlayer->posX + cosf(AngToRad(pl_weapon_rot)) * PL_WEAPON_OFS;
	pWeapon->posY = pPlayer->posY + sinf(AngToRad(pl_weapon_rot)) * PL_WEAPON_OFS + 25;
	pWeapon->rot = AngToRad(pl_weapon_rot);
	pWeapon->setRenderEnable(pl_attack);
	pl_weapon_rot += pl_weapon_rot_add;
	if (pl_weapon_rot_add > 0){if (pl_weapon_rot > 400) { pl_attack = false; }}
	else{if (pl_weapon_rot < 140) {pl_attack = false;}}
	*/

	//エネミー位置更新（仮）
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		switch (enemy_type[i])
		{
		default:
		case 0:
			if (fabsf(player_global_y - enemy_global_y[i]) < 10.0f)
			{
				if (enemy_global_x[i] > player_global_x) { enemy_global_x[i] -= 5.0f; }
				else if (enemy_global_x[i] < player_global_x) { enemy_global_x[i] += 5.0f; }
			}
			else
			{
				if (enemy_global_y[i] > player_global_y) { enemy_global_y[i] -= 2.0f; }
				else if (enemy_global_y[i] < player_global_y) { enemy_global_y[i] += 2.0f;}
			}
			break;

		case 1:
#if 0
			if (enemy_global_x[i] >= player_global_x)
			{
				enemy_global_x[i] -= 2.0;
			}
			else if (enemy_global_x[i] <= player_global_x)
			{
				enemy_global_x[i] += 2.0;
			}
			if (enemy_global_y[i] > player_global_y)
			{
				enemy_global_y[i] -= 1.0;
			}
			else if(enemy_global_y[i] < player_global_y)
			{
				enemy_global_y[i] += 1.0;
			}
			/*if (pPlayer->scaleX == 1.0f && enemy_global_x[i] < player_global_x)
			{
				enemy_global_x[i] += 3.0f;
			}
			else if (pPlayer->scaleX == -1.0f && enemy_global_x[i] > player_global_x)
			{
				enemy_global_x[i] -= 3.0f;
			}*/
#endif
			if ((pPlayer->scaleX * (player_global_x - enemy_global_y[i])) >= 0)
			{
				float angle = 0.0f;
				angle = atan2f(player_global_y - enemy_global_y[i], player_global_x - enemy_global_x[i]);
				enemy_global_x[i] += cosf(angle) * 5.0f;
				enemy_global_y[i] += sinf(angle) * 5.0f;
			}
			break;

		case 2:
			if (enemy_global_x[i] < player_global_x - 150)
			{
				pEnemy[i]->setRenderEnable(true);
				pEnemy[i]->setExecuteEnable(true);
				//enemy_global_y[i] = player_global_y;
				enemy_global_y[i] = 180.0f;

				enemy_type[i] = 1;
			}
			break;
		}
		
		if (enemy_global_y[i] > player_global_y)
		{
			pEnemy[i]->setRenderPriority(GameSpritePri::PRIO_ENEMY_FRONT);
		}
		else
		{
			pEnemy[i]->setRenderPriority(GameSpritePri::PRIO_ENEMY_BACK);
		}


		pEnemy[i]->posX = enemy_global_x[i] + bg_offset_x - pBg->sizeX / 2.0f;
		pEnemy[i]->posY = enemy_global_y[i];
	}

	//アイテム位置更新(仮)
	for (int i = 0; i < ITEM_NUM; i++)
	{
		pItem[i]->posX = item_global_x[i] + bg_offset_x - pBg->sizeX / 2.0f;
		pItem[i]->posY = item_global_y[i];
	}

	//プレイヤーの攻撃と敵の当たり判定
	/*
	if (pl_attack)
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (!pEnemy[i]->isExecuteEnable()) continue ;
			if (HitCheck(player_global_x + 60 * pPlayer->scaleX, player_global_y, enemy_global_x[i], enemy_global_y[i], HIT_DISTANCE_X, HIT_DISTANCE_Y))
			{
				pEnemy[i]->setExecuteEnable(false);
				pEnemy[i]->setRenderEnable(false);
				if (pl_hp <= 0)
				{switchScene(eSceneTable::GameOver);}
			}
		}
	}

	//プレイヤーと敵の当たり判定
	if (!pl_muteki_f)
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (!pEnemy[i]->isExecuteEnable())continue;
			if (HitCheck(player_global_x, player_global_y, enemy_global_x[i], enemy_global_y[i], HIT_DISTANCE_X, HIT_DISTANCE_Y))
			{
				pl_muteki_f = true;
				pl_muteki_cnt = 150;
				pl_hp--;
				if (pl_hp <= 0) { switchScene(eSceneTable::GameOver); }
			}
		}
	}
	else
	{
		pPlayer->setRenderEnable((bool)(pl_muteki_cnt / 4 % 2));
		pl_muteki_cnt--;
		if (pl_muteki_cnt <= 0)
		{
			pl_muteki_cnt = 0;
			pl_muteki_f = false;
			pPlayer->setRenderEnable(true);
		}
	}
    */


	//プレイヤーとアイテムの当たり判定
	for (int i = 0; i < ITEM_NUM; i++)
	{
		if (!pItem[i]->isExecuteEnable())continue;
		if (HitCheck(player_global_x, player_global_y, item_global_x[i], item_global_y[i], HIT_DISTANCE_X, HIT_DISTANCE_Y))
		{
			pItem[i]->setRenderEnable(false);
			pItem[i]->setExecuteEnable(false);
			pl_hp += 2;
			pl_mode++;
			if (pl_hp > PL_HP_MAX)pl_hp = PL_HP_MAX;
		}
	}
	for (int i = 0; i < PL_HP_MAX / 2; i++)
	{
		if (pl_hp / 2 >= i + 1)
		{
			pHp[i]->setRenderEnable(true);
			pHp[i]->sizeX = 64.0f;
			pHp[i]->posX = HP_POS_X + HP_POS_X_DIS * i;
			pHp[i]->vtx[1].u = 0.5f;
			pHp[i]->vtx[3].u = 0.5f;
		}
		else
		{
			pHp[i]->setRenderEnable(false);
		}
	}
    

	//ハートを半分消す
	if (pl_hp % 2 != 0)
	{
		pHp[pl_hp / 2]->setRenderEnable(true);
		pHp[pl_hp / 2]->sizeX = 32.0f;
		pHp[pl_hp / 2]->posX = HP_POS_X + HP_POS_X_DIS * (pl_hp / 2 - 1) + 46;
		pHp[pl_hp / 2]->vtx[1].u = 0.45f;
		pHp[pl_hp / 2]->vtx[3].u = 0.45;
	}
	//クリア条件	
	bool enemy_clear = true;
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!pEnemy[i]->isExecuteEnable())continue;
		enemy_clear = false;
		break;
	}
	if (enemy_clear)switchScene(eSceneTable::GameClear);

	vnScene::execute();
}

//描画関数	
void SceneGame::render()
{
	vnScene::render();
}

//ゲーム関連の初期化
void GameInit()
{
	game_pause_f = false;

	bg_offset_x = 1280.0f;
	bg_offset_y = 0.0f;

	player_global_x = 0.0f;
	player_global_y = 650.0f;

	pl_jump_f = false;
	pl_jump_r = 0.0f;
	pl_add_y = 0;
	pl_vec = 1;
	pl_anim_pat = 0;
	pl_muteki_f - false;
	pl_muteki_cnt = 0;
	pl_hp = PL_HP_MAX;
	pl_weapon_rot = 0;
	pl_weapon_rot_add = 0;
	pl_attack = false;

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy_global_x[i] = enemy_init_pos[i][0];
		enemy_global_y[i] = enemy_init_pos[i][1];
		enemy_type[i] = enemy_init_pos[i][2];
	}
	for (int i = 0; i < ITEM_NUM; i++)
	{
		item_global_x[i] = item_init_pos[i][0];
		item_global_y[i] = item_init_pos[i][1];
		item_type[i] = item_init_pos[i][2];
	}
	
}

float AngToRad(float ang)
{
	return ang / 180.0f * PI;
}

bool HitCheck(float x1, float y1, float x2, float y2, float dis_x, float dis_y)
{
	bool tmp = false;

	if (fabsf(x1 - x2) <= dis_x && fabsf(y1 - y2) <= dis_y)
	{
		tmp = true;
	}
	return tmp;
}