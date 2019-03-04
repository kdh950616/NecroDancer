#pragma once
#include "gameNode.h"
#include "mapEditor.h" // 나중에 로딩씬에 넣자.

class playGround : public gameNode
{
private:

	mapEditor* _mapEditor;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};