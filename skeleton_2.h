#pragma once
#include "enemy.h"
class skeleton_2 : public enemy
{
private:
	const int ANIMATION_FPS = 8;
public:
	skeleton_2();
	~skeleton_2();

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
	void headlessAni_LightChange();
	void moveCal();
	void specialMoveCal();
	//=========================================================
	//						render
	//=========================================================
};

