//--------------------------------------------------------------//
//	"scene_joystick_test.h"										//
//		シーン：ジョイスティックテスト							//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class SceneJoystickTest : public vnScene
{
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
