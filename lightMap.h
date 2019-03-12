#pragma once
#include "gameNode.h"

class lightMap : public gameNode
{
private:

	POINTFLOAT _posLT;				//좌표
	POINT _idx;						//인덱스
	D2D_RECT_F _rc;					//어둠용 렉트
	ID2D1SolidColorBrush* _brush;	//렉트용 브러쉬
	bool _isFind;					//찾았는지
	float _opacity;					//본인의 알파값

public:
	lightMap();
	~lightMap();

	void setLightMap(int idX, int idY);

	//get
	POINTFLOAT getPos() { return _posLT; }
	POINT getIdx() { return _idx; }
	D2D_RECT_F getRc() { return _rc; }
	bool getIsFind() { return _isFind; }
	float getOpacity() { return _opacity; }
	ID2D1SolidColorBrush* getBrush() { return _brush; }

	//set
	void setPos(POINTFLOAT posLT) { _posLT = posLT; }
	void setIdx(POINT idx) { _idx = idx; }
	void setRc(D2D_RECT_F rc) { _rc = rc; }
	void setIsFind(bool isFind) { _isFind = isFind; }
	void setOpacity(float alpha) { _opacity = alpha; }
	void setBrush(ID2D1SolidColorBrush* brush) { _brush = brush; }
};

