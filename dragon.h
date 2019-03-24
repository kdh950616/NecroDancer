#pragma once
#include "enemy.h"
class dragon : public enemy
{
private:
	const int ANIMATION_FPS = 2;
public:
	dragon();
	~dragon();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();
	void standAni_LightChange();

	void moveCal();
	void specialMoveCal();
};

