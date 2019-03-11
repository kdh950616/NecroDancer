#pragma once
#include "gameNode.h"
#include "mapLoader.h"
#include "player.h"

class testScene : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;
private:
	mapLoader* _mapLoader;

	vvMap _vvMap;
	vvObj _vvObj;
	int _tileSizeX;
	int _tileSizeY;
	bool _isTileChanged;
	int _tileCount;

	player* _player;


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

