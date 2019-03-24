#pragma once
#include "enemy.h"
class armadillo : public enemy
{
private:
	bool _isStun;
	const int ANIMATION_FPS = 8;
public:
	armadillo();
	~armadillo();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	bool getIsStun() { return _isStun; }
	void setIsStun(bool isStun) { _isStun = isStun; }

	void standAni_LightChange();
	void stunAni_LightChange();
	void frontAni_LightChange();
	void sideAni_LightChange();

	void moveCal();
	void specialMoveCal();

};

