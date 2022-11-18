#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
bool SceneTitle::initialize()
{
	pTitle = new vnSprite((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 1280.0f, 720.0f, L"data/image/TITLE.png");
	
	//�쐬�����X�v���C�g�����N���X�ivnScene�N���X�j�ɓo�^
	registerObject(pTitle);

	//BGM.SE
	pBgm_t = new vnSound(L"data/sound/121_Machi.wav");

	return true;
}
//�I���֐�
void SceneTitle::terminate()
{
	//�X�v���C�g�̍폜
	deleteObject(pTitle);
}
//�����֐�
void SceneTitle::execute()
{
	if (pBgm_t->isStopped()) { pBgm_t->play(); }
	//�L�[����
	if (vnKeyboard::trg(DIK_SPACE))
	{
		switchScene(eSceneTable::ActionGame);
	}
	vnScene::execute();
}
//�`��֐�	
void SceneTitle::render()
{
	vnScene::render();
}