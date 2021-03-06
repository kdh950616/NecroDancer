#include "stdafx.h"
#include "bat_Boss.h"
#include "player.h"


bat_Boss::bat_Boss()
{
}


bat_Boss::~bat_Boss()
{
}

HRESULT bat_Boss::init()
{
	enemy::init();

	imageInit();

	_maxHp = 3;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 3;
	_enemyType = BAT_BOSS;
	_beatCount = 0;
	_posZ = 30;
	_isNeedAstar = false;
	_gold = 10;

	_showPos = { 0,30 };

	return S_OK;
}

void bat_Boss::release()
{
}

void bat_Boss::update()
{

	if	  ((CAMERA->getPosX() - TILESIZE) / 2 <= _posLT.x
		&& (CAMERA->getPosY() - TILESIZE) / 2 <= _posLT.y
		&& (CAMERA->getPosX() + TILESIZE) / 2 + WINSIZEX >= _posLT.x
		&& (CAMERA->getPosY() + TILESIZE) / 2 + WINSIZEY >= _posLT.y)
	{
		if (_isBeat)
		{
			_beatCount++;
			_isBeat = false;
			_savePos = _posLT;
			lightFunc();


			if (_beatCount == 2)
			{
				_beatCount = 0;
				_savePos = _posLT;
				while (1)
				{
					//랜덤으로 방향 구하고
					_direction = { RND->getFromIntTo(-1,2),RND->getFromIntTo(-1,2) };
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true && (abs)(_direction.x) != (abs)(_direction.y))
					{
						if (_direction.x == -1)
						{
							_isLeft = true;
						}
						else if (_direction.x == 1)
						{
							_isLeft = false;
						}
						break;
					}
				}

				if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
				{
					attackPlayer(_dmg);

					SOUNDMANAGER->playEff("bat_Boss_Attack");
				}
				else if (_direction.x == 0)
				{
					if (_direction.y == 1)
					{
						(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
						_isMove = true;
						_vec.y += SPEED;
					}
					else if (_direction.y == -1)
					{
						(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
						_isMove = true;
						_vec.y -= SPEED;
					}
				}
				else if (_direction.y == 0)
				{
					if (_direction.x == 1)
					{
						(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
						_isMove = true;
						_isLeft = false;
						_vec.x += SPEED;
					}
					else if (_direction.x == -1)
					{
						(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
						_isMove = true;
						_isLeft = true;
						_vec.x -= SPEED;
					}
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

void bat_Boss::render()
{
	enemy::render();

	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 5, _posLT.y + 15 - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX() - 5, _posLT.y - 10 - _posZ - CAMERA->getPosY(), _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX() - 5, _posLT.y - 10 - _posZ - CAMERA->getPosY(), _ani);
	}
	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

void bat_Boss::imageInit()
{
	_img = IMAGEMANAGER->findImage("bat_Boss");

	KEYANIMANAGER->addAnimationType("bat_Boss");
	KEYANIMANAGER->addAnimationType("bat_Boss_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("bat_Boss", "bat_Boss_Stand", "bat_Boss", stand, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("bat_Boss_Shadow", "bat_Boss_Stand", "bat_Boss", stand_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("bat_Boss_Shadow", "bat_Boss_Stand");
	_ani->start();

	_shadowImg = IMAGEMANAGER->findImage("shadow_Large");

	_dustImg = nullptr;
	_dustAni = nullptr;
}

void bat_Boss::lightFunc()
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
		_ani = KEYANIMANAGER->findAnimation("bat_Boss", "bat_Boss_Stand");
		_ani->start();
	}
	else if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("bat_Boss_Shadow", "bat_Boss_Stand");
		_ani->start();
	}
}

void bat_Boss::moveCal()
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
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_vec.x = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
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
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
	}

	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	//if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE ||
	//	(*_vvObj)[_idx.y][_idx.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= ETC_TORCH_BOSS)
	//{
	//	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
	//}
	//else
	//{
	//	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	//}
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	//(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
}
