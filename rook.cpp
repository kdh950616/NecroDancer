#include "stdafx.h"
#include "rook.h"
#include "player.h"


rook::rook()
{
}


rook::~rook()
{
}

HRESULT rook::init()
{
	imageInit();
	
	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED * 1.5;
	_dmg = 2;
	_posZ = 0;
	_enemyType = ROOK;
	_isNeedAstar = true;
	_savePos = _posLT;
	_gold = 0;
	_isFind = false;

	return S_OK;
}

void rook::release()
{
}

void rook::update()
{
	if (_isFind && _isBeat)
	{
		_beatCount++;
		_isBeat = false;

		if (_beatCount == 1)
		{
			_ani = KEYANIMANAGER->findAnimation("rook", "rook_Attack");
			_ani->start();
		}
		else if (_beatCount >= 2)
		{
			_beatCount = 0;

			_ani = KEYANIMANAGER->findAnimation("rook", "rook_Stand");
			_ani->start();

			if (_listPath.size() > 0)
			{
				_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
			}
			else
			{

			}

			_savePos = _posLT;
			
			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				attackPlayer(_dmg);

				SOUNDMANAGER->playEff("piece_Attack");
			}
			else if (_direction.y == 1)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.y += _speed;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
			else if (_direction.y == -1)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.y -= _speed;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
			else if (_direction.x == 1)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.x += _speed;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
			else if (_direction.x == -1)
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_vec.x -= _speed;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
		}
	}
	if (_isMove)
	{
		moveCal();
	}
	if (_isCastling)
	{
		_dustAni->start();
		_isMove = true;
		if (_direction.x == 2)
		{
			_vec.x = _speed;
		}
		else if (_direction.x == -2)
		{
			_vec.x -= _speed;
		}
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
		(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
		_jumpPower = JUMPPOWER_HORIZON;
		_posZ = 0;
		_isCastling = false;
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

void rook::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 3, _posLT.y - 15 - CAMERA->getPosY(), 1.0f);

	_img->aniRender(_posLT.x - CAMERA->getPosX() + 10, _posLT.y - 20 - _posZ - CAMERA->getPosY(), _ani);

	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

void rook::imageInit()
{
	IMAGEMANAGER->addFrameImage("rook", L"images/monster/boss/rook_red.png", 114, 104, 3, 2);
	_img = IMAGEMANAGER->findImage("rook");

	KEYANIMANAGER->addAnimationType("rook");

	int stand[] = { 0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("rook", "rook_Stand", "rook", stand, 2, ANIMATION_FPS, true);

	int attack[] = { 2 };
	KEYANIMANAGER->addArrayFrameAnimation("rook", "rook_Attack", "rook", attack, 1, 1, true);

	_ani = KEYANIMANAGER->findAnimation("rook", "rook_Stand");
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

void rook::moveCal()
{
	_posZ += _jumpPower;
	_jumpPower -= GRAVITY;

	if ((_posLT.x - _savePos.x >= TILESIZE || _savePos.x - _posLT.x >= TILESIZE) && abs(_direction.x) == 1)
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
		_jumpPower = 0;
		_posZ = 0;
	}
	else if ((_posLT.x - _savePos.x >= TILESIZE * 2 || _savePos.x - _posLT.x >= TILESIZE * 2) && abs(_direction.x) != 1)
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
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;
		_isFind = true;
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

void rook::firstMoveCal()
{
}
