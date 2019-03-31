#pragma once
#include "enemy.h"
class king : public enemy
{
private:
	const int ANIMATION_FPS = 8;
	bool _isCastling;
	POINT _destIdx; //몹이 다 살아있을때 가려하는 종착좌표
public:
	king();
	~king();

	HRESULT init();
	void release();
	void update();
	void render();

	void imageInit();

	void moveCal();

	bool getIsCastling() { return _isCastling; }
	void setIsCastling(bool isCastling) { _isCastling = isCastling; }
};

