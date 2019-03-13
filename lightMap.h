#pragma once
#include "gameNode.h"

class lightMap : public gameNode
{
private:

	POINTFLOAT _posLT;				//��ǥ
	POINT _idx;						//�ε���
	D2D_RECT_F _tileRc;				//��Ʈ (��İ� ����Ұ���) 
	ID2D1SolidColorBrush* _brush;	//��Ʈ�� ���� �귯��
	bool _isFind;					//ã�Ҵ���
	float _opacity;					//������ ���İ�

public:
	lightMap();
	~lightMap();

	void setLightMap(int idX, int idY);


	//get
	POINTFLOAT getPos() { return _posLT; }
	POINT getIdx() { return _idx; }
	D2D_RECT_F getTileRc() { return _tileRc; }
	bool getIsFind() { return _isFind; }
	float getOpacity() { return _opacity; }
	ID2D1SolidColorBrush* getBrush() { return _brush; }

	//set
	void setPos(POINTFLOAT posLT) { _posLT = posLT; }
	void setIdx(POINT idx) { _idx = idx; }
	void setTileRc(D2D_RECT_F rc) { _tileRc = rc; }
	void setIsFind(bool isFind) { _isFind = isFind; }
	void setOpacity(float alpha) { _opacity = alpha; }
	void setBrush(ID2D1SolidColorBrush* brush) { _brush = brush; }
};

