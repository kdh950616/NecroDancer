#pragma once
#include "enemy.h"
class zombie : public enemy
{
private:
	const int ANIMATION_FPS = 4;
public:
	zombie();
	~zombie();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void change_Ani_LightChange();
	
	void moveCal();
	void specialMoveCal();
};

