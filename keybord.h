#pragma once
#include "enemy.h"
class keybord : public enemy
{
private:
	const int ANIMATION_FPS = 8;
	const POINT RESPAWN_LEFT = { -1 ,0 };
	const POINT RESPAWN_RIGHT = { 1 , 0 };
	const int MINUSX_LEFT = 10;
	const int MINUSX_RIGHT = -5;
	const int MINUSY = 62;
public:
	keybord();
	~keybord();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();

	void showAttackEffect();
};

