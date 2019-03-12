#pragma once
#include "gameNode.h"
#include "lightMap.h"
#include "tile.h"
class rayCast : public gameNode
{
private:
	typedef vector<lightMap*> vLine;
	typedef vector<vLine> vvLightMap;
	typedef vector<tile*> vLineObj;
	typedef vector<vLineObj> vvObj;

	const float OPACITY_FIND = 0.6f;
private:

	vvLightMap* _vvLightMap;
	vvObj* _vvObj;

public:
	rayCast();
	~rayCast();

	void setobjAdress(vvObj* vvobj) { _vvObj = vvobj; }
	void setLightMapAdress(vvLightMap* vvLightMap) { _vvLightMap = vvLightMap; }

	//				플레이어좌표			
	void rayCasting(POINT playeridx, int torchRange);
};

