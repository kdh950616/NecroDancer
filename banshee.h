#pragma once
#include "enemy.h"
class banshee :public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	banshee();
	~banshee();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();
	
	void standAni_LightChange();
	void attackAni_LightChange();
	
	void moveCal();
	
};

