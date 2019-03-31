#pragma once
#include "gameNode.h"
#include "tile.h"
#include <list>

enum
{
	�Ӽ��̸����¼Ӽ��̸�
};


class aStar : public gameNode
{
private:
	//Ÿ�� ���θ� ���� ����
	vector<vector<tile*>>* _vTotalList;

	//�������
	vector<tile*> _vOpenList;

	//�������
	vector<tile*> _vCloseList;

public:
	aStar();
	~aStar();

	HRESULT init();
	void release();
	void update();
	void render();

	//�Ϲݸ�����
	void pathFinder(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	//�̳�Ÿ��ν�, �� �� �� ���Ƹ����� �ִ� �ֵ� ����
	void pathFinder2(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	void pathFinderBishop(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);

	void pathFinderShopkeeper(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	void pathFinder8(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay);
	void setMap(vector<vector<tile*>>* map) { _vTotalList = map; }
};

