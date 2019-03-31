#pragma once
#include "enemy.h"
class king : public enemy
{
private:
	const int ANIMATION_FPS = 8;
	bool _isCastling;
	POINT _destIdx; //���� �� ��������� �����ϴ� ������ǥ
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

