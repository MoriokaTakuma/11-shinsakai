//--------------------------------------------------------------//
//	"scene_joystick_test.cpp"									//
//		シーン：ジョイスティックテスト							//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

#define LINE_POS(y)		((float)(y)*16.0f+32.0f)
#define DISP_START		(100.0f)
#define DISP_INTERVAL	(300.0f)

//初期化
bool SceneJoystickTest::initialize()
{
	return true;
}

//終了
void SceneJoystickTest::terminate()
{
	vnFont::setColor(0xffffffff);
}

//処理関数
void SceneJoystickTest::execute()
{
	for(int i=0; i<vnJOYSTICK_MAX; i++)
	{
		float x = DISP_INTERVAL * (float)i + DISP_START;
		int y=0;
		DWORD col = 0xffffffff;
		if(vnJoystick::isValid(i)==true)
		{
			vnFont::print(x, LINE_POS(y), col, L"Joystick[%d] : enable.", i);
		}
		else
		{
			col = 0x80ffffff;
			vnFont::print(x, LINE_POS(y), col, L"Joystick[%d] : disable.", i);
		}
		y+=2;
		
		vnFont::print(x, LINE_POS(y), col, L"leftX : %.3f", vnJoystick::leftX(i));		y++;
		vnFont::print(x, LINE_POS(y), col, L"leftY : %.3f", vnJoystick::leftY(i));		y++;
		vnFont::print(x, LINE_POS(y), col, L"rightX : %.3f", vnJoystick::rightX(i));	y++;
		vnFont::print(x, LINE_POS(y), col, L"rightY : %.3f", vnJoystick::rightY(i));	y++;
		
		y++;
		
		vnFont::print(x, LINE_POS(y), col, L"pov : %d", vnJoystick::pov(i));	y++;
		
		y++;
		for(int j=0; j<vnDirectInput::getJoystickButtonNum(); j++)
		{
			vnFont::print(x, LINE_POS(y), col, L"Button[%d] : %d", j, vnJoystick::on(j, i));
			y++;
		}
	}

	vnScene::execute();
}

//描画関数
void SceneJoystickTest::render()
{
	vnScene::render();
}
