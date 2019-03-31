#include "stdafx.h"
#include "pawn.h"
#include "player.h"


pawn::pawn()
{
}


pawn::~pawn()
{
}

HRESULT pawn::init()
{
	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED * 2;
	_dmg = 2;
	_posZ = 0;
	_enemyType = PAWN;
	_isNeedAstar = false;
	_savePos = _posLT;
	_gold = 0;
	_isFind = true;

	return S_OK;
}

void pawn::release()
{
}

void pawn::update()
{
	if (_isFind && _isBeat && _isFirstMove)
	{
		_beatCount++;
		_isBeat = false;

		if (_beatCount == 7)
		{
			_ani = KEYANIMANAGER->findAnimation("pawn", "pawn_Attack");
			_ani->start();
		}
		else if (_beatCount >= 8)
		{
			_beatCount = 0;
			_direction = { 0,1 };

			_savePos = _posLT;

			_ani = KEYANIMANAGER->findAnimation("pawn", "pawn_Stand");
			//_ani->start();

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				
			}
			else if ((*_player).getIdx().x + 1 == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y ||
					(*_player).getIdx().x - 1 == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				attackPlayer(_dmg);

				SOUNDMANAGER->playEff("piece_Attack");
			}
			else if((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == OBJ_WALL_BOSS || (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_TORCH_BOSS)
				{
					//특정 불값 트루시키고 에너미 매니저에서 이거 불값이 트루면 해당 에너미 지워버리고 그자리에 퀸생성
				}
			}
			else
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.y += _speed;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
		}
	}
	if (_isFind && _isBeat && !_isFirstMove)
	{
		_beatCount++;
		_isBeat = false;

		if (_beatCount == 7)
		{
			_ani = KEYANIMANAGER->findAnimation("pawn", "pawn_Attack");
			_ani->start();
		}
		else if (_beatCount >= 8)
		{
			_beatCount = 0;
			_direction = { 0,2 };

			_savePos = _posLT;

			_ani = KEYANIMANAGER->findAnimation("pawn", "pawn_Stand");
			//_ani->start();

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				_isFirstMove = true;
				_isMove = true;
			}
			else if ((*_player).getIdx().x + 1 == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + 1 ||
				(*_player).getIdx().x - 1 == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y +1)
			{
				attackPlayer(_dmg);

				SOUNDMANAGER->playEff("piece_Attack");
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false || (*_vvObj)[_idx.y + 1][_idx.x + _direction.x]->getIsAvailMove() == false)
			{
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == OBJ_WALL_BOSS || (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_TORCH_BOSS)
				{
					//특정 불값 트루시키고 에너미 매니저에서 이거 불값이 트루면 해당 에너미 지워버리고 그자리에 퀸생성
				}
			}
			else
			{
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.y += _speed;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
		}
	}

	if (_isMove && _isFirstMove)
	{
		moveCal();
	}
	else if (_isMove && !_isFirstMove)
	{
		firstMoveCal();
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

void pawn::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 3 , _posLT.y - 15 - CAMERA->getPosY(), 1.0f);

	_img->aniRender(_posLT.x - CAMERA->getPosX() + 12, _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);

	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((*_player).getPosLT().x - CAMERA->getPosX(), (*_player).getPosLT().y - CAMERA->getPosY(), _atkEff_Frame, 0, 90);
	}
}

void pawn::imageInit()
{
	IMAGEMANAGER->addFrameImage("pawn", L"images/monster/boss/pawn_red.png", 150, 92, 5, 2);
	_img = IMAGEMANAGER->findImage("pawn");

	KEYANIMANAGER->addAnimationType("pawn");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("pawn", "pawn_Stand", "pawn", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4 };
	KEYANIMANAGER->addArrayFrameAnimation("pawn", "pawn_Attack", "pawn", attack, 1, 1, true);

	_ani = KEYANIMANAGER->findAnimation("pawn", "pawn_Stand");
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

void pawn::moveCal()
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
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
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
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		if (_idx.y >= 13)
		{
			_isSpecialAct = true;
		}
	}

	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE ||
		(*_vvObj)[_idx.y][_idx.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= ETC_TORCH_BOSS)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
	}
	else if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == OBJ_NONE && (*_vvObj)[_idx.y][_idx.x]->getIsAvailMove() == false)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
	else
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
}

void pawn::firstMoveCal()
{
	_posZ += _jumpPower;
	_jumpPower -= GRAVITY;

	if (_posLT.x - _savePos.x >= TILESIZE * 2 || _savePos.x - _posLT.x >= TILESIZE * 2)
	{
		if (_vec.x > 0)
		{
			_posLT.x = _savePos.x + TILESIZE * 2;
		}
		else if (_vec.x < 0)
		{
			_posLT.x = _savePos.x - TILESIZE * 2;
		}
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_vec.x = 0;
		_savePos = _posLT;
		_isMove = false;
		_isFirstMove = true;
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;
	}

	if (_posLT.y - _savePos.y >= TILESIZE * 2 || _savePos.y - _posLT.y >= TILESIZE * 2)
	{
		if (_vec.y > 0)
		{
			_posLT.y = _savePos.y + TILESIZE * 2;
		}
		else if (_vec.y < 0)
		{
			_posLT.y = _savePos.y - TILESIZE * 2;
		}
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;
		_isFirstMove = true;
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;
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
//else if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == OBJ_NONE && (*_vvObj)[_idx.y][_idx.x]->getIsAvailMove() == false)
//{
//	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
//}
//else
//{
//	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
//}
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	//(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
}
