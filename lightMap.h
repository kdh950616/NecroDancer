#pragma once
#include "gameNode.h"
class lightMap : public gameNode
{
private:
	POINT _pos;			//��ǥ
	POINT _idx;			//�ε���
	D2D_RECT_F _rc;		//��ҿ� ��Ʈ
	bool _isFind;		//ã�Ҵ���

public:
	lightMap();
	~lightMap();
};

