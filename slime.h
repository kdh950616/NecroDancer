#pragma once
#include "enemy.h"
class slime : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	slime();
	~slime();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();
	void lightFunc();
};

