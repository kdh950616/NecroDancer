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

	//				�÷��̾���ǥ			��� ����	, ��� ��������
	void rayCasting(POINTFLOAT playerPos, int length, int angleNum);
};

