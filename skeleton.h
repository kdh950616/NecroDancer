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

	//=========================================================
	//						init
	//=========================================================
	void imageInit();


	//=========================================================
	//						update
	//=========================================================
	void unfoundFunc();
	void lightFunc();
	void attackAni_LightChange();
	void standAni_LightChange();
	void moveCal();

	//=========================================================
	//						render
	//=========================================================

};

