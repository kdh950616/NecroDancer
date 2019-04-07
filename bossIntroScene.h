#pragma once
#include "gameNode.h"
class bossIntroScene : public gameNode
{
private:

	POINT _topPos;
	POINT _midPos;
	POINT _bottomPos;

	float _alpha;

public:
	bossIntroScene();
	~bossIntroScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void topMove(int num);
	bool midMove(int num);
	void bottomMove(int num);

	void showBoss();

};

