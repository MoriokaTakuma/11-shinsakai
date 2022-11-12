//--------------------------------------------------------------//
//	"scene_se_test.cpp"											//
//		ÉVÅ[ÉìÅFSEÉeÉXÉg										//
//													2021/06/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

#define FILE_PATH_MAX	(256)

WCHAR seFile[][FILE_PATH_MAX] = 
{
	L"data/sound/decision1.wav",
	L"data/sound/decision2.wav",
	L"data/sound/decision3.wav",
	L"data/sound/decision7.wav",
	L"data/sound/decision17.wav",
	L"data/sound/decision21.wav",
	L"data/sound/decision22.wav",
	L"data/sound/decision23.wav",
	L"data/sound/cursor1.wav",
	L"data/sound/cursor2.wav",
	L"data/sound/cursor3.wav",
	L"data/sound/cursor4.wav",
	L"data/sound/cursor10.wav",
	L"data/sound/cancel1.wav",
};

//èâä˙âª
bool SceneSeTest::initialize()
{
	fileNum = sizeof(seFile)/(sizeof(WCHAR)*FILE_PATH_MAX);
	
	pSound = new vnSound*[fileNum];
	for(int i=0; i<fileNum; i++)
	{
		pSound[i] = new vnSound(seFile[i]);
	}
	return true;
}

//èIóπ
void SceneSeTest::terminate()
{
	if (pSound != NULL)
	{
		for (int i = 0; i < fileNum; i++)
		{
			if (pSound[i] == NULL)continue;
			delete pSound[i];
			pSound[i] = NULL;
		}
		delete[] pSound;
		pSound = NULL;
	}
	vnFont::setColor(0xffffffff);
}

//èàóùä÷êî
void SceneSeTest::execute()
{
	if(vnMouse::trg())
	{
		int i = (vnMouse::getY() - 100) / 32;
		if(i>=0 && i<fileNum)
		{
			if(vnMouse::getX()>400 && vnMouse::getX()<450)
			{
				if(pSound[i]->isPlaying()==false)
				{
					pSound[i]->play();
				}
				else
				{
					pSound[i]->stop();
				}
			}
			else if(vnMouse::getX()>560 && vnMouse::getX()<580)
			{
				pSound[i]->setVolume(pSound[i]->getVolume()-1000);
			}
			else if(vnMouse::getX()>700 && vnMouse::getX()<720)
			{
				pSound[i]->setVolume(pSound[i]->getVolume()+1000);
			}
			else if(vnMouse::getX()>860 && vnMouse::getX()<880)
			{
				pSound[i]->setPan(pSound[i]->getPan()-1000);
			}
			else if(vnMouse::getX()>1000 && vnMouse::getX()<1020)
			{
				pSound[i]->setPan(pSound[i]->getPan()+1000);
			}
		}
	}
	
	vnFont::print(100.0f, 68.0f, L"File");
	vnFont::print(400.0f, 68.0f, L"Control");
	vnFont::print(600.0f, 68.0f, L"Volume");
	vnFont::print(900.0f, 68.0f, L"Pan");
	
	for (int i = 0; i<fileNum; i++)
	{
		float y=100.0f+32.0f*(float)i;
		
		vnFont::setColor(pSound[i]->isPlaying() ? 0xffffff00 : 0xffffffff);
		
		vnFont::print(100.0f, y, L"%s", seFile[i]);
		
		vnFont::print(400.0f, y, pSound[i]->isPlaying() ? L"Stop" : L"Play");
		
		vnFont::print(560.0f, y, L"Å|");
		vnFont::print(600.0f, y, L"%d", pSound[i]->getVolume());
		vnFont::print(700.0f, y, L"Å{");
		
		vnFont::print(860.0f, y, L"Å|");
		vnFont::print(900.0f, y, L"%d", pSound[i]->getPan());
		vnFont::print(1000.0f, y, L"Å{");
	}
	vnScene::execute();
}

//ï`âÊä÷êî
void SceneSeTest::render()
{
	vnScene::render();
}

