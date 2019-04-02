#include "stdafx.h"
#include "zombie.h"
#include "player.h"


zombie::zombie()
{
}


zombie::~zombie()
{
}

HRESULT zombie::init()
{
	enemy::init();

	while (1)
	{
		_direction = { RND->getFromIntTo(-1, 2), RND->getFromIntTo(-1,2) };
		if (abs(_direction.x) != abs(_direction.y))
		{
			if (_direction.x == 0)
			{
				_isLeft = true;
			}
			break;
		}
	}

	imageInit();

	_maxHp = 1;
	_curHp = 1;
	_speed = SPEED;
	_dmg = 2;
	_posZ = 0;
	_enemyType = ZOMBIE;
	_isNeedAstar = false;

	_gold = 1;

	_showPos = { 0,15 };

	return S_OK;
}

void zombie::release()
{
}

void zombie::update()
{
	if (_isBeat)
	{
		_beatCount++;
		_isBeat = false;

		lightFunc();
		change_Ani_LightChange();


		
		if (_beatCount == 2)
		{
			_beatCount = 0;

			_savePos = _posLT;

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				attackPlayer(_dmg);

				SOUNDMANAGER->playEff("zombie_Attack");
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{
				_isSpecialAct = true;
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
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
				else if (_direction.y == -1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isMove = true;
					_vec.y -= SPEED;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
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
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
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
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
			}

			if (_isSpecialAct)
			{
				if (_direction.x == 0)
				{
					_dustAni->start();
					if (_direction.y == 1)
					{
						_vec.y += SPEED;
						_isMove = true;
						_jumpPower = JUMPPOWER_VERTICAL;
						_posZ = 0;
						_idx.x = (_posLT.x) / TILESIZE;
						_idx.y = (_posLT.y) / TILESIZE;
					}
					else if (_direction.y == -1)
					{
						_vec.y -= SPEED;
						_isMove = true;
						_jumpPower = JUMPPOWER_VERTICAL;
						_posZ = 0;
						_idx.x = (_posLT.x) / TILESIZE;
						_idx.y = (_posLT.y) / TILESIZE;
					}
				}
				else if (_direction.y == 0)
				{
					_dustAni->start();
					if (_direction.x == 1)
					{
						_vec.x += SPEED;
						_isMove = true;
						_jumpPower = JUMPPOWER_HORIZON;
						_posZ = 0;
						_idx.x = (_posLT.x) / TILESIZE;
						_idx.y = (_posLT.y) / TILESIZE;
					}
					else if (_direction.x == -1)
					{
						_vec.x -= SPEED;
						_isMove = true;
						_jumpPower = JUMPPOWER_HORIZON;
						_posZ = 0;
						_idx.x = (_posLT.x) / TILESIZE;
						_idx.y = (_posLT.y) / TILESIZE;
					}
				}
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

void zombie::render()
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

void zombie::imageInit()
{
	IMAGEMANAGER->addFrameImage("zombie", L"images/monster/normal/zombie.png", 1152, 100, 24, 2);
	_img = IMAGEMANAGER->findImage("zombie");

	//EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.f, 10);

	KEYANIMANAGER->addAnimationType("zombie");
	KEYANIMANAGER->addAnimationType("zombie_Shadow");

	int up[] = { 0,1,2,3,4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("zombie", "zombie_Up", "zombie", up, 8, ANIMATION_FPS, true);

	int side[] = { 8,9,10,11,12,13,14,15 };
	KEYANIMANAGER->addArrayFrameAnimation("zombie", "zombie_Side", "zombie", side, 8, ANIMATION_FPS, true);

	int down[] = { 16,17,18,19,20,21,22,23 };
	KEYANIMANAGER->addArrayFrameAnimation("zombie", "zombie_Down", "zombie", down, 8, ANIMATION_FPS, true);


	int up_Shadow[] = { 24,25,26,27,28,29,30,31 };
	KEYANIMANAGER->addArrayFrameAnimation("zombie_Shadow", "zombie_Up", "zombie", up_Shadow, 8, ANIMATION_FPS, true);

	int side_Shadow[] = { 32,33,34,35,36,37,38,39 };
	KEYANIMANAGER->addArrayFrameAnimation("zombie_Shadow", "zombie_Side", "zombie", side_Shadow, 8, ANIMATION_FPS, true);

	int down_Shadow[] = { 40,41,42,43,44,45,46,47 };
	KEYANIMANAGER->addArrayFrameAnimation("zombie_Shadow", "zombie_Down", "zombie", down_Shadow, 8, ANIMATION_FPS, true);

	change_Ani_LightChange();

	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);
	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

void zombie::change_Ani_LightChange()
{
	if (_direction.x == 1)
	{
		_isLeft = false;
		//밝음상태면	
		if (_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie", "zombie_Side");
		}
		//그림자상태면
		if (!_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie_Shadow", "zombie_Side");
		}
	}
	else if (_direction.x == -1)
	{
		_isLeft = true;
		if (_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie", "zombie_Side");
		}
		//그림자상태면
		if (!_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie_Shadow", "zombie_Side");
		}
	}
	else if (_direction.y == 1)
	{
		if (_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie", "zombie_Down");
		}
		//그림자상태면
		if (!_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie_Shadow", "zombie_Down");
		}
	}
	else if (_direction.y == -1)
	{
		//밝음상태면	
		if (_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie", "zombie_Up");
		}
		//그림자상태면
		if (!_isLight)
		{
			_ani = KEYANIMANAGER->findAnimation("zombie_Shadow", "zombie_Up");
		}
	}
	_ani->start();
}

void zombie::moveCal()
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
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
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
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
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

void zombie::specialMoveCal()
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
			_direction.x *= -1;
			_direction.y *= -1;
			change_Ani_LightChange();
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
			_direction.x *= -1;
			_direction.y *= -1;
			change_Ani_LightChange();
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
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
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
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
		}
	}

	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
}
