#pragma once
#include "gameNode.h"
#include "tile.h"
#include "skeleton.h"
#include "lightMap.h"
#include "aStar.h"

class player;

class enemyMgr : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;
	typedef vector<lightMap*> vLightLine;
	typedef vector<vLightLine> vvLightMap;
private:
	//=====================================================
	//					주소 받을 변수들
	//=====================================================
	player* _player; //플레이어 주소 받을곳
	vvMap* _vvMap;
	vvObj* _vvObj;
	vvLightMap* _vvLightMap;

	vector<enemy*> _vEnemy;

	aStar* _aStar;

	bool _isBeat;

public:
	enemyMgr();
	~enemyMgr();

	HRESULT init();
	void release();
	void update();
	void render();

	void createEnemy(int enemyType, POINT idx);

	void linkPlayer(player* player) { _player = player; }
	void linkMap(vvMap* vvMap) { _vvMap = vvMap; }
	void linkObj(vvObj* vvObj) { _vvObj = vvObj; }
	void linkLightMap(vvLightMap* vvLightMap) { _vvLightMap = vvLightMap; }

	void setIsBeat(bool isBeat) { _isBeat = isBeat; }

};

