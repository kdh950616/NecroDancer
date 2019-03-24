#pragma once
#include "enemy.h"
class bat_2 : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	bat_2();
	~bat_2();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void lightFunc();
	void moveCal();
};

