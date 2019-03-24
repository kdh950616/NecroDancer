#pragma once
#include "enemy.h"
class clone : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	clone();
	~clone();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void lightFunc();

	void moveCal();
	void specialMoveCal();
};

