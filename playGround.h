#pragma once
#include "gameNode.h"
#include "mapEditor.h" // 나중에 로딩씬에 넣자.
#include "testScene.h"
#include "stageBoss.h"

class playGround : public gameNode
{
private:

	mapEditor* _mapEditor;
	testScene* _testScene;
	stageBoss* _bossScene;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};