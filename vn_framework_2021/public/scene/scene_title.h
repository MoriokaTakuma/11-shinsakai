#pragma once
class SceneTitle : public vnScene
{
private:
	vnSprite* pTitle;
	//BGM�ESE
	vnSound* pBgm_t;
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