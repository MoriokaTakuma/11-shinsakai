#pragma once
class SceneGameOver : public vnScene
{
private:
	vnSprite* pGameOver;
public:
	//������
	bool initialize();
	//�I��
	void terminate();
	//����
	void execute();
	//�`��
	void render();
};