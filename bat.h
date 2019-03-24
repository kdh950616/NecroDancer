#pragma once
#include "enemy.h"

class bat : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	bat();
	~bat();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void lightFunc();
	void moveCal();
};

