#pragma once
#include "gameNode.h"
#include "tile.h"
#pragma warning(disable: 4996)

class mapLoader : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;

public:
	mapLoader();
	~mapLoader();

	//Ÿ�ϸ� �ּ�, ������Ʈ �ּ�, x������ �ּ�, y������ �ּ�, �ҷ��� �ʹ�ȣ 0 ~ 9
	void mapLoad(vvMap* vvMapAdress, vvObj* vvObjAdress, int * tileSizeX, int* tileSizeY, bool* isTileChanged, int mapNum);

	void setNewMapTile(tile * tile, int idX, int idY, bool isTileChanged);
	void setNewObjTile(tile * tile, int idX, int idY);
};

