#include "stdafx.h"
#include "bishop.h"
#include "player.h"


bishop::bishop()
{
}


bishop::~bishop()
{
}

HRESULT bishop::init()
{
	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 2;
	_posZ = 0;
	_enemyType = BISHOP;
	_isNeedAstar = false;
	_savePos = _posLT;
	_gold = 0;
	_isFind = false;


	return S_OK;
}

void bishop::release()
{
}

void bishop::update()
{
	if (_isFind && _isBeat)
	{
		_beatCount++;
		_isBeat = false;

		if (_beatCount == 3)
		{
			_ani = KEYANIMANAGER->findAnimation("bishop", "bishop_Attack");
			_ani->start();
		}
		else if (_beatCount >= 4)
		{
			_beatCount = 0;

			_ani = KEYANIMANAGER->findAnimation("bishop", "bishop_Stand");
			_ani->start();

			if ((*_player).getIdx().x > _idx.x)
			{
				_direction.x = 1;
			}
			else if ((*_player).getIdx().x < _idx.x)
			{
				_direction.x = -1;
			}
			else
			{
				while (1)
				{
					_direction.x = RND->getFromIntTo(-1, 2);
					if (_direction.x != 0)break;
				}
			}
			
			if ((*_player).getIdx().y > _idx.y)
			{
				_direction.y = 1;
			}
			else if ((*_player).getIdx().y < _idx.y)
			{
				_direction.y = -1;
			}
			else
			{
				while (1)
				{
					_direction.y = RND->getFromIntTo(-1, 2);
					if (_direction.y != 0)break;
				}
			}

			_savePos = _posLT;

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				attackPlayer(_dmg);

				SOUNDMANAGER->playEff("piece_Attack");
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false || _direction.x == 0 || _direction.y == 0)
			{
				//암것도안함
			}
			else if (_direction.x == -1)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				//왼쪽위
				if (_direction.y == -1)
				{
					_vec.x -= _speed;
					_vec.y -= _speed;
					_posZ = 0;
				}
				//왼쪽아래
				else if (_direction.y == 1)
				{
					_vec.x -= _speed;
					_vec.y = _speed;
					_posZ = 0;
				}
			}
			else if (_direction.x == 1)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				//오른쪽위
				if (_direction.y == -1)
				{
					_vec.x = _speed;
					_vec.y -= _speed;
					_posZ = 0;
				}
				//오른쪽아래
				else if (_direction.y == 1)
				{
					_vec.x = _speed;
					_vec.y = _speed;
					_posZ = 0;
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

void bishop::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 3, _posLT.y - 15 - CAMERA->getPosY(), 1.0f);

	_img->aniRender(_posLT.x - CAMERA->getPosX() + 10, _posLT.y - 20 - _posZ - CAMERA->getPosY(), _ani);

	if (_isAttack && _direction.x > 0)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
	if (_isAttack && _direction.x < 0)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

void bishop::imageInit()
{
	IMAGEMANAGER->addFrameImage("bishop", L"images/monster/boss/bishop_red.png", 180, 128, 5, 2);
	_img = IMAGEMANAGER->findImage("bishop");

	KEYANIMANAGER->addAnimationType("bishop");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("bishop", "bishop_Stand", "bishop", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4 };
	KEYANIMANAGER->addArrayFrameAnimation("bishop", "bishop_Attack", "bishop", attack, 1, 1, true);

	_ani = KEYANIMANAGER->findAnimation("bishop", "bishop_Stand");
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

void bishop::moveCal()
{

	if (_posLT.x - _savePos.x >= TILESIZE || _savePos.x - _posLT.x >= TILESIZE &&
		_posLT.y - _savePos.y >= TILESIZE || _savePos.y - _posLT.y >= TILESIZE)
	{
		if (_vec.x > 0)
		{
			_posLT.x = _savePos.x + TILESIZE;
		}
		else if (_vec.x < 0)
		{
			_posLT.x = _savePos.x - TILESIZE;
		}
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
		_vec.x = 0;
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;
	}

//	if (_posLT.y - _savePos.y >= sqrtf(TILESIZE * 2) || _savePos.y - _posLT.y >= sqrtf(TILESIZE * 2))
//	{
//		float a = sqrtf(TILESIZE * 2);
//		if (_vec.y > 0)
//		{
//			_posLT.y = _savePos.y + sqrtf(TILESIZE * 2);
//		}
//		else if (_vec.y < 0)
//		{
//			_posLT.y = _savePos.y - sqrtf(TILESIZE * 2);
//		}
//		_vec.y = 0;
//		_savePos = _posLT;
//		_isMove = false;
//		_direction = { 0,0 };
//		_jumpPower = 0;
//		_posZ = 0;
//	}


	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
}
