#pragma once
#include "gameNode.h"
#include "lightMap.h"
class rayCast : public gameNode
{
private:
	typedef vector<lightMap> vLine;
	typedef vector<vLine> vvLightMap;
private:
	vvLightMap _vvLightMap;
public:
	rayCast();
	~rayCast();

	//				플레이어좌표			
	void rayCasting(POINTFLOAT playerPos, int torchLevel);
};

