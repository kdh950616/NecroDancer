#pragma once
#include "enemy.h"
class coralriff : public enemy
{
private:
	const int ANIMATION_FPS = 8;
	const int MINUSX = 10;
	const int MINUSY = 10;
public:
	coralriff();
	~coralriff();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();
};

