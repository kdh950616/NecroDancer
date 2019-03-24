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

void enemy::unfoundFunc()
{
	//비트타이밍마다
	if (_isBeat)
	{
		_isBeat = false;
		//본인이 속한 밝기맵이 밝혀졌다면 -> 플레이어의 횟불범위안에 들어온다면
		if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() <= 0.649f)
		{
			//발견됨으로 바꿈
			_isFind = true;
			_isLight = true;
			_beatCount = 0;
			_ani = KEYANIMANAGER->findAnimation("skeleton_Normal", "skeleton_Normal_Stand");
			_ani->start();
		}
	}
}

void enemy::lightFunc()
{
	//횟불범위 안에 있으면
	if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() <= 0.64f)
	{
		//밝음
		_isLight = true;
	}
	//횟불 범위 밖에있으면
	else if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() > 0.64f)
	{
		//어두움
		_isLight = false;
	}
}

void enemy::showAttackEffect()
{
	if (_direction.x == 0)
	{
		if (_direction.y == 1)
		{
			EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
		else if (_direction.y == -1)
		{
			EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
	}
	else if (_direction.y == 0)
	{
		if (_direction.x == 1)
		{
			EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
		else if (_direction.x == -1)
		{
			EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
	}
}
