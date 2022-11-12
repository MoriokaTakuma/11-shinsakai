#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
bool SceneTitle::initialize()
{
	pTitle = new vnSprite((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 1280.0f, 720.0f, L"data/image/TITLE.png");
	//�쐬�����X�v���C�g�����N���X�ivnScene�N���X�j�ɓo�^
	registerObject(pTitle);
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