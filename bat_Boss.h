#pragma once
#include "enemy.h"
class bat_Boss : public enemy
{
private: 
	const int ANIMATION_FPS = 8;
public:
	bat_Boss();
	~bat_Boss();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void lightFunc();
	void moveCal();
};

