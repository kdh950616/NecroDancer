#pragma once
#include "enemy.h"
class bishop : public enemy
{
private:
	const int ANIMATION_FPS = 4;
public:
	bishop();
	~bishop();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();
};

