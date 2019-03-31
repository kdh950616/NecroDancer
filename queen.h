#pragma once
#include "enemy.h"
class queen : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	queen();
	~queen();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();
};

