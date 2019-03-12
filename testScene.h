#pragma once
#include "gameNode.h"
#include "mapLoader.h"
#include "player.h"
#include "lightMap.h"
#include "rayCast.h"

class testScene : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;
	typedef vector<lightMap*> vLightLine;
	typedef vector<vLightLine> vvLightMap;
private:
	mapLoader* _mapLoader;

	vvMap _vvMap;
	vvObj _vvObj;
	vvLightMap _vvLightMap;
	int _tileSizeX;
	int _tileSizeY;
	bool _isTileChanged;
	int _tileCount;

	player* _player;
	rayCast* _rayCast;

public:
	testScene();
	~testScene();

	HRESULT init();
	void release();
	void update();
	void render();

	//===========================================
	//					link
	//===========================================


	//===========================================
	//					init
	//===========================================
	
	void tileInit();

	//===========================================
	//					update
	//===========================================

	void tileUpdate();

	//===========================================
	//					render
	//===========================================

	void tileRender();

	//===========================================
	//					get
	//===========================================


	//===========================================
	//					set
	//===========================================

};

