#pragma once
#include "enemy.h"
class slime_Blue : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	slime_Blue();
	~slime_Blue();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();
	
	void attackAni_LightChange();
	void standAni_LightChange();

	void moveCal();
};

