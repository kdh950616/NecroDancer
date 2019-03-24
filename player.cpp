#include "stdafx.h"
#include "player.h"
#include "enemyMgr.h"


player::player()
{
}


player::~player()
{
}

HRESULT player::init()
{
	imgInit();


	//일단 걍 생성
	//_posCT = { (float)(*_vvMap)[20][6]->getPos().x, (float)(*_vvMap)[20][6]->getPos().y};
	_savePos = _posCT;
	_rc = {	(float)_posCT.x,
			(float)_posCT.y,
			(float)_posCT.x + TILESIZE,
			(float)_posCT.y + TILESIZE};
	_dmg = 1;
	_speed = SPEED;
	_jumpPower = JUMPPOWER_HORIZON;
	_isMove = false;
	_isReverse = false;
	_isArrive = true;
	_shovel = BASIC;
	_shovel_Dmg = 1;
	_direction = { 0,0 };

	return S_OK;
}

void player::release()
{
}

void player::update()
{
	CAMERA->setPosX(_posLT.x - WINSIZEX / 2);
	CAMERA->setPosY(_posLT.y - WINSIZEY / 2);

	move();
	moveCal();
	KEYANIMANAGER->update("playerHead");
	KEYANIMANAGER->update("playerBody");
}

void player::render()
{

	WCHAR str[128];
	swprintf_s(str, L"tileX : %d tileY : %d ", (*_vvMap)[10][10]->getPos().x, (*_vvMap)[10][10]->getPos().y);
	D2DMANAGER->drawText(str, 0, 260, 20, 0x00ffff);
	swprintf_s(str, L"ct X : %f ct Y : %f ", _posCT.x, _posCT.y);
	D2DMANAGER->drawText(str, 0, 280, 20, 0x00ffff);
	swprintf_s(str, L"idx : %d idy : %d ", _idx.x, _idx.y);
	D2DMANAGER->drawText(str, 0, 300, 20, 0x00ffff);
	swprintf_s(str, L"HP : %d ", _curHp);
	D2DMANAGER->drawText(str, 0, 320, 20, 0x00ffff);
	//D2DMANAGER->fillRectangle(0xff0000, _posLT.x, _posLT.y, _posLT.x + TILESIZE, _posLT.y + TILESIZE, 1.0);
	//D2DMANAGER->fillRectangle(0x00ff00, _posCT.x, _posCT.y, _posCT.x + TILESIZE, _posCT.y + TILESIZE, 1.0);
	//D2DMANAGER->fillRectangle(0x0000ff, _savePos.x, _savePos.y, _savePos.x + TILESIZE, _savePos.y + TILESIZE, 1.0);

	_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - CAMERA->getPosY(), 1.0f);
	if (_isReverse)
	{
		_bodyImg->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _bodyAni);
		_headImg->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _headAni);
	}
	else if(!_isReverse)
	{
		_bodyImg->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _bodyAni);
		_headImg->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _headAni);
	}
}

