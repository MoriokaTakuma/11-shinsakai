#pragma once
class SceneTitle : public vnScene
{
private:
	vnSprite* pTitle;

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