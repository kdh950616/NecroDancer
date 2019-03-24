#pragma once
#include "enemy.h"
class drum : public enemy
{
private:
	const int ANIMATION_FPS = 8;
	const POINT RESPAWN_LEFT = { -1 ,-1 };
	const POINT RESPAWN_RIGHT = { 1 , 1 };
	const int MINUSX_LEFT = 15;
	const int MINUSX_RIGHT = 20;
	const int MINUSY = 62;
public:
	drum();
	~drum();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();

	void showAttackEffect();
};

