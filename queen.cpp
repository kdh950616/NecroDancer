#include "stdafx.h"
#include "queen.h"
#include "player.h"


queen::queen()
{
}


queen::~queen()
{
}

HRESULT queen::init()
{
	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 3;
	_posZ = 0;
	_enemyType = QUEEN;
	_isNeedAstar = true;
	_savePos = _posLT;
	_gold = 0;
	_isFind = true;

	return S_OK;
}

void queen::release()
{
}

void queen::update()
{
	if (_isFind && _isBeat)
	{
		_isBeat = false;
		_beatCount++;

		if (_beatCount >= 1)
		{
			_beatCount = 0;
			_savePos = _posLT;

			if (_listPath.size() > 0)
			{
				_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
			}
			else
			{

			}

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				attackPlayer(_dmg);

				int rnd = RND->getFromIntTo(1, 7);

				switch (rnd)
				{
				case 1:
					SOUNDMANAGER->playEff("queen_Attack1");
					break;
				case 2:
					SOUNDMANAGER->playEff("queen_Attack2");
					break;
				case 3:
					SOUNDMANAGER->playEff("queen_Attack3");
					break;
				case 4:
					SOUNDMANAGER->playEff("queen_Attack4");
					break;
				case 5:
					SOUNDMANAGER->playEff("queen_Attack5");
					break;
				case 6:
					SOUNDMANAGER->playEff("queen_Attack6");
					break;
				}
			}
			else
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				switch (_direction.x)
				{
				case -1:
					_vec.x -= _speed;
					break;
				case 0:
					_vec.x = 0;
					break;
				case 1:
					_vec.x += _speed;
					break;
				}

				switch (_direction.y)
				{
				case -1:
					_vec.y -= _speed;
					break;
				case 0:
					_vec.y = 0;
					break;
				case 1:
					_vec.y += _speed;
					break;
				}
			}
		}

		
	}
	if (_isMove)
	{
		moveCal();
	}

	if (_curHp <= 0)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}

	if (_isAttack)
	{
		countAttackEffect();
	}
}

void queen::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 3, _posLT.y - 15 - CAMERA->getPosY(), 1.0f);

	_img->frameRender(_posLT.x - CAMERA->getPosX(), _posLT.y - TILESIZE - _posZ - CAMERA->getPosY(), 0,0);

	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

void queen::imageInit()
{
	IMAGEMANAGER->addFrameImage("queen", L"images/monster/boss/queen.png", 66, 156, 1, 2);
	_img = IMAGEMANAGER->findImage("queen");

	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);
	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

void queen::moveCal()
{
	if (_posLT.x - _savePos.x >= TILESIZE || _savePos.x - _posLT.x >= TILESIZE ||
		_posLT.y - _savePos.y >= TILESIZE || _savePos.y - _posLT.y >= TILESIZE)
	{
		if (_vec.x > 0)
		{
			_posLT.x = _savePos.x + TILESIZE;
		}
		else if (_vec.x < 0)
		{
			_posLT.x = _savePos.x - TILESIZE;
		}
		if (_vec.y > 0)
		{
			_posLT.y = _savePos.y + TILESIZE;
		}
		else if (_vec.y < 0)
		{
			_posLT.y = _savePos.y - TILESIZE;
		}
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_vec.x = 0;
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;
	}

	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
}
