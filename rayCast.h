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

	const float OPACITY_FIND = 0.65f;
	const float WALL_COST = 10;
private:

	vvLightMap* _vvLightMap;	//밝기용 맵의		주소를 가지고있음
	vvObj* _vvObj;				//밝기용 오브젝트맵을 주소를 가지고있음

public:
	rayCast();
	~rayCast();

	//주소 링크(스테이지)
	void setobjAdress(vvObj* vvobj) { _vvObj = vvobj; }
	void setLightMapAdress(vvLightMap* vvLightMap) { _vvLightMap = vvLightMap; }

	//				플레이어좌표		//횃불세기 (2 ~ 5) 2가 기본
	void rayCasting(POINT playeridx, int torchRange);
};

