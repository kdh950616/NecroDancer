#pragma once
#include "gameNode.h"
#include "mapEditor.h" // 나중에 로딩씬에 넣자.
#include "testScene.h"
#include "stageBoss.h"
#include "stage1.h"
#include "stage2.h"
#include "loadingScene.h"
#include "lobbyScene.h"
#include "test2Scene.h"


class playGround : public gameNode
{
private:

	mapEditor* _mapEditor;
	testScene* _testScene;
	stageBoss* _bossScene;
	stage1* _stage1;
	stage2* _stage2;
	loadingScene* _loadingScene;
	lobbyScene* _lobbyScene;
	test2Scene* _test2Scene;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void imageInit();
	void soundInit();

};