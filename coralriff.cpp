#include "stdafx.h"
#include "coralriff.h"
#include "player.h"


coralriff::coralriff()
{
}


coralriff::~coralriff()
{
}

HRESULT coralriff::init()
{
	imageInit();

	_maxHp = 3;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 3;
	_posZ = 0;
	_enemyType = DRUM;
	_isNeedAstar = false;
	_savePos = _posLT;
	_beatCount = 0;

	_moveCount = 999;
	_shakeVal = 0;

	_isFind = true;

	return S_OK;
}

void coralriff::release()
{
}

void coralriff::update()
{
	if (_isBeat)
	{
		_isBeat = false;
		_beatCount++;
		if (_beatCount >= 53)//여기추가
		{
			_isFind = true;
			_isNeedAstar = true;
			_beatCount = 0;
		}

		if (_isFind)
		{
			if (_beatCount == 1)
			{
				_shakeVal = 3;
			}
			else if (_beatCount == 2)
			{
				_beatCount = 0;
				_shakeVal = 0;
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

				for (int i = -1; i <= i; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if (_player->getIdx().x == _idx.x + _direction.x + j && _player->getIdx().y == _idx.y + _direction.y + i)
						{
							(*_player).setCurHp((*_player).getCurHp() - _dmg);

							showAttackEffect();
							//break;
						}
					}
				}
				 if (_direction.x == 0)
				{
					if (_direction.y == 1)
					{
						_isMove = true;
						_vec.y += SPEED;
					}
					else if (_direction.y == -1)
					{
						_isMove = true;
						_vec.y -= SPEED;
					}
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
				else if (_direction.y == 0)
				{
					if (_direction.x == 1)
					{
						_isMove = true;
						_vec.x += SPEED;
					}
					else if (_direction.x == -1)
					{
						_isMove = true;
						_vec.x -= SPEED;
					}
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
			}
		}
	}
	if (_shakeVal != 0)
	{
		_shakeCount++;
		if (_shakeCount >= 3)
		{
			_shakeCount = 0;
			_shakeVal == 3 ? _shakeVal = -3 : _shakeVal = 3;
		}
	}
	if (_isMove)
	{
		moveCal();
	}
}

void coralriff::render()
{
	_img->aniRender(_posLT.x - CAMERA->getPosX() + _shakeVal - MINUSX, _posLT.y - _posZ - CAMERA->getPosY() - MINUSY, _ani);
}

void coralriff::imageInit()
{
	IMAGEMANAGER->addFrameImage("coralriff", L"images/monster/boss/coralriff/coralriff_head.png", 344, 240, 4, 2);
	_img = IMAGEMANAGER->findImage("coralriff");

	KEYANIMANAGER->addAnimationType("coralriff");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("coralriff", "coralriff_Stand", "coralriff", stand, 4, ANIMATION_FPS, true);

	int shadow[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("coralriff", "coralriff_Shadow", "coralriff", shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("coralriff", "coralriff_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Tell", L"images/monster/boss/coralriff/tentacle_tell.png", 30, 20);

	_shadowImg = IMAGEMANAGER->findImage("shadow_Tell");

	_dustImg = nullptr;
	_dustAni = nullptr;
}

void coralriff::moveCal()
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
		_vec.x = 0;
		_savePos = _posLT;
		_isMove = false;
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

	}


	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	if (_isMove == false)
	{
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_direction = { 0,0 };
	}
	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
}
