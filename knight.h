#pragma once
#include "enemy.h"
class knight :
	public enemy
{
private:
	const int ANIMATION_FPS = 8;
	const int MINUS = 0;

	bool _reverseImg;
public:
	knight();
	~knight();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();
	void countAttackEffect();
	void showAttackEffect();
};

