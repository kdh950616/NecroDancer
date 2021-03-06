#pragma once
#include "gameNode.h"
#include "mapLoader.h"
#include "player.h"
#include "lightMap.h"
#include "rayCast.h"
#include "enemyMgr.h"


class stageBoss : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;
	typedef vector<lightMap*> vLightLine;
	typedef vector<vLightLine> vvLightMap;


	const float VOLUME = 0.3f;
	const float VOLUME_EFF = 1;
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

	float _itemPosY;			//아이템 둥둥 떠다니기 전용 변수
	bool _isNeedIncress;		//증가 -> 감소 -> 증가 하기위한 불변수

	int _tileBlinkX;			//코인배수가 있을때 이미지프레임 x에 더할 값 ( + 1 ~ + 2)  
								//플레이어한테 코인배수 받아와서 2이상이면 1 ~ 2사이로 움직이면서 적요하면됨 적용법은 이전에 맵에디터에 만들어뒀던거 참고
	int _torchFrameY;			//횃불프레임변화용 변수
	float _torchCompare;		//횃불프레임 변화에 쓰일 비교값(비트와 비트사이의 크기 저장해놈)

	int _trapCount;

	aStar* _aStar;
	list<POINT> _lShopVol;
	list<POINT>::iterator _lIShopVol;

	int _pawnBeat[8];
	int _pawnMakeCount;

	bool _isBattle;
public:
	stageBoss();
	~stageBoss();

	HRESULT init();
	void release();
	void update();
	void render();

	//===========================================
	//					init
	//===========================================

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
	void itemFloating();

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
	list<POINT>* getShopVol() { return &_lShopVol; }
};

