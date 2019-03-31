#pragma once
#include "enemy.h"

class rook : public enemy
{
private:
	const int ANIMATION_FPS = 5;
	bool _isCastling;
public:
	rook();
	~rook();

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
	bool getIsCastling() { return _isCastling; }
	void setIsCastling(bool isCastling) { _isCastling = isCastling; }
};

