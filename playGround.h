#pragma once
#include "gameNode.h"
#include "mapEditor.h" // ���߿� �ε����� ����.
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