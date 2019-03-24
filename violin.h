#pragma once
#include "enemy.h"
class violin : public enemy
{
private:
	const int ANIMATION_FPS = 8;
	const POINT RESPAWN_LEFT = { 0 ,-1 };
	const POINT RESPAWN_RIGHT = { 0 , 1 };
	const int MINUSX_LEFT = 30;
	const int MINUSX_RIGHT = 10;
	const int MINUSY = 64;
public:
	violin();
	~violin();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();

	void showAttackEffect();
};