void player::attackFunc()
{
	for (int i = 0; i < _em->getVEnemy().size(); i++)
	{
		if (_em->getVEnemy()[i]->getIdx().x == _idx.x + _direction.x && _em->getVEnemy()[i]->getIdx().y == _idx.y + _direction.y)
		{
			if (_em->getVEnemy()[i]->getEnemyType() == ARMADILLO && !_em->getVEnemy()[i]->getIsStun())
			{
				SOUNDMANAGER->playEff("armadillo_Block");
				_em->getVEnemy()[i]->setDirection(_direction);
			}
			else if (_em->getVEnemy()[i]->getEnemyType() == BANSHEE)
			{
				_em->getVEnemy()[i]->setDirection(_direction);
				_em->getVEnemy()[i]->setCurHp(_em->getVEnemy()[i]->getCurHp() - _dmg);

				int rnd = RND->getFromIntTo(1, 4);
				if (_em->getVEnemy()[i]->getMaxHp() - _em->getVEnemy()[i]->getCurHp() == 1)
				{
					SOUNDMANAGER->setZoneVolume(*_em->getStageKeyName(), SOUNDMANAGER->getVolume() / 25);
					SOUNDMANAGER->playEff("banshee_Loop");
					float tmp = SOUNDMANAGER->getEffectVol() / 10;
					SOUNDMANAGER->setEffectVol(SOUNDMANAGER->getEffectVol() / 10);
					tmp = SOUNDMANAGER->getEffectVol();
				}

				switch (rnd)
				{
				case 1:
					SOUNDMANAGER->playEff("banshee_Hit1");
					break;
				case 2:
					SOUNDMANAGER->playEff("banshee_Hit2");
					break;
				case 3:
					SOUNDMANAGER->playEff("banshee_Hit3");
					break;
				}
			}
			else
			{
				_em->getVEnemy()[i]->setCurHp(_em->getVEnemy()[i]->getCurHp() - _dmg);
				int rnd;
				switch (_em->getVEnemy()[i]->getEnemyType())
				{
					case BAT:
						SOUNDMANAGER->playEff("bat_Hit");
					break;
					case SKELETON:
						rnd = RND->getFromIntTo(1, 5);
						switch (rnd)
						{
						case 1:
							SOUNDMANAGER->playEff("skeleton_Hit1");
							break;
						case 2:
							SOUNDMANAGER->playEff("skeleton_Hit2");
							break;
						case 3:
							SOUNDMANAGER->playEff("skeleton_Hit3");
							break;
						case 4:
							SOUNDMANAGER->playEff("skeleton_Hit4");
							break;
						}
					break;
					case SLIME:
						rnd = RND->getFromIntTo(1, 5);
						switch (rnd)
						{
						case 1:
							SOUNDMANAGER->playEff("slime_Hit1");
							break;
						case 2:
							SOUNDMANAGER->playEff("slime_Hit2");
							break;
						case 3:
							SOUNDMANAGER->playEff("slime_Hit3");
							break;
						case 4:
							SOUNDMANAGER->playEff("slime_Hit4");
							break;
						}
					break;
					case ARMADILLO:
						SOUNDMANAGER->playEff("armadillo_Hit");
					break;
					case DRAGON:
						rnd = RND->getFromIntTo(1, 4);
						switch (rnd)
						{
						case 1:
							SOUNDMANAGER->playEff("dragon_Hit1");
							break;
						case 2:
							SOUNDMANAGER->playEff("dragon_Hit2");
							break;
						case 3:
							SOUNDMANAGER->playEff("dragon_Hit3");
							break;
						}
					break;
					case BAT_BOSS:
						SOUNDMANAGER->playEff("bat_Boss_Hit");
					break;
					//case PAWN:
					//break;
					//case KNIGHT:
					//break;
					//case BISHOP:
					//break;
					//case ROOK:
					//break;
					//case QUEEN:
					//break;
					//case KING:
					//break;
					//case CORALRIFF:
					//break;
					//case PIANO:
					//break;
					//case VIOLIN:
					//break;
					//case TRUNPET:
					//break;
					//case DRUM:
					//break;
					//case END:
					//break;
				}

				break;
			}


		}
	}
}

//===========================================
//					init
//===========================================

void player::imgInit()
{
	IMAGEMANAGER->addFrameImage("playerHead", L"images/player/head.png", 192, 96, 4, 2);
	IMAGEMANAGER->addFrameImage("playerBody", L"images/player/body.png", 192, 480, 4, 10);
	IMAGEMANAGER->addImage("playerShadow", L"images/player/shadow.png", 48, 54);

	int head[] = { 0,1,2,3,4,5,6,7 };
	KEYANIMANAGER->addAnimationType("playerHead");
	KEYANIMANAGER->addArrayFrameAnimation("playerHead", "head", "playerHead", head, 8, 8, true);

	int normalBody[] = { 0,1,2,3 };
	KEYANIMANAGER->addAnimationType("playerBody");
	KEYANIMANAGER->addArrayFrameAnimation("playerBody", "normalBody", "playerBody", normalBody, 4, 8, true);

	_headImg = IMAGEMANAGER->findImage("playerHead");
	_headAni = KEYANIMANAGER->findAnimation("playerHead", "head");
	_headAni->start();

	_bodyImg = IMAGEMANAGER->findImage("playerBody");
	_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "normalBody");
	_bodyAni->start();

	_shadowImg = IMAGEMANAGER->findImage("playerShadow");

	EFFECTMANAGER->addEffect("shovel_basic", "images/item/shovel/shovel_basic.png", 48, 48, 48, 48, 1, 0.1, 50);

	//방어구 입었을때 이미지들도 나중에 비교해서 만들어놓자. 일단은 그냥 기본몸만.
}

