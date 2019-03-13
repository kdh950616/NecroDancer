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

	vvLightMap* _vvLightMap;	//���� ����		�ּҸ� ����������
	vvObj* _vvObj;				//���� ������Ʈ���� �ּҸ� ����������

public:
	rayCast();
	~rayCast();

	//�ּ� ��ũ(��������)
	void setobjAdress(vvObj* vvobj) { _vvObj = vvobj; }
	void setLightMapAdress(vvLightMap* vvLightMap) { _vvLightMap = vvLightMap; }

	//				�÷��̾���ǥ		//ȶ�Ҽ��� (2 ~ 5) 2�� �⺻
	void rayCasting(POINT playeridx, int torchRange);
};

