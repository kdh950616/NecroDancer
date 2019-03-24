#pragma once
#include "gameNode.h"
#include "mapEditor.h" // ���߿� �ε����� ����.
#include "testScene.h"
#include "boss_Coralriff.h"

class playGround : public gameNode
{
private:

	mapEditor* _mapEditor;
	testScene* _testScene;
	boss_Coralriff* _bossCoral;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};