//===========================================
//					update
//===========================================

void player::move()
{
	if (_isPressKey)
	{
		_isPressKey = false;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT) && !_isMove)
	{
		_isPressKey = true;
		if (_isBeat)
		{
			_direction = { -1, 0 };
			_isReverse = true;
			//못가는 곳이엇을때
			if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{
				//그게 벽이면
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1
					&& (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE)
				{
					//벽없애기
					objFunc(_direction);
				}
				//벽이 아닌 다른거 -> 적이면
				else
				{
					//적만큼 돌면서 내가 공격한곳 인덱스랑 같은곳에 위치한놈 찾아내고 그놈 피 깎음
					attackFunc();
				}
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
			{
				_savePos = _posCT;
				_vec.x -= _speed;
				_isArrive = false;
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				//				움직일때마다 도착한곳은 갈수있냐? -> 펄스로  내가 전에있던곳은 트루로 바꿔줘야함
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				_isMove = true;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
			_isBeat = false;
		}
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && !_isMove)
	{
		_isPressKey = true;
		if (_isBeat)
		{
			_direction = { 1, 0 };
			_isReverse = false;
			if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1
					&& (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE)
				{
					objFunc(_direction);
				}
				else
				{
					attackFunc();
				}
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
			{
				_savePos = _posCT;
				_vec.x += _speed;
				_isArrive = false;
				_isMove = true;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
			_isBeat = false;
		}
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_UP) && !_isMove)
	{
		_isPressKey = true;
		if (_isBeat)
		{
			_direction = { 0, -1 };
			if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1
					&& (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE)
				{
					objFunc(_direction);
				}
				else
				{
					attackFunc();
				}
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
			{
				_savePos = _posCT;
				_vec.y -= _speed;
				_isArrive = false;
				_isMove = true;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
			_isBeat = false;
		}
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && !_isMove)
	{
		_isPressKey = true;
		if (_isBeat)
		{
			_direction = { 0, 1 };
			if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1
					&& (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE)
				{
					objFunc(_direction);
				}
				else
				{
					attackFunc();
				}
			}
			else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
			{
				_savePos = _posCT;
				_vec.y += _speed;
				_isArrive = false;
				_isMove = true;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
			_isBeat = false;
		}
	}
	

	
}

void player::moveCal()
{
	if (_isMove)
	{
		_posZ += _jumpPower;
		_jumpPower -= GRAVITY;
	}
	else if (!_isMove)
	{
		_jumpPower = 0;
		_posZ = 0;
	}
	if (_posCT.x - _savePos.x >= TILESIZE || _savePos.x - _posCT.x >= TILESIZE)
	{
		if (_vec.x > 0)
		{
			_posCT.x = _savePos.x + TILESIZE;
		}
		else if (_vec.x < 0)
		{
			_posCT.x = _savePos.x - TILESIZE;
		}
		_vec.x = 0;
		_savePos = _posCT;
		_isMove = false;
		_isArrive = true;
		_direction = { 0,0 };
	}

	if (_posCT.y - _savePos.y >= TILESIZE || _savePos.y - _posCT.y >= TILESIZE)
	{
		if (_vec.y > 0)
		{
			_posCT.y = _savePos.y + TILESIZE;
		}
		else if (_vec.y < 0)
		{
			_posCT.y = _savePos.y - TILESIZE;
		}
		_vec.y = 0;
		_savePos = _posCT;
		_isMove = false;
		_isArrive = true;
		_direction = { 0,0 };
	}



	_posCT.x += _vec.x;
	_posCT.y += _vec.y;
	_posLT.x = _posCT.x - TILESIZE / 2;
	_posLT.y = _posCT.y - TILESIZE / 2;
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
}

void player::objFunc(POINT direction)
{
	switch ((*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->getAttribute())
	{
	case OBJ_WALL1:
		EFFECTMANAGER->play("shovel_basic", _posCT.x + (direction.x * TILESIZE) - CAMERA->getPosX(), _posCT.y + (direction.y * TILESIZE) - 35 - CAMERA->getPosY());
		if (_shovel_Dmg > 0)
		{
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		}
		break;
	case OBJ_WALL2:
		EFFECTMANAGER->play("shovel_basic", _posCT.x + (direction.x * TILESIZE) - CAMERA->getPosX(), _posCT.y + (direction.y * TILESIZE) - 35 - CAMERA->getPosY());
		if (_shovel_Dmg > 1)
		{
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		}
		break;
	case OBJ_WALL3:
		EFFECTMANAGER->play("shovel_basic", _posCT.x + (direction.x * TILESIZE) - CAMERA->getPosX(), _posCT.y + (direction.y * TILESIZE) - 35 - CAMERA->getPosY());
		if (_shovel_Dmg > 2)
		{
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		}
		break;
	case OBJ_WALL_GOLD:
		EFFECTMANAGER->play("shovel_basic", _posCT.x + (direction.x * TILESIZE) - CAMERA->getPosX(), _posCT.y + (direction.y * TILESIZE) - 35 - CAMERA->getPosY());
		//(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		break;
	case OBJ_WALL_BOSS:
		EFFECTMANAGER->play("shovel_basic", _posCT.x + (direction.x * TILESIZE) - CAMERA->getPosX(), _posCT.y + (direction.y * TILESIZE) - 35 - CAMERA->getPosY());
		//(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		break;
	case OBJ_WALL_END:
		EFFECTMANAGER->play("shovel_basic", _posCT.x + (direction.x * TILESIZE) - CAMERA->getPosX(), _posCT.y + (direction.y * TILESIZE) - 35 - CAMERA->getPosY());
		//(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		break;
	case OBJ_DOOR_FRONT:
		(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		break;
	case OBJ_DOOR_SIDE:
		(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		break;
		//case ENEMY1_1_1:
		//
		//break;
		//case ENEMY1_1_2:
		//
		//break;
		//case ENEMY1_2_1:
		//
		//break;
		//case ENEMY1_2_2:
		//
		//break;
		//case ENEMY1_2_3:
		//
		//break;
		//case ENEMY1_3_1:
		//
		//break;
		//case ENEMY1_3_2:
		//
		//break;
		//case ENEMY1_3_3:
		//
		//break;
		//case ENEMY2_1_1:
		//
		//break;
		//case ENEMY2_1_2:
		//
		//break;
		//case ENEMY2_2_1:
		//
		//break;
		//case ENEMY2_2_2:
		//
		//break;
		//case ENEMY3_1_1:
		//
		//break;
		//case ENEMY3_1_2:
		//
		//break;
		//case ENEMY3_1_3:
		//
		//break;
		//case ENEMY4_1_1:
		//
		//break;
		//case ENEMY4_1_2:
		//
		//break;
		//case ENEMY4_1_3:
		//
		//break;
		//case ENEMY4_2_1:
		//
		//break;
		//case ENEMY4_2_2:
		//
		//break;
		//case ENEMY4_2_3:
		//
		//break;
	default:
		//_savePos = _posCT;
		//_vec.x -= _speed;
		//_isMove = true;
		//_jumpPower = JUMPPOWER_HORIZON;
		//_isReverse = true;
		//_posZ = 0;
		break;
	}
}

void player::enemyAtk()
{

}


//===========================================
//					render
//===========================================


//===========================================
//					get
//===========================================


//===========================================
//					set
//===========================================


