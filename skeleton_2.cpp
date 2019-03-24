#include "stdafx.h"
#include "skeleton_2.h"
#include "player.h"


skeleton_2::skeleton_2()
{
}


skeleton_2::~skeleton_2()
{
}

HRESULT skeleton_2::init()
{
	imageInit();

	_maxHp = 2;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 3;
	_posZ = 0;
	_enemyType = SKELETON;
	_isNeedAstar = true;


	return S_OK;
}

void skeleton_2::release()
{
}

void skeleton_2::update()
{
	//발견되지 않았고 카메라범위 안에있다면
	if (!_isFind
		&& CAMERA->getPosX() - TILESIZE <= _posLT.x
		&& CAMERA->getPosY() - TILESIZE <= _posLT.y
		&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
		&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
	{
		unfoundFunc();
	}
	//발견됐고  비트타이밍이면
	else if (_isFind && _isBeat)
	{
		//비트카운트 증가
		_beatCount++;		//비트카운트 증가
		_isBeat = false;	//비트타이밍 펄스

		lightFunc();

		if (_curHp <= 1 && !_isSpecialAct)
		{
			_isSpecialAct = true;
			_direction.x = -1 * (_listPath.begin()->x - _idx.x);
			_direction.y = -1 * (_listPath.begin()->y - _idx.y);
			int rnd = RND->getFromIntTo(1, 4);
			switch (rnd)
			{
				case 1:
					SOUNDMANAGER->playEff("skeleton_HeadLess1");
				break;
				case 2:
					SOUNDMANAGER->playEff("skeleton_HeadLess2");
				break;
				case 3:
					SOUNDMANAGER->playEff("skeleton_HeadLess3");
				break;
	
			}
		}

		if (!_isSpecialAct)
		{
			//비트카운트가 1이면 -> 손드는 이미지로 변경
			if (_beatCount == 1)
			{
				attackAni_LightChange();
			}
			//비트카운트가 2면 -> 이동하자
			else if (_beatCount == 2)
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
				_savePos = _posLT;

				standAni_LightChange();

				//에이슷하로 구한 내가 가야할길이 플레이어라면 
				if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
				{
					(*_player).setCurHp((*_player).getCurHp() - _dmg);

					showAttackEffect();
					SOUNDMANAGER->playEff("skeleton_Attack");
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
			}
		}
		else if (_isSpecialAct)
		{
			_savePos = _posLT;
			lightFunc();
			headlessAni_LightChange();

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				(*_player).setCurHp((*_player).getCurHp() - _dmg);

				showAttackEffect();
			}
			else if (_direction.x == 0)
			{
				_dustAni->start();
				if (_direction.y == 1)
				{
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
					_vec.y += SPEED;
					_isMove = true;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
				else if (_direction.y == -1)
				{
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
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
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
					_vec.x += SPEED;
					_isMove = true;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
				else if (_direction.x == -1)
				{
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
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
	//움직여야하면
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

void skeleton_2::render()
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
}

void skeleton_2::imageInit()
{
	IMAGEMANAGER->addFrameImage("skeleton_Yellow", L"images/monster/normal/skeleton_yellow.png", 432, 100, 9, 2);
	_img = IMAGEMANAGER->findImage("skeleton_Yellow");

	//이펙트 등록
	EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	KEYANIMANAGER->addAnimationType("skeleton_Yellow");
	KEYANIMANAGER->addAnimationType("skeleton_Yellow_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Yellow", "skeleton_Yellow_Stand", "skeleton_Yellow", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Yellow", "skeleton_Yellow_Attack", "skeleton_Yellow", attack, 4, ANIMATION_FPS, false);

	int headless[] = { 8 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Yellow", "skeleton_Yellow_Headless", "skeleton_Yellow", headless, 1, ANIMATION_FPS, true);

	int stand_Shadow[] = { 9,10,11,12 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Yellow_Shadow", "skeleton_Yellow_Stand", "skeleton_Yellow", stand_Shadow, 4, ANIMATION_FPS, true);

	int attack_Shadow[] = { 13,14,15,16 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Yellow_Shadow", "skeleton_Yellow_Attack", "skeleton_Yellow", attack_Shadow, 4, ANIMATION_FPS, false);

	int headless_Shadow[] = { 17 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Yellow_Shadow", "skeleton_Yellow_Headless", "skeleton_Yellow", headless_Shadow, 1, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow_Shadow", "skeleton_Yellow_Stand");
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

void skeleton_2::unfoundFunc()
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
			_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow", "skeleton_Yellow_Stand");
			_ani->start();
		}
	}
}

void skeleton_2::lightFunc()
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

void skeleton_2::attackAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow", "skeleton_Yellow_Attack");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow_Shadow", "skeleton_Yellow_Attack");
		_ani->start();
	}
}

void skeleton_2::standAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow", "skeleton_Yellow_Stand");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow_Shadow", "skeleton_Yellow_Stand");
		_ani->start();
	}
}

void skeleton_2::headlessAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow", "skeleton_Yellow_Headless");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Yellow_Shadow", "skeleton_Yellow_Headless");
		_ani->start();
	}
}

void skeleton_2::moveCal()
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

void skeleton_2::specialMoveCal()
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
				}
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
				(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
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
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
				(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
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
		}
	}
	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;

}
