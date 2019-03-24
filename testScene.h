#pragma once
#include "gameNode.h"
#include "mapLoader.h"
#include "player.h"
#include "lightMap.h"
#include "rayCast.h"
#include "enemyMgr.h"

class testScene : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;
	typedef vector<lightMap*> vLightLine;
	typedef vector<vLightLine> vvLightMap;


	const float VOLUME = 0.3f;
	const float VOLUME_EFF = 1.0f;
private:
	mapLoader*					_mapLoader;

	vvMap						_vvMap;
	vvObj						_vvObj;
	vvLightMap					_vvLightMap;
	int							_tileSizeX;
	int							_tileSizeY;
	bool						_isBeat;

	player*						_player;

	rayCast*					_rayCast;

	enemyMgr*					_em;
		
	//사운드용
	string						_stageKeyName;
	const char*					_beatFileName;
	float						_pitch;
	int							_time;
	vector<tagBeat>				_vBeat;			//비트 담을곳
	image*						_heartImg;
	D2D1_RECT_F					_rc_Correct;
	D2D1_RECT_F					_rc_Wrong;
	bool						_isBeatCorrect;


	bool _showText;
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
	
	void imageInit();
	void mapLoaderInit();
	void lightMapInit();
	void playerInit();
	void rayCastInit();
	void beatInit();

	//===========================================
	//					update
	//===========================================

	void tileUpdate();
	void playerUpdate();
	void beatUpdate();

	//===========================================
	//					render
	//===========================================

	void tileRender();
	void objRender();
	void lightMapRender();
	void beatRender();
	void textRender();
	//===========================================
	//					get
	//===========================================


	//===========================================
	//					set
	//===========================================

};

