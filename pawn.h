#pragma once
#include "enemy.h"
class pawn : public enemy
{
private:
	const int ANIMATION_FPS = 4;
	
	bool _isFirstMove;

public:
	pawn();
	~pawn();

	HRESULT init();
	void release();
	void update();
	void render();

	//=========================================================
	//						init
	//=========================================================
	void imageInit();


	void moveCal();
	void firstMoveCal();
};

