#pragma once
#pragma once
class SceneGameClear : public vnScene
{
private:
	vnSprite* pGameClear;
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