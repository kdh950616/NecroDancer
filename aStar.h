#pragma once
#include "gameNode.h"
#include "tile.h"
#include <list>

enum
{
	속성이못가는속성이면
};


class aStar : public gameNode
{
private:
	//타일 전부를 넣을 벡터
	vector<vector<tile*>>* _vTotalList;

	//열린목록
	vector<tile*> _vOpenList;

	//닫힌목록
	vector<tile*> _vCloseList;

public:
	aStar();
	~aStar();

	HRESULT init();
	void release();
	void update();
	void render();

	//일반몹전용
	void pathFinder(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	//미노타우로스, 용 등 벽 갉아먹을수 있는 애들 전용
	void pathFinder2(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	void pathFinderBishop(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);

	void pathFinderShopkeeper(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	void pathFinder8(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	void setMap(vector<vector<tile*>>* map) { _vTotalList = map; }
};

