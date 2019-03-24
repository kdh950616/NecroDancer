#pragma once
#include "enemy.h"
class wraith : public enemy
{
private:
	const int ANIMATION_FPS = 6;
public:
	wraith();
	~wraith();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void standAni_LightChange();
	void attackAni_LightChange();

	void moveCal();
};

