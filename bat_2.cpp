#include "stdafx.h"
#include "bat_2.h"
#include "player.h"


bat_2::bat_2()
{
}


bat_2::~bat_2()
{
}

HRESULT bat_2::init()
{
	enemy::init();

	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 2;
	_enemyType = BAT;
	_beatCount = 0;
	_posZ = 30;
	_isNeedAstar = false;
	_gold = 2;

	return S_OK;
}

void bat_2::release()
{
}

void bat_2::update()
{
	if (CAMERA->getPosX() - TILESIZE <= _posLT.x
		&& CAMERA->getPosY() - TILESIZE <= _posLT.y
		&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
		&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
	{
		if (_isBeat)
		{
			_beatCount++;
			_isBeat = false;

			lightFunc();


			if (_beatCount == 1)
			{
				_beatCount = 0;
				_savePos = _posLT;
				while (1)
				{
					//�������� ���� ���ϰ�
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

					SOUNDMANAGER->playEff("bat_Attack");
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

	if (_isAttack)
	{
		countAttackEffect();
	}
}

void bat_2::render()
{
	_shadowImg->render(			_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRender(		_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

void bat_2::imageInit()
{
	_img = IMAGEMANAGER->findImage("bat_Red");

	KEYANIMANAGER->addAnimationType("bat_Red");
	KEYANIMANAGER->addAnimationType("bat_Red_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("bat_Red", "bat_Red_Stand", "bat_Red", stand, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("bat_Red_Shadow", "bat_Red_Stand", "bat_Red", stand_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("bat_Red_Shadow", "bat_Red_Stand");
	_ani->start();

	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	_dustImg = nullptr;
	_dustAni = nullptr;
}

void bat_2::lightFunc()
{
	//Ƚ�ҹ��� �ȿ� ������
	if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() <= 0.64f)
	{
		//����
		_isLight = true;
	}
	//Ƚ�� ���� �ۿ�������
	else if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() > 0.64f)
	{
		//��ο�
		_isLight = false;
	}

	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("bat_Red", "bat_Red_Stand");
		_ani->start();
	}
	else if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("bat_Red_Shadow", "bat_Red_Stand");
		_ani->start();
	}
}

void bat_2::moveCal()
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
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
}
