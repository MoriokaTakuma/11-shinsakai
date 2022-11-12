//--------------------------------------------------------------//
//	"vn_sceneTable.h"											//
//		シーンテーブル											//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//シーンの種類
enum class eSceneTable
{
	Boot,
	ActionTest,
	BlockTest,
	SpriteTest,
	PriorityTest,
	JoystickTest,
	SeTest,
	FontTest,
	ActionGame,
	Title,
	GameClear,
	GameOver,
	SceneMax,
};

//シーンクラス定義ファイル
#include "vn_scene.h"
#include "scene_boot.h"
#include "scene_action_test.h"
#include "scene_block_test.h"
#include "scene_sprite_test.h"
#include "scene_priority_test.h"
#include "scene_joystick_test.h"
#include "scene_se_test.h"
#include "scene_font_test.h"
#include "SceneGame.h"
#include "scene_title.h"
#include "scene_gameclear.h"
#include "scene_gameover.h"


//シーン切り替え関数(予約)
void switchScene(eSceneTable scene);
//シーン切り替え関数(実処理)
void switchScene();
//シーン削除
void deleteScene();
