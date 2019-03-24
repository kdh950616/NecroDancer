#include "stdafx.h"
#include "clone.h"
#include "player.h"


clone::clone()
{
}


clone::~clone()
{
}

HRESULT clone::init()
{
	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 3;
	_posZ = 0;
	_enemyType = CLONE;
	_isNeedAstar = false;

	return S_OK;
}

void clone::release()
{
}

void clone::update()
{
	//플레이어가 움직이는중 + 도착하지 않았으면
	if ((*_player).getIsMove() && !(*_player).getIsArrive())
	{
		//플레이어의 방향에 -를 곱한것을 클론이 가짐.
		_direction.x = ((*_player).getDirection().x) * -1;
		_direction.y = ((*_player).getDirection().y) * -1;
	}
	if (_isBeat)
	{
		_isBeat = false;
		if (_isFind)
		{
			//라이트 설정 + 애니(만화아님 ㅎ) 변경
			lightFunc();

			_savePos = _posLT;

			if ((*_player).getIdx().x + (*_player).getDirection().x == _idx.x + _direction.x && 
				(*_player).getIdx().y + (*_player).getDirection().y == _idx.y + _direction.y)
			{
				(*_player).setCurHp((*_player).getCurHp() - _dmg);

				showAttackEffect();

				//이거 안하면 비트마다 계속 공격함
				_direction = { 0,0 };
				SOUNDMANAGER->playEff("clone_Attack");
			}
			//클론이 갈곳이 못가는곳이고  클론이 계산된 direction을 가지고 있다면
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false && (abs(_direction.x) == 1 || abs(_direction.y) == 1)) 
			{
				_isSpecialAct = true;			//특수행동변수 트루

				if (_direction.x == 0)
				{
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
			//===================================================
			//						그냥 이동
			//===================================================
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
		else if (!_isFind
			&&CAMERA->getPosX() - TILESIZE <= _posLT.x
			&& CAMERA->getPosY() - TILESIZE <= _posLT.y
			&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
			&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
		{
			_isFind = true;
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

	if (_curHp <= 0)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
}

void clone::render()
{
	if (_isFind)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	
		_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - CAMERA->getPosY(), 1.0f);
		if (_isLeft)
		{
			_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
		}
		else if (!_isLeft)
		{
			_img->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
		}
	}
}

void clone::imageInit()
{
	IMAGEMANAGER->addFrameImage("clone", L"images/monster/normal/clone.png", 192, 96, 4, 2);
	_img = IMAGEMANAGER->findImage("clone");

	EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	KEYANIMANAGER->addAnimationType("clone");
	KEYANIMANAGER->addAnimationType("clone_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("clone", "clone_Stand", "clone", stand, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("clone_Shadow", "clone_Stand", "clone", stand_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("clone_Shadow", "clone_Stand");
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

void clone::lightFunc()
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

	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("clone", "clone_Stand");
		_ani->start();
	}
	else if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("clone_Shadow", "clone_Stand");
		_ani->start();
	}
}

void clone::moveCal()
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
		_direction = { 0,0 };
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
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;
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

void clone::specialMoveCal()
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
			_vec.x = 0;
			_savePos = _posLT;
			_isMove = false;
			_isBackward = false;
			_isSpecialAct = false;
			_jumpPower = 0;
			_posZ = 0;
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
			_isSpecialAct = false;
			_jumpPower = 0;
			_posZ = 0;
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
			_direction = { 0,0 };
		}
	}
	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
}
