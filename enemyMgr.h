#pragma once
#include "gameNode.h"
#include "tile.h"
#include "lightMap.h"
#include "aStar.h"
#include "skeleton.h"
#include "skeleton_2.h"
#include "skeleton_3.h"
#include "bat.h"
#include "bat_2.h"
#include "slime.h"
#include "slime_Blue.h"
#include "zombie.h"
#include "wraith.h"
#include "clone.h"
#include "armadillo.h"
#include "minotaur.h"
#include "dragon.h"
#include "bat_Boss.h"
#include "banshee.h"
#include "drum.h"
#include "horn.h"
#include "keybord.h"
#include "violin.h"
#include "coralriff.h"

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

	int* _time;
	string* _stageKeyName;

private:
	const int ANIMATION_FPS = 5;
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
	void linkTime(int* time) { _time = time; }
	void linkStageKeyName(string* stageKeyName) { _stageKeyName = stageKeyName; }

	void setIsBeat(bool isBeat) { _isBeat = isBeat; }

	//=========================================================
	//						겟셋기
	//=========================================================
	vector<enemy*> getVEnemy() { return _vEnemy; }
	string* getStageKeyName() { return _stageKeyName; }

};

