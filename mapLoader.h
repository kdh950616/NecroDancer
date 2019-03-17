#pragma once
#include "gameNode.h"
#include "tile.h"
#include "enemyMgr.h"
#pragma warning(disable: 4996)

class mapLoader : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;
	string _stageKeyName;
	const char * _beatFileName; 
	float _volume; 
	vector<tagBeat>* _vBeat;

	enemyMgr* _em;

public:
	mapLoader();
	~mapLoader();

	HRESULT beatInit(string stageKeyName, const char * beatFileName, float volume);
	HRESULT beatInit(string stageKeyName, const char * beatFileName, float volume, vector<tagBeat>* vBeat);

	
	//타일맵 주소, 오브젝트 주소, x사이즈 주소, y사이즈 주소, 불러올 맵번호 0 ~ 9
	void mapLoad(vvMap* vvMapAdress, vvObj* vvObjAdress, int * tileSizeX, int* tileSizeY, bool* isTileChanged, int mapNum);
	
	void linkenemyMgr(enemyMgr* em) { _em = em; }
	void makeEnemy(vvObj* vvObj, int* tileSizeX, int* tileSizeY);

	void setNewMapTile(tile * tile, int idX, int idY, bool isTileChanged);
	void setNewObjTile(tile * tile, int idX, int idY);
};

