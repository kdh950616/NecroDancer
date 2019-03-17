#pragma once
#include "enemy.h"
class skeleton : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	skeleton();
	~skeleton();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();
};

