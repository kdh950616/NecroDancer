#include "stdafx.h"
#include "lightMap.h"


lightMap::lightMap()
{
}


lightMap::~lightMap()
{
}

void lightMap::setLightMap(int idX, int idY)
{
	_idx = { idX,idY };
	_posLT = { (float)(TILESIZE * idX), (float)(TILESIZE * idY) };
	_brush = D2DMANAGER->createBrush(0x000000, 1.0f);
	_rc = { (float)_posLT.x,
			(float)_posLT.y,
			(float)_posLT.x + TILESIZE,
			(float)_posLT.y + TILESIZE };
	_isFind = false;
	_opacity = 1.0f;
}

