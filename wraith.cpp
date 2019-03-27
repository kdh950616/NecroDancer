#include "stdafx.h"
#include "wraith.h"
#include "player.h"

wraith::wraith()
{
}


wraith::~wraith()
{
}

HRESULT wraith::init()
{
	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 1;
	_posZ = 20;
	_enemyType = WRAITH;
	_isNeedAstar = true;
	_gold = 2;


	return S_OK;
}

void wraith::release()
{
}

void wraith::update()
{
	if (_isFind && _isBeat)
	{
		_beatCount++;
		_isBeat = false;


		lightFunc();

		//이프문 굳이 없어도 되는데 나중에 고치기 편하게 미리 만들좌
		if (_beatCount == 1)
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
				attackPlayer(_dmg);

				attackAni_LightChange();

				SOUNDMANAGER->playEff("wraith_Attack");
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
	if (_isMove)
	{
		moveCal();
	}

	if (_curHp <= 0)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
}

void wraith::render()
{
	if (_isFind)
	{
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
}

void wraith::imageInit()
{
	IMAGEMANAGER->addFrameImage("wraith", L"images/monster/normal/wraith.png", 144, 96, 3, 2);
	_img = IMAGEMANAGER->findImage("wraith");

	//EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.f, 10);

	KEYANIMANAGER->addAnimationType("wraith");
	KEYANIMANAGER->addAnimationType("wraith_Shadow");

	int stand[] = { 0,1,0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("wraith", "wraith_Stand", "wraith", stand, 4,ANIMATION_FPS, true);

	int attack[] = { 0,2,0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("wraith", "wraith_Attack", "wraith", attack, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 3,4,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("wraith_Shadow", "wraith_Stand", "wraith", stand_Shadow, 4, ANIMATION_FPS, true);

	int attack_Shadow[] = { 3,5,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("wraith_Shadow", "wraith_Attack", "wraith", attack_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("wraith_Shadow", "wraith_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	_dustImg = nullptr;
	_dustAni = nullptr;
}

void wraith::standAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("wraith", "wraith_Stand");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("wraith_Shadow", "wraith_Stand");
		_ani->start();
	}
}

void wraith::attackAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("wraith", "wraith_Attack");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("wraith_Shadow", "wraith_Attack");
		_ani->start();
	}
}

void wraith::moveCal()
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
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
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
