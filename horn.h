#pragma once
#include "enemy.h"
class horn :public enemy
{
private:
	const int ANIMATION_FPS = 8;
	const POINT RESPAWN_LEFT = { -1 , 1 };
	const POINT RESPAWN_RIGHT = { 1 , -1 };
	const int MINUSX_LEFT = 18;
	const int MINUSX_RIGHT = 5;
	const int MINUSY = 68;
public:
	horn();
	~horn();

		HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();

	void showAttackEffect();
};

