#pragma once
#include "gameNode.h"
class rayCast : public gameNode
{
private:
	typedef vector<POINT> vPoint;
	typedef vector<vPoint> vvPoint;
public:
	rayCast();
	~rayCast();

	//				플레이어좌표			뻗어갈 길이	, 몇도씩 돌릴건지
	void rayCasting(POINTFLOAT playerPos, int length, int angleNum);
};

