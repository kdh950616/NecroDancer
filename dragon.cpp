#include "stdafx.h"
#include "dragon.h"
#include "player.h"


dragon::dragon()
{
}


dragon::~dragon()
{
}

HRESULT dragon::init()
{
	imageInit();

	_maxHp = 4;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 4;
	_posZ = 0;
	_enemyType = DRAGON;
	_isNeedAstar = true;
	
	standAni_LightChange();

	return S_OK;
}

void dragon::release()
{
}

void dragon::update()
{
	if (!_isFind && _isBeat
		&& CAMERA->getPosX() - TILESIZE <= _posLT.x
		&& CAMERA->getPosY() - TILESIZE <= _posLT.y
		&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
		&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
	{
		_isFind = true;
		_isBeat = false;
		_beatCount = 0;
		SOUNDMANAGER->playEff("dragon_Cry");
	}
	else if (_isFind && _isBeat)
	{
		_beatCount++;
		_isBeat = false;
		_savePos = _posLT;
		lightFunc();

		standAni_LightChange();

		if (_beatCount >= 2)
		{
			_beatCount = 0;

			if (_listPath.size() > 0)
			{
				_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
			}
			else
			{
				POINT tmp = { RND->getInt(2),RND->getInt(2) };
				_listPath.push_back(tmp);
				_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
			}

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				(*_player).setCurHp((*_player).getCurHp() - _dmg);

				showAttackEffect();
				SOUNDMANAGER->playEff("dragon_Attack");
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == OBJ_WALL1 ||
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL2 ||
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL2 ||
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL_GOLD)
			{
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->objSetDefault();
			}
			else if (_direction.x == 0)
			{
				_dustAni->start();
				if (_direction.y == 1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isMove = true;
					_vec.y += SPEED;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
				}
				else if (_direction.y == -1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isMove = true;
					_vec.y -= SPEED;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
				}
			}
			else if (_direction.y == 0)
			{
				_dustAni->start();
				if (_direction.x == 1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isMove = true;
					_isLeft = false;
					_vec.x += SPEED;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
				}
				else if (_direction.x == -1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isMove = true;
					_isLeft = true;
					_vec.x -= SPEED;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
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
}

void dragon::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 22, _posLT.y - 30 - CAMERA->getPosY() + 40, 1.0f);
	if (_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX() - 24, _posLT.y - 30 - _posZ - CAMERA->getPosY() - 48, _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX() - 24, _posLT.y - 30 - _posZ - CAMERA->getPosY() - 48, _ani);
	}
}

void dragon::imageInit()
{
	IMAGEMANAGER->addFrameImage("dragon", L"images/monster/miniboss/dragon.png", 216, 204, 2, 2);
	_img = IMAGEMANAGER->findImage("dragon");

	EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	KEYANIMANAGER->addAnimationType("dragon");
	KEYANIMANAGER->addAnimationType("dragon_Shadow");

	int stand[] = { 0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("dragon", "dragon_Stand", "dragon", stand, 2, ANIMATION_FPS, true);

	int stand_Shadow[] = { 2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("dragon_Shadow", "dragon_Stand", "dragon", stand_Shadow, 2, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("dragon_Shadow", "dragon_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Large", L"images/monster/miniboss/large_shadow.png", 86, 28);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Large");

	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);
	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

void dragon::standAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("dragon", "dragon_Stand");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("dragon_Shadow", "dragon_Stand");
		_ani->start();
	}
}

void dragon::moveCal()
{
	int rnd = RND->getFromIntTo(1, 4);
	_posZ += _jumpPower;
	_jumpPower -= GRAVITY;

	if (_posLT.x - _savePos.x >= TILESIZE || _savePos.x - _posLT.x >= TILESIZE)
	{
		if (_vec.x > 0)
		{
			_posLT.x = _savePos.x + TILESIZE;
		}
		else if (_vec.x < 0)
		{
			_posLT.x = _savePos.x - TILESIZE;
		}
		_vec.x = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;

		switch (rnd)
		{
		case 1:
			SOUNDMANAGER->playEff("dragon_Walk1");
			break;
		case 2:
			SOUNDMANAGER->playEff("dragon_Walk2");
			break;
		case 3:
			SOUNDMANAGER->playEff("dragon_Walk3");
			break;
		}
	}

	if (_posLT.y - _savePos.y >= TILESIZE || _savePos.y - _posLT.y >= TILESIZE)
	{
		if (_vec.y > 0)
		{
			_posLT.y = _savePos.y + TILESIZE;
		}
		else if (_vec.y < 0)
		{
			_posLT.y = _savePos.y - TILESIZE;
		}
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;

		switch (rnd)
		{
		case 1:
			SOUNDMANAGER->playEff("dragon_Walk1");
			break;
		case 2:
			SOUNDMANAGER->playEff("dragon_Walk2");
			break;
		case 3:
			SOUNDMANAGER->playEff("dragon_Walk3");
			break;
		}
	}


	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
	}
	else
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
}

void dragon::specialMoveCal()
{
	_posZ += _jumpPower;
	_jumpPower -= GRAVITY;
	if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
	{
		if (_posLT.x - _savePos.x >= TILESIZE || _savePos.x - _posLT.x >= TILESIZE)
		{
			if (_vec.x > 0)
			{
				_posLT.x = _savePos.x + TILESIZE;
			}
			else if (_vec.x < 0)
			{
				_posLT.x = _savePos.x - TILESIZE;
			}
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			_vec.x = 0;
			_savePos = _posLT;
			_isMove = false;
			_jumpPower = 0;
			_posZ = 0;
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
		}
		else if (_posLT.y - _savePos.y >= TILESIZE || _savePos.y - _posLT.y >= TILESIZE)
		{
			if (_vec.y > 0)
			{
				_posLT.y = _savePos.y + TILESIZE;
			}
			else if (_vec.y < 0)
			{
				_posLT.y = _savePos.y - TILESIZE;
			}
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			_vec.y = 0;
			_savePos = _posLT;
			_isMove = false;
			_jumpPower = 0;
			_posZ = 0;
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
		}
	}
	else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
	{
		if (!_isBackward)
		{
			if (_posLT.x - _savePos.x >= TILESIZE / 2 || _savePos.x - _posLT.x >= TILESIZE / 2)
			{
				if (_vec.x > 0)
				{
					_posLT.x = _savePos.x + TILESIZE / 2;
				}
				else if (_vec.x < 0)
				{
					_posLT.x = _savePos.x - TILESIZE / 2;
				};
				_vec.x *= -1;
				_savePos = _posLT;
				_isBackward = true;
				_jumpPower = 0;
				_posZ = 0;
			}
			else if (_posLT.y - _savePos.y >= TILESIZE / 2 || _savePos.y - _posLT.y >= TILESIZE / 2)
			{
				if (_vec.y > 0)
				{
					_posLT.y = _savePos.y + TILESIZE / 2;
				}
				else if (_vec.y < 0)
				{
					_posLT.y = _savePos.y - TILESIZE / 2;
				}
				_vec.y *= -1;
				_savePos = _posLT;
				_isBackward = true;
				_jumpPower = 0;
				_posZ = 0;
			}
		}
		else if (_isBackward)
		{
			if (_posLT.x - _savePos.x >= TILESIZE / 2 || _savePos.x - _posLT.x >= TILESIZE / 2)
			{
				if (_vec.x > 0)
				{
					_posLT.x = _savePos.x + TILESIZE / 2;
				}
				else if (_vec.x < 0)
				{
					_posLT.x = _savePos.x - TILESIZE / 2;
				}
				_vec.x = 0;
				_savePos = _posLT;
				_isMove = false;
				_isBackward = false;
				_jumpPower = 0;
				_posZ = 0;
				_idx.x = (_posLT.x) / TILESIZE;
				_idx.y = (_posLT.y) / TILESIZE;
				_isSpecialAct = false;
				_direction = { 0,0 };
			}
			else if (_posLT.y - _savePos.y >= TILESIZE / 2 || _savePos.y - _posLT.y >= TILESIZE / 2)
			{
				if (_vec.y > 0)
				{
					_posLT.y = _savePos.y + TILESIZE / 2;
				}
				else if (_vec.y < 0)
				{
					_posLT.y = _savePos.y - TILESIZE / 2;
				}
				_vec.y = 0;
				_savePos = _posLT;
				_isMove = false;
				_isBackward = false;
				_jumpPower = 0;
				_posZ = 0;
				_idx.x = (_posLT.x) / TILESIZE;
				_idx.y = (_posLT.y) / TILESIZE;
				_isSpecialAct = false;
				_direction = { 0,0 };
			}
		}
	}
	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
}
