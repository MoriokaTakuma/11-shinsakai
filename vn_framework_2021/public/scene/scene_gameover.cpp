#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
bool SceneGameOver::initialize()
{
	 pGameOver= new vnSprite((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 1280.0f, 720.0f, L"data/image/GAMEOVER.png");
	//�쐬�����X�v���C�g�����N���X�ivnScene�N���X�j�ɓo�^
	registerObject(pGameOver);
	return true;
}
//�I���֐�
void SceneGameOver::terminate()
{
	//�X�v���C�g�̍폜
	deleteObject(pGameOver);
}
//�����֐�
void SceneGameOver::execute()
{
	//�L�[����
	if (vnKeyboard::trg(DIK_SPACE))
	{
		switchScene(eSceneTable::Title);
	}
	vnScene::execute();
}
//�`��֐�	
void SceneGameOver::render()
{
	vnScene::render();
}