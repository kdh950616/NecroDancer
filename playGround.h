#pragma once
#include "gameNode.h"
#include "mapEditor.h" // 나중에 로딩씬에 넣자.
#include "testScene.h"

class playGround : public gameNode
{
private:

	mapEditor* _mapEditor;
	testScene* _testScene;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};