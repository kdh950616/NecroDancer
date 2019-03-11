#pragma once
#include "gameNode.h"
class lightMap : public gameNode
{
private:
	POINT _pos;			//좌표
	POINT _idx;			//인덱스
	D2D_RECT_F _rc;		//어둠용 렉트
	bool _isFind;		//찾았는지

public:
	lightMap();
	~lightMap();
};

