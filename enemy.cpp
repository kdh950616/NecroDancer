#include "stdafx.h"
#include "enemy.h"
#include "player.h"


enemy::enemy()
{
}


enemy::~enemy()
{
}

HRESULT enemy::init()
{


	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
}

void enemy::render()
{
}

void enemy::makeEnemyIdx(POINT mapIdx)
{
	_idx = mapIdx;

	_posLT = { (float)(mapIdx.x * TILESIZE), (float)(mapIdx.y * TILESIZE)};

	_posCT = { _posLT.x + (TILESIZE / 2), _posLT.y + (TILESIZE / 2) };

	_rc = { (float)_posLT.x,
			(float)_posLT.y,
			(float)_posLT.x + TILESIZE,
			(float)_posLT.y + TILESIZE };
}
