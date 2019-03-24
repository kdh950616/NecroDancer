#pragma once
#include "enemy.h"
class minotaur : public enemy
{
private:
	bool _isStun;
	bool _isCharging;
	const int ANIMATION_FPS = 8;
public:
	minotaur();
	~minotaur();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	bool getIsStun() { return _isStun; }
	void setIsStun(bool isStun) { _isStun = isStun; }
	bool getIsCharging() { return _isCharging; }
	void setIsCharging(bool isCharging) { _isCharging = isCharging; }

	void standAni_LightChange();
	void stunAni_LightChange();
	void attackAni_LightChange();

	void moveCal();
	void specialMoveCal();
};