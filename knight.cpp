#include "stdafx.h"
#include "knight.h"
#include "player.h"


knight::knight()
{
}


knight::~knight()
{
}

HRESULT knight::init()
{
	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED * 1.5f;
	_dmg = 2;
	_posZ = 0;
	_enemyType = KNIGHT;
	_isNeedAstar = false;
	_savePos = _posLT;
	_gold = 0;
	_isFind = false;

	return S_OK;
}

void knight::release()
{
}

void knight::update()
{
	if (_isFind && _isBeat)
	{
		_beatCount++;
		_isBeat = false;

		if (_beatCount == 3)
		{
			_ani = KEYANIMANAGER->findAnimation("knight", "knight_Attack");
			_ani->start();
		}
		else if (_beatCount >= 4)
		{
			_beatCount = 0;
			_ani = KEYANIMANAGER->findAnimation("knight", "knight_Stand");
			POINT tmp = { (*_player).getIdx().x - _idx.x, (*_player).getIdx().y - _idx.y };

			_savePos = _posLT;

			if (abs(tmp.x) >= abs(tmp.y))
			{
				if (tmp.x >= 0)
				{
					_direction.x = 2;
				}
				else if (tmp.x < 0)
				{
					_direction.x = -2;
				}

				if (tmp.y >= 0)
				{
					_direction.y = 1;
				}
				else if (tmp.y < 0)
				{
					_direction.y = -1;
				}
			}
			else if (abs(tmp.y) > abs(tmp.x))
			{
				if (tmp.y >= 0)
				{
					_direction.y = 2;
				}
				else if (tmp.y < 0)
				{
					_direction.y = -2;
				}

				if (tmp.x >= 0)
				{
					_direction.x = 1;
				}
				else if (tmp.x < 0)
				{
					_direction.x = -1;
				}
			}

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				attackPlayer(_dmg);

				SOUNDMANAGER->playEff("piece_Attack");
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{

			}
			else if(_direction.x == 2)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.x = _speed;
				_posZ = 0;

				if (_direction.y == 1)
				{
					_vec.y = _speed / 2;
				}
				else if (_direction.y == -1)
				{
					_vec.y -= _speed / 2;
				}
			}
			else if (_direction.x == -2)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.x -= _speed;
				_posZ = 0;

				if (_direction.y == 1)
				{
					_vec.y = _speed / 2;
				}
				else if (_direction.y == -1)
				{
					_vec.y -= _speed / 2;
				}
			}
			else if (_direction.y == 2)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.y = _speed;
				_posZ = 0;

				if (_direction.x == 1)
				{
					_vec.x = _speed / 2;
				}
				else if (_direction.x == -1)
				{
					_vec.x -= _speed / 2;
				}
			}
			else if (_direction.y == -2)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.y -= _speed;
				_posZ = 0;

				if (_direction.x == 1)
				{
					_vec.x = _speed / 2;
				}
				else if (_direction.x == -1)
				{
					_vec.x -= _speed / 2;
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

void knight::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 3, _posLT.y - 15 - CAMERA->getPosY(), 1.0f);

	_img->aniRender(_posLT.x - CAMERA->getPosX() + 10, _posLT.y - 20  - CAMERA->getPosY(), _ani);

	if (_isAttack && _reverseImg)
	{
		IMAGEMANAGER->findImage("knithg_Attack_R")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), 0, 0, _atkAngle);
	}
	else if (_isAttack && !_reverseImg)
	{
		IMAGEMANAGER->findImage("knithg_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), 0, 0, _atkAngle);
	}
}

void knight::imageInit()
{
	_img = IMAGEMANAGER->findImage("knight");

	KEYANIMANAGER->addAnimationType("knight");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("knight", "knight_Stand", "knight", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4 };
	KEYANIMANAGER->addArrayFrameAnimation("knight", "knight_Attack", "knight", attack, 1, 1, true);

	_ani = KEYANIMANAGER->findAnimation("knight", "knight_Stand");
	_ani->start();

	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

void knight::moveCal()
{
	_posZ += _jumpPower;
	_jumpPower -= GRAVITY;

	if (_posLT.x - _savePos.x >= TILESIZE - MINUS || _savePos.x - _posLT.x >= TILESIZE - MINUS &&
		_posLT.y - _savePos.y >= TILESIZE - MINUS || _savePos.y - _posLT.y >= TILESIZE - MINUS)
	{
		if (_direction.x == 2)
		{
			_posLT.x = _savePos.x + TILESIZE * 2;
			
			if (_direction.y == 1)
			{
				_posLT.y = _savePos.y + TILESIZE;
			}
			else if (_direction.y == -1)
			{
				_posLT.y = _savePos.y - TILESIZE;
			}
		}
		else if (_direction.x == -2)
		{
			_posLT.x = _savePos.x - TILESIZE * 2;

			if (_direction.y == 1)
			{
				_posLT.y = _savePos.y + TILESIZE;
			}
			else if (_direction.y == -1)
			{
				_posLT.y = _savePos.y - TILESIZE;
			}
		}
		else if (_direction.y == 2)
		{
			_posLT.y = _savePos.y + TILESIZE * 2;

			if (_direction.x == 1)
			{
				_posLT.x = _savePos.x + TILESIZE;
			}
			else if (_direction.x == -1)
			{
				_posLT.x = _savePos.x - TILESIZE;
			}
		}
		else if (_direction.y == -2)
		{
			_posLT.y = _savePos.y - TILESIZE * 2;

			if (_direction.x == 1)
			{
				_posLT.x = _savePos.x + TILESIZE;
			}
			else if (_direction.x == -1)
			{
				_posLT.x = _savePos.x - TILESIZE;
			}
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

void knight::countAttackEffect()
{
	_atkCount++;

	if (_atkCount >= 10)
	{
		_atkCount = 0;
		_isAttack = false;
		_atkDirection = { 0,0 };
	}
}

void knight::showAttackEffect()
{
	if (_direction.y == 2)
	{
		if (_direction.x == 1)
		{
			_atkDirection = { 0,0 };
			_atkAngle = 90;
			_reverseImg = false;
		}
		else if (_direction.x == -1)
		{
			_atkDirection = { -1,0 };
			_atkAngle = 270;
			_reverseImg = true;
		}
	}
	else if (_direction.y == -2)
	{
		if (_direction.x == 1)
		{
			_atkDirection = { 0,-2 };
			_atkAngle = 90;
			_reverseImg = true;
		}
		else if (_direction.x == -1)
		{
			_atkDirection = { -1,-2 };
			_atkAngle = 270;
			_reverseImg = false;
		}
	}
	else if (_direction.x == 2)
	{
		if (_direction.y == 1)
		{
			_atkDirection = { 0,0 };
			_atkAngle = 180;
			_reverseImg = true;
		}
		else if (_direction.y == -1)
		{
			_atkDirection = { 0,-1 };
			_atkAngle = 0;
			_reverseImg = false;
		}
	}
	else if (_direction.x == -2)
	{
		if (_direction.y == 1)
		{
			_atkDirection = { -2,0 };
			_atkAngle = 180;
			_reverseImg = false;
		}
		else if (_direction.y == -1)
		{
			_atkDirection = { -2,-1 };
			_atkAngle = 0;
			_reverseImg = true;
		}
	}
	_isAttack = true;
}
