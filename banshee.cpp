#include "stdafx.h"
#include "banshee.h"
#include "player.h"


banshee::banshee()
{
}


banshee::~banshee()
{
}

HRESULT banshee::init()
{
	enemy::init();

	imageInit();

	_maxHp = 4;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 5;
	_enemyType = BANSHEE;
	_beatCount = 0;
	_posZ = 30;
	_isNeedAstar = true;
	_gold = 10;

	return S_OK;
}

void banshee::release()
{
}

void banshee::update()
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
		SOUNDMANAGER->playEff("banshee_Cry");
	}
	else if (_isFind && _isBeat)
	{
		_isBeat = false;
		_savePos = _posLT;
		lightFunc();

		//풀피가 아니면 화난이미지로변경
		if (_curHp == _maxHp)
		{
			standAni_LightChange();
		}
		else attackAni_LightChange();

		if (_direction.x == 0 && _direction.y == 0)
		{
			if (_listPath.size() > 0)
			{
				_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
				if (_direction.x == 1)
				{
					_isLeft = false;
				}
				else if (_direction.x == -1)
				{
					_isLeft = true;
				}
			}
			else
			{
				POINT tmp = { RND->getInt(2),RND->getInt(2) };
				_listPath.push_back(tmp);
				_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
			}
		}

		if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
		{
			attackPlayer(_dmg);

			SOUNDMANAGER->playEff("banshee_Attack");
		}
		else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
		{
			_direction.x = 0;
			_direction.y = 0;
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
				_vec.x += SPEED;
			}
			else if (_direction.x == -1)
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.x -= SPEED;
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

void banshee::render()
{
	_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX() - 10, _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX() - 10, _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

void banshee::imageInit()
{
	IMAGEMANAGER->addFrameImage("banshee", L"images/monster/miniboss/banshee.png", 544, 156, 8, 2);
	_img = IMAGEMANAGER->findImage("banshee");

	KEYANIMANAGER->addAnimationType("banshee");
	KEYANIMANAGER->addAnimationType("banshee_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("banshee", "banshee_Stand", "banshee", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("banshee", "banshee_Attack", "banshee", attack, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 8,9,10,11 };
	KEYANIMANAGER->addArrayFrameAnimation("banshee_Shadow", "banshee_Stand", "banshee", stand_Shadow, 4, ANIMATION_FPS, true);

	int attack_Shadow[] = { 12,13,14,15 };
	KEYANIMANAGER->addArrayFrameAnimation("banshee_Shadow", "banshee_Attack", "banshee", attack_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("banshee_Shadow", "banshee_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	_dustImg = nullptr;
	_dustAni = nullptr;
}

void banshee::standAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("banshee", "banshee_Stand");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("banshee_Shadow", "banshee_Stand");
		_ani->start();
	}
}

void banshee::attackAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("banshee", "banshee_Attack");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("banshee_Shadow", "banshee_Attack");
		_ani->start();
	}
}

void banshee::moveCal()
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
