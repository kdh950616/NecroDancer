#include "stdafx.h"
#include "armadillo.h"
#include "player.h"


armadillo::armadillo()
{
}


armadillo::~armadillo()
{
}

HRESULT armadillo::init()
{
	enemy::init();

	imageInit();

	_isStun = false;
	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 3;
	_posZ = 0;
	_enemyType = ARMADILLO;
	_isNeedAstar = false;
	_beatCount = 0;
	_gold = 2;

	

	return S_OK;
}

void armadillo::release()
{
}

void armadillo::update()
{
	if (!_isFind
	&&CAMERA->getPosX() - TILESIZE <= _posLT.x
	&& CAMERA->getPosY() - TILESIZE <= _posLT.y
	&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
	&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
	{
		_isFind = true;
	}
	if (_isBeat)
	{
		_isBeat = false;
		lightFunc();
		_savePos = _posLT;

		standAni_LightChange();
		if (_isStun)
		{
			stunAni_LightChange();
			_beatCount++;

			if (_beatCount == 3)
			{
				_beatCount = 0;
				_isStun = false;
				standAni_LightChange();
			}
		}
		else if ((*_player).getIdx().x == _idx.x && _direction.x == 0 && _direction.y == 0)
		{
			if ((*_player).getIdx().y > _idx.y)	//플레이어가 아래 -> 아래로돌진
			{
				_direction = { 0, 1 };
			}
			else if ((*_player).getIdx().y < _idx.y)// 플레이어가 위 -> 위로돌진
			{
				_direction = { 0, -1 };
			}
			SOUNDMANAGER->playEff("armadillo_Rush");
		}
		else if ((*_player).getIdx().y == _idx.y && _direction.x == 0 && _direction.y == 0)
		{	
			if ((*_player).getIdx().x > _idx.x)	//플레이어가 오른쪽 -> 오른쪽돌진
			{
				_direction = { 1,0 };
			}
			else if ((*_player).getIdx().x < _idx.x)// 플레이어가 왼쪽 -> 왼쪽돌진
			{
				_direction = { -1,0 };
			}
			SOUNDMANAGER->playEff("armadillo_Rush");
		}
		//다음에 갈곳이 플레이어면
		if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y && !_isStun)
		{
			attackPlayer(_dmg);

			if (abs(_direction.x) == 1)
			{
				sideAni_LightChange();
			}
			else if (abs(_direction.y) == 1)
			{
				frontAni_LightChange();
			}

			_isSpecialAct = true;
			_isStun = true;
			(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setAttribute(true);
			SOUNDMANAGER->playEff("armadillo_Attack");
		}
		//다음에 갈곳이 못가는곳이면
		else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false && (abs(_direction.x) == 1 || abs(_direction.y) == 1))
		{
			_isSpecialAct = true;			

			//그놈의 속성이 벽이었다면 벽없앰.
			if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == OBJ_WALL1 ||
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == OBJ_WALL2)
			{
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->objSetDefault();
				SOUNDMANAGER->playEff("armadillo_Wall");
			}

			if (_direction.x == 0)
			{
				frontAni_LightChange();
				_dustAni->start();
				if (_direction.y == 1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
					_isMove = true;
					_vec.y += SPEED;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
				}
				else if (_direction.y == -1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
					_isMove = true;
					_vec.y -= SPEED;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
				}
			}
			else if (_direction.y == 0)
			{
				sideAni_LightChange();
				_dustAni->start();
				if (_direction.x == 1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
					_isMove = true;
					_isLeft = false;
					_vec.x += SPEED;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
				}
				else if (_direction.x == -1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
					_isMove = true;
					_isLeft = true;
					_vec.x -= SPEED;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
				}
			}
		}
		if (_direction.x == 0 && _direction.y != 0)
		{
			frontAni_LightChange();
			_dustAni->start();
			if (_direction.y == 1)
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				if (!_isStun)
				{
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
				_isMove = true;
				_vec.y += SPEED;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
			else if (_direction.y == -1)
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				if (!_isStun)
				{
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
				_isMove = true;
				_vec.y -= SPEED;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
		}
		else if (_direction.y == 0 && _direction.x != 0)
		{
			sideAni_LightChange();
			_dustAni->start();
			if (_direction.x == 1)
			{
				_isLeft = false;
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				if (!_isStun)
				{
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
				_isMove = true;
				_vec.x += SPEED;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
			else if (_direction.x == -1)
			{
				_isLeft = true;
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				if (!_isStun)
				{
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
				_isMove = true;
				_vec.x -= SPEED;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
		}
	}
	if (_isMove)
	{
		if (!_isSpecialAct)
		{
			moveCal();
		}
		else if (_isSpecialAct)
		{
			specialMoveCal();
		}
	}

	if (_isAttack)
	{
		countAttackEffect();
	}
}

void armadillo::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

void armadillo::imageInit()
{
	IMAGEMANAGER->addFrameImage("armadillo", L"images/monster/normal/armadillo.png", 528, 96, 11, 2);
	_img = IMAGEMANAGER->findImage("armadillo");

	//EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	KEYANIMANAGER->addAnimationType("armadillo");
	KEYANIMANAGER->addAnimationType("armadillo_Shadow");

	int stand[] = { 0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo", "armadillo_Stand", "armadillo", stand, 2, 2, true);

	int stun[] = { 2 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo", "armadillo_Stun", "armadillo", stun, 1, 1, true);

	int front[] = { 3,4,5,6 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo", "armadillo_Front", "armadillo", front, 4, ANIMATION_FPS, true);

	int side[] = { 7,8,9,10 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo", "armadillo_Side", "armadillo", side, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 11,12 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo_Shadow", "armadillo_Stand", "armadillo", stand_Shadow, 2, 2, true);

	int stun_Shadow[] = { 13 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo_Shadow", "armadillo_Stun", "armadillo", stun_Shadow, 1, 1, true);

	int front_Shadow[] = { 14,15,16,17 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo_Shadow", "armadillo_Front", "armadillo", front_Shadow, 4, ANIMATION_FPS, true);

	int side_Shadow[] = { 18,19,20,21 };
	KEYANIMANAGER->addArrayFrameAnimation("armadillo_Shadow", "armadillo_Side", "armadillo", side_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("armadillo_Shadow", "armadillo_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);
	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

void armadillo::standAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo", "armadillo_Stand");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo_Shadow", "armadillo_Stand");
		_ani->start();
	}
}

void armadillo::stunAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo", "armadillo_Stun");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo_Shadow", "armadillo_Stun");
		_ani->start();
	}
}

void armadillo::frontAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo", "armadillo_Front");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo_Shadow", "armadillo_Front");
		_ani->start();
	}
}

void armadillo::sideAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo", "armadillo_Side");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("armadillo_Shadow", "armadillo_Side");
		_ani->start();
	}
}

void armadillo::moveCal()
{
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
		_jumpPower = 0;
		_posZ = 0;
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

void armadillo::specialMoveCal()
{
	_posZ += _jumpPower;
	_jumpPower -= GRAVITY;

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
			}
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
			stunAni_LightChange();
			_vec.x = 0;
			_savePos = _posLT;
			_isMove = false;
			_isBackward = false;
			_isSpecialAct = false;
			_isStun = true;
			_jumpPower = 0;
			_posZ = 0;
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			_direction = { 0,0 };
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
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
			stunAni_LightChange();
			_vec.y = 0;
			_savePos = _posLT;
			_isMove = false;
			_isBackward = false;
			_isSpecialAct = false;
			_isStun = true;
			_jumpPower = 0;
			_posZ = 0;
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			_direction = { 0,0 };
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		}
	}
	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
}
