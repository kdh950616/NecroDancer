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
	inventoryInit();

	_savePos = _posCT;
	_rc = {	(float)_posCT.x,
			(float)_posCT.y,
			(float)_posCT.x + TILESIZE,
			(float)_posCT.y + TILESIZE};
	
	_speed = SPEED;
	_jumpPower = JUMPPOWER_HORIZON;
	_isMove = false;
	_isReverse = false;
	_isArrive = true;

	_direction = { 0,0 };
	_effCount = 0;
	_atkCombo = 1;
	_bloodKill = 0;
	_effectAngle = 0;
	_grooveChain = 1;
	_killCombo = 0;

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

	if (_bloodKill >= 10)
	{
		_curHp += 1;
		_bloodKill = 0;
	}

	if (_isShowEff)
	{
		_effCount++;

		if (_effCount >= 2)
		{
			_effCount = 0;
			int tmp = _effectImg->GetFrameX();
			int tmp2 = _effectImg->GetMaxFrameX();
			if (_effectImg->GetMaxFrameX() == _effectImg->GetFrameX())
			{
				_effectImg->SetFrameX(0);
				_isShowEff = false;
			}
			else
			{
				_effectImg->SetFrameX(_effectImg->GetFrameX() + 1);
			}
		}
	}

	if (_isShowShovel)
	{
		_shovelCount++;

		if (_shovelCount > 10)
		{
			_isShowShovel = false;
			_shovelCount = 0;
		}
	}

	if (_inventory[INVEN_HP].isGet && KEYMANAGER->isOnceKeyDown('E'))
	{
		useHpIten();
	}

	switch (_killCombo)
	{
	case 0:
		_grooveChain = 1;
		_killCombo++;
		break;
	case 2:
		_grooveChain = 2;
		_killCombo++;		// 스위치문 적용 한번만 하기위해 킬콤보 ++함
		SOUNDMANAGER->playEff("grooveChainStart");
		break;
	case 6:
		_grooveChain = 3;
		_killCombo++;
		SOUNDMANAGER->playEff("grooveChainStart");
		break;
	case 13:
		_grooveChain = 4;
		_killCombo++;
		SOUNDMANAGER->playEff("grooveChainStart");
		break;
	}

	if (_isBeat)
	{
		for (int i = 0; i < INVEN_END; i++)
		{
			if (_inventory[i].frameX == 8 && _inventory[i].frameY != 10)
			{
				switch (_inventory[i].frameY)
				{
				case 0:
					_dmg = _grooveChain;
					break;
				case 2:
					_dmg = _grooveChain;
					break;
				case 4:
					_torchPower = 2 + _grooveChain;
					break;
				case 6:
					_shovelDmg = _grooveChain;
					break;
				case 8:
					_def = _grooveChain;
					break;
				}
			}
		}
	}
}

void player::render()
{
	
	

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

//===========================================
//					init
//===========================================

void player::imgInit()
{
	IMAGEMANAGER->addFrameImage("playerHead", L"images/player/head.png", 192, 96, 4, 2);
	IMAGEMANAGER->addFrameImage("playerBody", L"images/player/body.png", 192, 480, 4, 10);
	IMAGEMANAGER->addImage("playerShadow", L"images/player/shadow.png", 48, 54);
	//아이템 & 슬롯
	IMAGEMANAGER->addFrameImage("Item", L"images/item/item.png", 432, 576, 9, 12);
	IMAGEMANAGER->addImage("slot_Shovel", L"images/item/slot_Shovel.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Weapon", L"images/item/slot_Weapon.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Armor", L"images/item/slot_Armor.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Torch", L"images/item/slot_Torch.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Hp", L"images/item/slot_Hp.png", 60, 84);

	//이펙트 이미지
	IMAGEMANAGER->addFrameImage("effect_Dagger", L"images/effect/swipe_dagger.png", 144, 48,3,1);
	IMAGEMANAGER->addFrameImage("effect_Spear", L"images/effect/swipe_rapier.png", 384, 48,4,1);
	IMAGEMANAGER->addFrameImage("effect_Sword", L"images/effect/swipe_broadsword.png", 144, 144, 3, 1);
	
	_effectImg = IMAGEMANAGER->findImage("effect_Dagger");
	_shovelImg = IMAGEMANAGER->findImage("Item");

	int head[] = { 0,1,2,3,4,5,6,7 };
	KEYANIMANAGER->addAnimationType("playerHead");
	KEYANIMANAGER->addArrayFrameAnimation("playerHead", "head", "playerHead", head, 8, 8, true);

	int normalBody[] = { 0,1,2,3 };
	KEYANIMANAGER->addAnimationType("playerBody");
	KEYANIMANAGER->addArrayFrameAnimation("playerBody", "normalBody", "playerBody", normalBody, 4, 8, true);

	int body1[] = { 4,5,6,7 };
	KEYANIMANAGER->addAnimationType("playerBody");
	KEYANIMANAGER->addArrayFrameAnimation("playerBody", "body1", "playerBody", body1, 4, 8, true);

	int body2[] = { 8,9,10,11 };
	KEYANIMANAGER->addAnimationType("playerBody");
	KEYANIMANAGER->addArrayFrameAnimation("playerBody", "body2", "playerBody", body2, 4, 8, true);

	int body3[] = { 12,13,14,15 };
	KEYANIMANAGER->addAnimationType("playerBody");
	KEYANIMANAGER->addArrayFrameAnimation("playerBody", "body3", "playerBody", body3, 4, 8, true);

	int bodyGlass[] = { 36,37,38,39 };
	KEYANIMANAGER->addAnimationType("playerBody");
	KEYANIMANAGER->addArrayFrameAnimation("playerBody", "bodyGlass", "playerBody", bodyGlass, 4, 8, true);

	int bodyObsidian[] = { 24,25,26,27 };
	KEYANIMANAGER->addAnimationType("playerBody");
	KEYANIMANAGER->addArrayFrameAnimation("playerBody", "bodyObsidian", "playerBody", bodyObsidian, 4, 8, true);

	_headImg = IMAGEMANAGER->findImage("playerHead");
	_headAni = KEYANIMANAGER->findAnimation("playerHead", "head");
	_headAni->start();

	_bodyImg = IMAGEMANAGER->findImage("playerBody");
	_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "normalBody");
	_bodyAni->start();

	_shadowImg = IMAGEMANAGER->findImage("playerShadow");

	//EFFECTMANAGER->addEffect("shovel_basic", "images/item/shovel/shovel_basic.png", 48, 48, 48, 48, 1, 0.1, 50);

	//방어구 입었을때 이미지들도 나중에 비교해서 만들어놓자. 일단은 그냥 기본몸만.
}

void player::inventoryInit()
{
	_inventory[INVEN_WEAPON].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_WEAPON].slotImg = IMAGEMANAGER->findImage("slot_Weapon");
	_inventory[INVEN_WEAPON].frameX = 8;
	_inventory[INVEN_WEAPON].frameY = 10;
	_inventory[INVEN_WEAPON].isGet = false;
	_inventory[INVEN_WEAPON].isGlass = false;
	_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
	_inventory[INVEN_WEAPON].itemVal = ITEM_DAGGER;
	_dmg = 1;

	_inventory[INVEN_TORCH].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_TORCH].slotImg = IMAGEMANAGER->findImage("slot_Torch");
	_inventory[INVEN_TORCH].frameX = NULL;
	_inventory[INVEN_TORCH].frameY = NULL;
	_inventory[INVEN_TORCH].isGet = false;
	_inventory[INVEN_TORCH].isGlass = false;
	_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
	_inventory[INVEN_TORCH].itemVal = NULL;
	_torchPower = 2;

	_inventory[INVEN_SHOVEL].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_SHOVEL].slotImg = IMAGEMANAGER->findImage("slot_Shovel");
	_inventory[INVEN_SHOVEL].frameX = 0;
	_inventory[INVEN_SHOVEL].frameY = 6;
	_inventory[INVEN_SHOVEL].isGet = false;
	_inventory[INVEN_SHOVEL].isGlass = false;
	_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
	_inventory[INVEN_SHOVEL].itemVal = ITEM_SHOVEL_NORMAL;
	_shovelDmg = 1;
	_shovelFrameX = _inventory[INVEN_SHOVEL].frameX;
	_shovelFrameY = _inventory[INVEN_SHOVEL].frameY;
	_shovelCount = 0;

	_inventory[INVEN_ARMOR].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_ARMOR].slotImg = IMAGEMANAGER->findImage("slot_Armor");
	_inventory[INVEN_ARMOR].frameX = NULL;
	_inventory[INVEN_ARMOR].frameY = NULL;
	_inventory[INVEN_ARMOR].isGet = false;
	_inventory[INVEN_ARMOR].isGlass = false;
	_inventory[INVEN_ARMOR].itemKind = INVEN_ARMOR;
	_inventory[INVEN_ARMOR].itemVal = NULL;
	_def = 0;

	_inventory[INVEN_HP].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_HP].slotImg = IMAGEMANAGER->findImage("slot_Hp");
	_inventory[INVEN_HP].frameX = NULL;
	_inventory[INVEN_HP].frameY = NULL;
	_inventory[INVEN_HP].isGet = false;
	_inventory[INVEN_HP].isGlass = false;
	_inventory[INVEN_HP].itemKind = INVEN_HP;
	_inventory[INVEN_HP].itemVal = NULL;
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
			setWeaponVDirection(_inventory[INVEN_WEAPON].itemVal, _direction);
			_isMove = true;


			for (int i = 0; i < _vDirection.size(); i++)
			{
				//못가는 곳이고 벽이면
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false &&
					((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE ||
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= ETC_TORCH_BOSS) ||
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
				{
					//벽없애고 포문탈출
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
					{
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->openChest();
						_isMove = false;
						SOUNDMANAGER->playEff("chest");
						break;
					}
					else
					{
						objFunc(_direction);
						_isMove = false;
						break;
					}
				}
				//그냥 못가는곳이면(적이면)
				else if ((*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getIsAvailMove() == false &&
					(*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getAttribute() == OBJ_NONE)
				{
					//적만큼 돌면서 내가 공격한곳 인덱스랑 같은곳에 위치한놈 찾아내고 그놈 피 깎음
					attackFunc(_vDirection[i]);
					_isShowEff = true;
					_effectAngle = 180;
					_isMove = false;
				}
			}
			_isBeat = false;
			_vDirection.clear();
			if (_isMove)
			{
				_savePos = _posCT;
				_vec.x -= _speed;
				_isArrive = false;
				_isMove = true;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
		}
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && !_isMove)
	{
		_isPressKey = true;
		if (_isBeat)
		{
			_direction = { 1, 0 };
			_isReverse = false;
			setWeaponVDirection(_inventory[INVEN_WEAPON].itemVal, _direction);
			_isMove = true;

			for (int i = 0; i < _vDirection.size(); i++)
			{
				//못가는 곳이고 벽이면
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false &&
					((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE ||
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= ETC_TORCH_BOSS) ||
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
				{
					//벽없애고 포문탈출
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
					{
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->openChest();
						_isMove = false;
						SOUNDMANAGER->playEff("chest");
						break;
					}
					else
					{
						objFunc(_direction);
						_isMove = false;
						break;
					}
				}
				//그냥 못가는곳이면(적이면)
				else if ((*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getIsAvailMove() == false &&
					(*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getAttribute() == OBJ_NONE)
				{
					//적만큼 돌면서 내가 공격한곳 인덱스랑 같은곳에 위치한놈 찾아내고 그놈 피 깎음
					attackFunc(_vDirection[i]);
					_isMove = false;
					_effectAngle = 0;
					_isShowEff = true;
				}
			}
			_isBeat = false;
			_vDirection.clear();
			if (_isMove)
			{
				_savePos = _posCT;
				_vec.x += _speed;
				_isArrive = false;
				_jumpPower = JUMPPOWER_HORIZON;
				_posZ = 0;
			}
		}
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_UP) && !_isMove)
	{
		_isPressKey = true;
		if (_isBeat)
		{
			_direction = { 0, -1 };
			setWeaponVDirection(_inventory[INVEN_WEAPON].itemVal, _direction);
			_isMove = true;


			for (int i = 0; i < _vDirection.size(); i++)
			{
				//못가는 곳이고 벽이면
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false &&
					((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE ||
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= ETC_TORCH_BOSS) ||
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
				{
					//벽없애고 포문탈출
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
					{
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->openChest();
						_isMove = false;
						SOUNDMANAGER->playEff("chest");
						break;
					}
					else
					{
						objFunc(_direction);
						_isMove = false;
						break;
					}
				}
				//그냥 못가는곳이면(적이면)
				else if ((*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getIsAvailMove() == false &&
					(*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getAttribute() == OBJ_NONE)
				{
					//적만큼 돌면서 내가 공격한곳 인덱스랑 같은곳에 위치한놈 찾아내고 그놈 피 깎음
					attackFunc(_vDirection[i]);
					_isMove = false;
					_isShowEff = true;
					_effectAngle = 270;
				}
			}
			_isBeat = false;
			_vDirection.clear();
			if (_isMove)
			{
				_savePos = _posCT;
				_vec.y -= _speed;
				_isArrive = false;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
		}
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && !_isMove)
	{
		_isPressKey = true;

		if (_isBeat)
		{
			_direction = { 0, 1 };
			setWeaponVDirection(_inventory[INVEN_WEAPON].itemVal, _direction);
			_isMove = true;
		
			for (int i = 0; i < _vDirection.size(); i++)
			{
				//못가는 곳이고 벽이면
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false &&
					((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= OBJ_DOOR_SIDE ||
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() <= ETC_TORCH_BOSS) ||
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
				{
					//벽없애고 포문탈출
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_CHEST)
					{
						(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->openChest();
						_isMove = false;
						SOUNDMANAGER->playEff("chest");
						break;
					}
					else
					{
						objFunc(_direction);
						_isMove = false;
						break;
					}
				}
				//그냥 못가는곳이면(적이면)
				else if ((*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getIsAvailMove() == false &&
					(*_vvObj)[_idx.y + _vDirection[i].y][_idx.x + _vDirection[i].x]->getAttribute() == OBJ_NONE)
				{
					//적만큼 돌면서 내가 공격한곳 인덱스랑 같은곳에 위치한놈 찾아내고 그놈 피 깎음
					attackFunc(_vDirection[i]);
					_isMove = false;
					_isShowEff = true;
					_effectAngle = 90;
				}
			}
			_isBeat = false;
			_vDirection.clear();
			if (_isMove)
			{
				_savePos = _posCT;
				_vec.y += _speed;
				_isArrive = false;
				_jumpPower = JUMPPOWER_VERTICAL;
				_posZ = 0;
			}
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
		_atkCombo = 1;

		//
		_posCT.x += _vec.x;
		_posCT.y += _vec.y;
		_posLT.x = _posCT.x - TILESIZE / 2;
		_posLT.y = _posCT.y - TILESIZE / 2;
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() > ITEM_START && (*_vvObj)[_idx.y][_idx.x]->getAttribute() < ITEM_END)
		{
			setInventoryItem((*_vvObj)[_idx.y][_idx.x]->getAttribute());
			(*_vvObj)[_idx.y][_idx.x]->objSetDefault();
		}
		if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == ETC_SLOW && _isSlow == false && _isFast == false)
		{
			_isSlow = true;
			SOUNDMANAGER->playEff("trap");
			SOUNDMANAGER->setPitch(0.9f);
			_trapIdx = { _idx.x, _idx.y };
			(*_vvObj)[_trapIdx.y][_trapIdx.x]->setFrameX((*_vvObj)[_idx.y][_idx.x]->getFrameX() + 1);
		}
		else if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == ETC_FAST && _isSlow == false && _isFast == false)
		{
			_isFast = true;
			SOUNDMANAGER->playEff("trap");
			SOUNDMANAGER->setPitch(1.1f);
			_trapIdx = { _idx.x, _idx.y };
			(*_vvObj)[_trapIdx.y][_trapIdx.x]->setFrameX((*_vvObj)[_idx.y][_idx.x]->getFrameX() + 1);
		}
		else if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == ETC_TRAPDOOR)
		{
			SOUNDMANAGER->playEff("trap");
			SOUNDMANAGER->playEff("trapdoor");
		}
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
		_atkCombo = 1;

		_posCT.x += _vec.x;
		_posCT.y += _vec.y;
		_posLT.x = _posCT.x - TILESIZE / 2;
		_posLT.y = _posCT.y - TILESIZE / 2;
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() > ITEM_START && (*_vvObj)[_idx.y][_idx.x]->getAttribute() < ITEM_END)
		{
			setInventoryItem((*_vvObj)[_idx.y][_idx.x]->getAttribute());
			(*_vvObj)[_idx.y][_idx.x]->objSetDefault();
		}
		if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == ETC_SLOW && _isSlow == false && _isFast == false)
		{
			_isSlow = true;
			SOUNDMANAGER->playEff("trap");
			SOUNDMANAGER->setPitch(0.9f);
			_trapIdx = { _idx.x, _idx.y };
			(*_vvObj)[_trapIdx.y][_trapIdx.x]->setFrameX((*_vvObj)[_idx.y][_idx.x]->getFrameX() + 1);
		}
		else if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == ETC_FAST && _isSlow == false && _isFast == false)
		{
			_isFast = true;
			SOUNDMANAGER->playEff("trap");
			SOUNDMANAGER->setPitch(1.1f);
			_trapIdx = { _idx.x, _idx.y };
			(*_vvObj)[_trapIdx.y][_trapIdx.x]->setFrameX((*_vvObj)[_idx.y][_idx.x]->getFrameX() + 1);
		}
		else if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() == ETC_TRAPDOOR)
		{
			SOUNDMANAGER->playEff("trap");
			SOUNDMANAGER->playEff("trapdoor");
		}
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
	int rnd = RND->getFromIntTo(1, 7);

	switch ((*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->getAttribute())
	{
	case OBJ_WALL1: case ETC_TORCH_WALL1:
		_isShowShovel = true;
		if (_shovelDmg > 0)
		{
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			
			switch (rnd)
			{
			case 1:
				SOUNDMANAGER->playEff("dig1");
				break;
			case 2:
				SOUNDMANAGER->playEff("dig2");
				break;
			case 3:
				SOUNDMANAGER->playEff("dig3");
				break;
			case 4:
				SOUNDMANAGER->playEff("dig4");
				break;
			case 5:
				SOUNDMANAGER->playEff("dig5");
				break;
			case 6:
				SOUNDMANAGER->playEff("dig6");
				break;
			}
			SOUNDMANAGER->playEff("wall1_Dig");
		}
		else
		{
			SOUNDMANAGER->playEff("dig_Fail");
			_killCombo = 0;
			if (_grooveChain > 1)
			{
				SOUNDMANAGER->playEff("grooveChainFail");
			}
		}
		break;
	case OBJ_WALL2: case ETC_TORCH_WALL2:
		_isShowShovel = true;
		if (_shovelDmg > 1)
		{
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			switch (rnd)
			{
			case 1:
				SOUNDMANAGER->playEff("dig1");
				break;
			case 2:
				SOUNDMANAGER->playEff("dig2");
				break;
			case 3:
				SOUNDMANAGER->playEff("dig3");
				break;
			case 4:
				SOUNDMANAGER->playEff("dig4");
				break;
			case 5:
				SOUNDMANAGER->playEff("dig5");
				break;
			case 6:
				SOUNDMANAGER->playEff("dig6");
				break;
			}
			SOUNDMANAGER->playEff("wall2_Dig");
		}
		else if (_inventory[INVEN_SHOVEL].itemVal == ITEM_SHOVEL_BLOOD)
		{
			_curHp -= 1;
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			switch (rnd)
			{
			case 1:
				SOUNDMANAGER->playEff("dig1");
				SOUNDMANAGER->playEff("hit1");
				break;
			case 2:
				SOUNDMANAGER->playEff("dig2");
				SOUNDMANAGER->playEff("hit2");
				break;
			case 3:
				SOUNDMANAGER->playEff("dig3");
				SOUNDMANAGER->playEff("hit3");
				break;
			case 4:
				SOUNDMANAGER->playEff("dig4");
				SOUNDMANAGER->playEff("hit4");
				break;
			case 5:
				SOUNDMANAGER->playEff("dig5");
				SOUNDMANAGER->playEff("hit5");
				break;
			case 6:
				SOUNDMANAGER->playEff("dig6");
				SOUNDMANAGER->playEff("hit6");
				break;
			}
			SOUNDMANAGER->playEff("wall2_Dig");
			SOUNDMANAGER->playEff("hit");
		}
		else
		{
			SOUNDMANAGER->playEff("dig_Fail");
			_killCombo = 0;
			if (_grooveChain > 1)
			{
				SOUNDMANAGER->playEff("grooveChainFail");
			}
		}
		break;
	case OBJ_WALL3: case ETC_TORCH_WALL3:
		_isShowShovel = true;
		if (_shovelDmg > 2)
		{
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			switch (rnd)
			{
			case 1:
				SOUNDMANAGER->playEff("dig1");
				break;
			case 2:
				SOUNDMANAGER->playEff("dig2");
				break;
			case 3:
				SOUNDMANAGER->playEff("dig3");
				break;
			case 4:
				SOUNDMANAGER->playEff("dig4");
				break;
			case 5:
				SOUNDMANAGER->playEff("dig5");
				break;
			case 6:
				SOUNDMANAGER->playEff("dig6");
				break;
			}
			SOUNDMANAGER->playEff("wall3_Dig");
		}
		else if (_inventory[INVEN_SHOVEL].itemVal == ITEM_SHOVEL_BLOOD)
		{
			_curHp -= 1;
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			_curHp -= 1;
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			switch (rnd)
			{
			case 1:
				SOUNDMANAGER->playEff("dig1");
				SOUNDMANAGER->playEff("hit1");
				break;
			case 2:
				SOUNDMANAGER->playEff("dig2");
				SOUNDMANAGER->playEff("hit2");
				break;
			case 3:
				SOUNDMANAGER->playEff("dig3");
				SOUNDMANAGER->playEff("hit3");
				break;
			case 4:
				SOUNDMANAGER->playEff("dig4");
				SOUNDMANAGER->playEff("hit4");
				break;
			case 5:
				SOUNDMANAGER->playEff("dig5");
				SOUNDMANAGER->playEff("hit5");
				break;
			case 6:
				SOUNDMANAGER->playEff("dig6");
				SOUNDMANAGER->playEff("hit6");
				break;
			}
			SOUNDMANAGER->playEff("wall2_Dig");
			SOUNDMANAGER->playEff("hit");
		}
		else
		{
			SOUNDMANAGER->playEff("dig_Fail");
			_killCombo = 0;
			if (_grooveChain > 1)
			{
				SOUNDMANAGER->playEff("grooveChainFail");
			}
		}
		break;
	case OBJ_WALL_GOLD: case ETC_TORCH_GOLD:
		_isShowShovel = true;
		if (_inventory[INVEN_SHOVEL].itemVal == ITEM_SHOVEL_BLOOD)
		{
			_curHp -= 1;
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			_curHp -= 1;
			(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
			switch (rnd)
			{
			case 1:
				SOUNDMANAGER->playEff("dig1");
				SOUNDMANAGER->playEff("hit1");
				break;
			case 2:
				SOUNDMANAGER->playEff("dig2");
				SOUNDMANAGER->playEff("hit2");
				break;
			case 3:
				SOUNDMANAGER->playEff("dig3");
				SOUNDMANAGER->playEff("hit3");
				break;
			case 4:
				SOUNDMANAGER->playEff("dig4");
				SOUNDMANAGER->playEff("hit4");
				break;
			case 5:
				SOUNDMANAGER->playEff("dig5");
				SOUNDMANAGER->playEff("hit5");
				break;
			case 6:
				SOUNDMANAGER->playEff("dig6");
				SOUNDMANAGER->playEff("hit6");
				break;
			}
			SOUNDMANAGER->playEff("wall2_Dig");
			SOUNDMANAGER->playEff("hit");
		}
		else
		{
			SOUNDMANAGER->playEff("dig_Fail");
			_killCombo = 0;
			if (_grooveChain > 1)
			{
				SOUNDMANAGER->playEff("grooveChainFail");
			}
		}
		break;
	case OBJ_WALL_BOSS: case ETC_TORCH_BOSS:
		_isShowShovel = true;
		SOUNDMANAGER->playEff("dig_Fail");
		_killCombo = 0;
		if (_grooveChain > 1)
		{
			SOUNDMANAGER->playEff("grooveChainFail");
		}
		break;
	case OBJ_WALL_END:
		_isShowShovel = true;
		SOUNDMANAGER->playEff("dig_Fail");
		_killCombo = 0;
		if (_grooveChain > 1)
		{
			SOUNDMANAGER->playEff("grooveChainFail");
		}
		break;
	case OBJ_DOOR_FRONT:
		(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		SOUNDMANAGER->playEff("doorOpen");
		_killCombo = 0;
		if (_grooveChain > 1)
		{
			SOUNDMANAGER->playEff("grooveChainFail");
		}
		break;
	case OBJ_DOOR_SIDE:
		(*_vvObj)[_idx.y + direction.y][_idx.x + direction.x]->objSetDefault();
		SOUNDMANAGER->playEff("doorOpen");
		_killCombo = 0;
		if (_grooveChain > 1)
		{
			SOUNDMANAGER->playEff("grooveChainFail");
		}
		break;
	}

	_atkCombo = 1;
}

void player::setInventoryItem(int objAttr)
{
	switch (objAttr)
	{
	case ITEM_SPEAR_NORMAL:
		_inventory[INVEN_WEAPON].frameX = 0;
		_inventory[INVEN_WEAPON].frameY = 0;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		_effectImg = IMAGEMANAGER->findImage("effect_Spear");
		SOUNDMANAGER->playEff("pickup_Weapon");
		//공격 이펙트 이미지 넣을것.
		break;
	case ITEM_SPEAR_BLOOD:
		_inventory[INVEN_WEAPON].frameX = 2;
		_inventory[INVEN_WEAPON].frameY = 0;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		_effectImg = IMAGEMANAGER->findImage("effect_Spear");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SPEAR_TITANIUM:
		_inventory[INVEN_WEAPON].frameX = 4;
		_inventory[INVEN_WEAPON].frameY = 0;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 2;
		_effectImg = IMAGEMANAGER->findImage("effect_Spear");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SPEAR_GLASS:
		_inventory[INVEN_WEAPON].frameX = 6;
		_inventory[INVEN_WEAPON].frameY = 0;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = true;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 4;
		_effectImg = IMAGEMANAGER->findImage("effect_Spear");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SPEAR_OBSIDIAN:
		_inventory[INVEN_WEAPON].frameX = 8;
		_inventory[INVEN_WEAPON].frameY = 0;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		_effectImg = IMAGEMANAGER->findImage("effect_Spear");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SWORD_NORMAL:
		_inventory[INVEN_WEAPON].frameX = 0;
		_inventory[INVEN_WEAPON].frameY = 2;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		_effectImg = IMAGEMANAGER->findImage("effect_Sword");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SWORD_BLOOD:
		_inventory[INVEN_WEAPON].frameX = 2;
		_inventory[INVEN_WEAPON].frameY = 2;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		_effectImg = IMAGEMANAGER->findImage("effect_Sword");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SWORD_TITANIUM:
		_inventory[INVEN_WEAPON].frameX = 4;
		_inventory[INVEN_WEAPON].frameY = 2;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 2;
		_effectImg = IMAGEMANAGER->findImage("effect_Sword");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SWORD_GLASS:
		_inventory[INVEN_WEAPON].frameX = 6;
		_inventory[INVEN_WEAPON].frameY = 2;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = true;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 4;
		_effectImg = IMAGEMANAGER->findImage("effect_Sword");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_SWORD_OBSIDIAN:
		_inventory[INVEN_WEAPON].frameX = 8;
		_inventory[INVEN_WEAPON].frameY = 2;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		_effectImg = IMAGEMANAGER->findImage("effect_Sword");
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	case ITEM_GLASS_WEAPON:
		_inventory[INVEN_WEAPON].frameX = 6;
		_inventory[INVEN_WEAPON].frameY = 10;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		_effectImg = IMAGEMANAGER->findImage("effect_Dagger");
		break;
	case ITEM_TORCH_1:
		_inventory[INVEN_TORCH].frameX = 0;
		_inventory[INVEN_TORCH].frameY = 4;
		_inventory[INVEN_TORCH].isGet = true;
		_inventory[INVEN_TORCH].isGlass = false;
		_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
		_inventory[INVEN_TORCH].itemVal = objAttr;
		_torchPower = 3;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_TORCH_2:
		_inventory[INVEN_TORCH].frameX = 2;
		_inventory[INVEN_TORCH].frameY = 4;
		_inventory[INVEN_TORCH].isGet = true;
		_inventory[INVEN_TORCH].isGlass = false;
		_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
		_inventory[INVEN_TORCH].itemVal = objAttr;
		_torchPower = 4;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_TORCH_3:
		_inventory[INVEN_TORCH].frameX = 4;
		_inventory[INVEN_TORCH].frameY = 4;
		_inventory[INVEN_TORCH].isGet = true;
		_inventory[INVEN_TORCH].isGlass = false;
		_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
		_inventory[INVEN_TORCH].itemVal = objAttr;
		_torchPower = 5;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_TORCH_GLASS:
		_inventory[INVEN_TORCH].frameX = 6;
		_inventory[INVEN_TORCH].frameY = 4;
		_inventory[INVEN_TORCH].isGet = true;
		_inventory[INVEN_TORCH].isGlass = true;
		_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
		_inventory[INVEN_TORCH].itemVal = objAttr;
		_torchPower = 6;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_TORCH_OBSIDIAN:
		_inventory[INVEN_TORCH].frameX = 8;
		_inventory[INVEN_TORCH].frameY = 4;
		_inventory[INVEN_TORCH].isGet = true;
		_inventory[INVEN_TORCH].isGlass = false;
		_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
		_inventory[INVEN_TORCH].itemVal = objAttr;
		_torchPower = 3;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_GLASS_TORCH:
		_inventory[INVEN_TORCH].frameX = 6;
		_inventory[INVEN_TORCH].frameY = 10;
		_inventory[INVEN_TORCH].isGet = true;
		_inventory[INVEN_TORCH].isGlass = false;
		_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
		_inventory[INVEN_TORCH].itemVal = objAttr;
		_torchPower = 2;
		break;
	case ITEM_SHOVEL_NORMAL:
		_inventory[INVEN_SHOVEL].frameX = 0;
		_inventory[INVEN_SHOVEL].frameY = 6;
		_inventory[INVEN_SHOVEL].isGet = true;
		_inventory[INVEN_SHOVEL].isGlass = false;
		_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
		_inventory[INVEN_SHOVEL].itemVal = objAttr;
		_shovelDmg = 1;
		_shovelFrameX = _inventory[INVEN_SHOVEL].frameX;
		_shovelFrameY = _inventory[INVEN_SHOVEL].frameY;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_SHOVEL_BLOOD:
		_inventory[INVEN_SHOVEL].frameX = 2;
		_inventory[INVEN_SHOVEL].frameY = 6;
		_inventory[INVEN_SHOVEL].isGet = true;
		_inventory[INVEN_SHOVEL].isGlass = false;
		_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
		_inventory[INVEN_SHOVEL].itemVal = objAttr;
		_shovelDmg = 1;
		_shovelFrameX = _inventory[INVEN_SHOVEL].frameX;
		_shovelFrameY = _inventory[INVEN_SHOVEL].frameY;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_SHOVEL_TITANIUM:
		_inventory[INVEN_SHOVEL].frameX = 4;
		_inventory[INVEN_SHOVEL].frameY = 6;
		_inventory[INVEN_SHOVEL].isGet = true;
		_inventory[INVEN_SHOVEL].isGlass = false;
		_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
		_inventory[INVEN_SHOVEL].itemVal = objAttr;
		_shovelDmg =2;
		_shovelFrameX = _inventory[INVEN_SHOVEL].frameX;
		_shovelFrameY = _inventory[INVEN_SHOVEL].frameY;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_SHOVEL_GLASS:
		_inventory[INVEN_SHOVEL].frameX = 6;
		_inventory[INVEN_SHOVEL].frameY = 6;
		_inventory[INVEN_SHOVEL].isGet = true;
		_inventory[INVEN_SHOVEL].isGlass = true;
		_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
		_inventory[INVEN_SHOVEL].itemVal = objAttr;
		_shovelDmg = 3;
		_shovelFrameX = _inventory[INVEN_SHOVEL].frameX;
		_shovelFrameY = _inventory[INVEN_SHOVEL].frameY;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_SHOVEL_OBSIDIAN:
		_inventory[INVEN_SHOVEL].frameX = 8;
		_inventory[INVEN_SHOVEL].frameY = 6;
		_inventory[INVEN_SHOVEL].isGet = true;
		_inventory[INVEN_SHOVEL].isGlass = false;
		_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
		_inventory[INVEN_SHOVEL].itemVal = objAttr;
		_shovelDmg = 1;
		_shovelFrameX = _inventory[INVEN_SHOVEL].frameX;
		_shovelFrameY = _inventory[INVEN_SHOVEL].frameY;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_GLASS_SHOVEL:
		_inventory[INVEN_SHOVEL].frameX = 6;
		_inventory[INVEN_SHOVEL].frameY = 10;
		_inventory[INVEN_SHOVEL].isGet = true;
		_inventory[INVEN_SHOVEL].isGlass = false;
		_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
		_inventory[INVEN_SHOVEL].itemVal = objAttr;
		_shovelDmg = 1;
		_shovelFrameX = _inventory[INVEN_SHOVEL].frameX;
		_shovelFrameY = _inventory[INVEN_SHOVEL].frameY;
		break;
	case ITEM_ARMOR_1:
		_inventory[INVEN_ARMOR].frameX = 0;
		_inventory[INVEN_ARMOR].frameY = 8;
		_inventory[INVEN_ARMOR].isGet = true;
		_inventory[INVEN_ARMOR].isGlass = false;
		_inventory[INVEN_ARMOR].itemKind = INVEN_ARMOR;
		_inventory[INVEN_ARMOR].itemVal = objAttr;
		_def = 1;
		_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "body1");
		_bodyImg->SetFrameX(0);
		_bodyAni->start();
		_headImg->SetFrameX(0);
		_headAni->start();
		SOUNDMANAGER->playEff("pickup_Armor");
		break;
	case ITEM_ARMOR_2:
		_inventory[INVEN_ARMOR].frameX = 2;
		_inventory[INVEN_ARMOR].frameY = 8;
		_inventory[INVEN_ARMOR].isGet = true;
		_inventory[INVEN_ARMOR].isGlass = false;
		_inventory[INVEN_ARMOR].itemKind = INVEN_ARMOR;
		_inventory[INVEN_ARMOR].itemVal = objAttr;
		_def = 2;
		_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "body2");
		_bodyImg->SetFrameX(0);
		_bodyAni->start();
		_headImg->SetFrameX(0);
		_headAni->start();
		SOUNDMANAGER->playEff("pickup_Armor");
		break;
	case ITEM_ARMOR_3:
		_inventory[INVEN_ARMOR].frameX = 4;
		_inventory[INVEN_ARMOR].frameY = 8;
		_inventory[INVEN_ARMOR].isGet = true;
		_inventory[INVEN_ARMOR].isGlass = false;
		_inventory[INVEN_ARMOR].itemKind = INVEN_ARMOR;
		_inventory[INVEN_ARMOR].itemVal = objAttr;
		_def = 3;
		_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "body3");
		_bodyImg->SetFrameX(0);
		_bodyAni->start();
		_headImg->SetFrameX(0);
		_headAni->start();
		SOUNDMANAGER->playEff("pickup_Armor");
		break;
	case ITEM_ARMOR_GLASS:
		_inventory[INVEN_ARMOR].frameX = 6;
		_inventory[INVEN_ARMOR].frameY = 8;
		_inventory[INVEN_ARMOR].isGet = true;
		_inventory[INVEN_ARMOR].isGlass = true;
		_inventory[INVEN_ARMOR].itemKind = INVEN_ARMOR;
		_inventory[INVEN_ARMOR].itemVal = objAttr;
		_def = 999;
		_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "bodyGlass");
		_bodyImg->SetFrameX(0);
		_bodyAni->start();
		_headImg->SetFrameX(0);
		_headAni->start();
		SOUNDMANAGER->playEff("pickup_Armor");
		break;
	case ITEM_ARMOR_OBSIDIAN:
		_inventory[INVEN_ARMOR].frameX = 8;
		_inventory[INVEN_ARMOR].frameY = 8;
		_inventory[INVEN_ARMOR].isGet = true;
		_inventory[INVEN_ARMOR].isGlass = false;
		_inventory[INVEN_ARMOR].itemKind = INVEN_ARMOR;
		_inventory[INVEN_ARMOR].itemVal = objAttr;
		_def = 1;
		_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "bodyObsidian");
		_bodyImg->SetFrameX(0);
		_bodyAni->start();
		_headImg->SetFrameX(0);
		_headAni->start();
		SOUNDMANAGER->playEff("pickup_Armor");
		break;
	case ITEM_HP_APPLE:
		_inventory[INVEN_HP].frameX = 0;
		_inventory[INVEN_HP].frameY = 10;
		_inventory[INVEN_HP].isGet = true;
		_inventory[INVEN_HP].isGlass = false;
		_inventory[INVEN_HP].itemKind = INVEN_HP;
		_inventory[INVEN_HP].itemVal = objAttr;
		_recoverHp = 2;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_HP_CHEESE:
		_inventory[INVEN_HP].frameX = 2;
		_inventory[INVEN_HP].frameY = 10;
		_inventory[INVEN_HP].isGet = true;
		_inventory[INVEN_HP].isGlass = false;
		_inventory[INVEN_HP].itemKind = INVEN_HP;
		_inventory[INVEN_HP].itemVal = objAttr;
		_recoverHp = 4;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_HP_MEAT:
		_inventory[INVEN_HP].frameX = 4;
		_inventory[INVEN_HP].frameY = 10;
		_inventory[INVEN_HP].isGet = true;
		_inventory[INVEN_HP].isGlass = false;
		_inventory[INVEN_HP].itemKind = INVEN_HP;
		_inventory[INVEN_HP].itemVal = objAttr;
		_recoverHp = 6;
		SOUNDMANAGER->playEff("pickup");
		break;
	case ITEM_DAGGER:
		_inventory[INVEN_WEAPON].frameX = 8;
		_inventory[INVEN_WEAPON].frameY = 10;
		_inventory[INVEN_WEAPON].isGet = true;
		_inventory[INVEN_WEAPON].isGlass = false;
		_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
		_inventory[INVEN_WEAPON].itemVal = objAttr;
		_dmg = 1;
		SOUNDMANAGER->playEff("pickup_Weapon");
		break;
	}
}

void player::attackFunc(POINT direction)
{
	int rnd = RND->getFromIntTo(1, 5);

	if (_atkCombo > 4)
	{
		_atkCombo = 1;
	}
	switch (_atkCombo)
	{
	case 1:
		switch (rnd)
		{
		case 1:
			SOUNDMANAGER->playEff("atk1_1");
			break;
		case 2:
			SOUNDMANAGER->playEff("atk1_2");
			break;
		case 3:
			SOUNDMANAGER->playEff("atk1_3");
			break;
		case 4:
			SOUNDMANAGER->playEff("atk1_4");
			break;
		}
		break;
	case 2:
		switch (rnd)
		{
		case 1:
			SOUNDMANAGER->playEff("atk2_1");
			break;
		case 2:
			SOUNDMANAGER->playEff("atk2_2");
			break;
		case 3:
			SOUNDMANAGER->playEff("atk2_3");
			break;
		case 4:
			SOUNDMANAGER->playEff("atk2_4");
			break;
		}
		break;
	case 3:
		switch (rnd)
		{
		case 1:
			SOUNDMANAGER->playEff("atk3_1");
			break;
		case 2:
			SOUNDMANAGER->playEff("atk3_2");
			break;
		case 3:
			SOUNDMANAGER->playEff("atk3_3");
			break;
		case 4:
			SOUNDMANAGER->playEff("atk3_4");
			break;
		}
		break;
	case 4:
		switch (rnd)
		{
		case 1:
			SOUNDMANAGER->playEff("atk4_1");
			break;
		case 2:
			SOUNDMANAGER->playEff("atk4_2");
			break;
		case 3:
			SOUNDMANAGER->playEff("atk4_3");
			break;
		case 4:
			SOUNDMANAGER->playEff("atk4_4");
			break;
		}
		break;
	}

	_atkCombo++;

	for (int i = 0; i < _em->getVEnemy().size(); i++)
	{
		if (_em->getVEnemy()[i]->getIdx().x == _idx.x + direction.x && _em->getVEnemy()[i]->getIdx().y == _idx.y + direction.y)
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
				}
				break;
			}
		}
	}
}

void player::setWeaponVDirection(int itemVal, POINT direction)
{
	POINT tmp;
	//무기가 단검 or 유리조각이면
	if (direction.x == -1)
	{
		if (itemVal == ITEM_DAGGER || itemVal == ITEM_GLASS_WEAPON)
		{
			//현재것만 넣음
			_vDirection.push_back(_direction);
		}
		//무기가 창이면
		else if (itemVal >= ITEM_SPEAR_NORMAL && itemVal <= ITEM_SPEAR_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x - 1, _direction.y };
			_vDirection.push_back(tmp);
		}
		//무기가 대검이엇으면
		else if (itemVal >= ITEM_SWORD_NORMAL && itemVal <= ITEM_SWORD_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x, _direction.y - 1 };
			_vDirection.push_back(tmp);
			tmp = { _direction.x, _direction.y + 1 };
			_vDirection.push_back(tmp);
		}
	}
	else if (direction.x == 1)
	{
		if (itemVal == ITEM_DAGGER || itemVal == ITEM_GLASS_WEAPON)
		{
			//현재것만 넣음
			_vDirection.push_back(_direction);
		}
		//무기가 창이면
		else if (itemVal >= ITEM_SPEAR_NORMAL && itemVal <= ITEM_SPEAR_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x + 1, _direction.y };
			_vDirection.push_back(tmp);
		}
		//무기가 대검이엇으면
		else if (itemVal >= ITEM_SWORD_NORMAL && itemVal <= ITEM_SWORD_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x, _direction.y - 1 };
			_vDirection.push_back(tmp);
			tmp = { _direction.x, _direction.y + 1 };
			_vDirection.push_back(tmp);
		}
	}
	else if (direction.y == -1)
	{
		if (itemVal == ITEM_DAGGER || itemVal == ITEM_GLASS_WEAPON)
		{
			//현재것만 넣음
			_vDirection.push_back(_direction);
		}
		//무기가 창이면
		else if (itemVal >= ITEM_SPEAR_NORMAL && itemVal <= ITEM_SPEAR_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x, _direction.y - 1 };
			_vDirection.push_back(tmp);
		}
		//무기가 대검이엇으면
		else if (itemVal >= ITEM_SWORD_NORMAL && itemVal <= ITEM_SWORD_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x - 1, _direction.y };
			_vDirection.push_back(tmp);
			tmp = { _direction.x + 1, _direction.y };
			_vDirection.push_back(tmp);
		}
	}
	else if (direction.y == 1)
	{
		if (itemVal == ITEM_DAGGER || itemVal == ITEM_GLASS_WEAPON)
		{
			//현재것만 넣음
			_vDirection.push_back(_direction);
		}
		//무기가 창이면
		else if (itemVal >= ITEM_SPEAR_NORMAL && itemVal <= ITEM_SPEAR_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x, _direction.y + 1 };
			_vDirection.push_back(tmp);
		}
		//무기가 대검이엇으면
		else if (itemVal >= ITEM_SWORD_NORMAL && itemVal <= ITEM_SWORD_OBSIDIAN)
		{
			_vDirection.push_back(_direction);
			tmp = { _direction.x - 1, _direction.y };
			_vDirection.push_back(tmp);
			tmp = { _direction.x + 1, _direction.y };
			_vDirection.push_back(tmp);
		}
	}
}

void player::useHpIten()
{
	_curHp += _recoverHp;
	_inventory[INVEN_HP].frameX = NULL;
	_inventory[INVEN_HP].frameY = NULL;
	_inventory[INVEN_HP].isGet = false;
	_inventory[INVEN_HP].isGlass = false;
	_inventory[INVEN_HP].itemKind = INVEN_HP;
	_inventory[INVEN_HP].itemVal = NULL;
	_recoverHp = 0;
	SOUNDMANAGER->playEff("heal");
	int rnd = RND->getFromIntTo(1, 4);
	switch (rnd)
	{
	case 1:
		SOUNDMANAGER->playEff("heal1");
		break;
	case 2:
		SOUNDMANAGER->playEff("heal2");
		break;
	case 3:
		SOUNDMANAGER->playEff("heal3");
		break;
	}
}

//===========================================
//					render
//===========================================

void player::renderUI()
{
	_inventory[INVEN_SHOVEL].slotImg->render(10, 10);
	_inventory[INVEN_SHOVEL].img->frameRender(15, 22, _inventory[INVEN_SHOVEL].frameX, _inventory[INVEN_SHOVEL].frameY);
	_inventory[INVEN_WEAPON].slotImg->render(80, 10);
	_inventory[INVEN_WEAPON].img->frameRender(85, 22, _inventory[INVEN_WEAPON].frameX, _inventory[INVEN_WEAPON].frameY);
	if (_inventory[INVEN_HP].isGet)
	{
		_inventory[INVEN_HP].slotImg->render(10, 80);
		_inventory[INVEN_HP].img->frameRender(15, 92, _inventory[INVEN_HP].frameX, _inventory[INVEN_HP].frameY);
	}
	if (_inventory[INVEN_ARMOR].isGet)
	{
		_inventory[INVEN_ARMOR].slotImg->render(150, 10);
		_inventory[INVEN_ARMOR].img->frameRender(155, 22, _inventory[INVEN_ARMOR].frameX, _inventory[INVEN_ARMOR].frameY);
	}

	if (!_inventory[INVEN_ARMOR].isGet && _inventory[INVEN_TORCH].isGet)
	{
		_inventory[INVEN_TORCH].slotImg->render(150, 10);
		_inventory[INVEN_TORCH].img->frameRender(155, 22, _inventory[INVEN_TORCH].frameX, _inventory[INVEN_TORCH].frameY);
	}
	else if (_inventory[INVEN_ARMOR].isGet && _inventory[INVEN_TORCH].isGet)
	{
		_inventory[INVEN_TORCH].slotImg->render(220, 10);
		_inventory[INVEN_TORCH].img->frameRender(225, 22, _inventory[INVEN_TORCH].frameX, _inventory[INVEN_TORCH].frameY);
	}
}

void player::txtRender()
{
	WCHAR str[128];
	swprintf_s(str, L"ct X : %f ct Y : %f ", _posCT.x, _posCT.y);
	D2DMANAGER->drawText(str, 0, 280, 20, 0x00ffff);
	swprintf_s(str, L"idx : %d idy : %d ", _idx.x, _idx.y);
	D2DMANAGER->drawText(str, 0, 300, 20, 0x00ffff);
	swprintf_s(str, L"HP : %d ", _curHp);
	D2DMANAGER->drawText(str, 0, 320, 20, 0x00ffff);
	swprintf_s(str, L"dmg : %d ", _dmg);
	D2DMANAGER->drawText(str, 0, 340, 20, 0x00ffff);
	swprintf_s(str, L"def : %d ", _def);
	D2DMANAGER->drawText(str, 0, 360, 20, 0x00ffff);
	swprintf_s(str, L"grooveChain : %d ", _grooveChain);
	D2DMANAGER->drawText(str, 0, 380, 20, 0x00ffff);
	swprintf_s(str, L"killCombo : %d ", _killCombo);
	D2DMANAGER->drawText(str, 0, 400, 20, 0x00ffff);
}

void player::effectRender()
{
	if (_isShowEff)
	{
		if (_inventory[INVEN_WEAPON].itemVal == ITEM_DAGGER || _inventory[INVEN_WEAPON].itemVal == ITEM_GLASS_WEAPON)
		{
			_effectImg->frameRenderAngle((_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY(), _effectImg->GetFrameX(), _effectImg->GetFrameY(), _effectAngle, 1.0f);
		}
		else if (_inventory[INVEN_WEAPON].itemVal >= ITEM_SPEAR_NORMAL && _inventory[INVEN_WEAPON].itemVal <= ITEM_SPEAR_OBSIDIAN)
		{
			//_effectImg->frameRenderAngle((_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY(), _effectImg->GetFrameX(), _effectImg->GetFrameY(), _angle, 1.0f);

			if (_direction.x == 1)
			{
				_effectImg->frameRenderAngle((_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() - 10, _effectImg->GetFrameX(), _effectImg->GetFrameY(), _effectAngle, 1.0f);
			}
			else if (_direction.x == -1)
			{
				_effectImg->frameRenderAngle((_idx.x + _direction.x - 1) * TILESIZE - CAMERA->getPosX(), (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() - 5, _effectImg->GetFrameX(), _effectImg->GetFrameY(), _effectAngle, 1.0f);
			}
			else if (_direction.y == 1)
			{
				_effectImg->frameRenderAngle((_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() - 20, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY(), _effectImg->GetFrameX(), _effectImg->GetFrameY(), _effectAngle, 1.0f);
			}
			else if (_direction.y == -1)
			{
				_effectImg->frameRenderAngle((_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() - 25, (_idx.y + _direction.y - 1) * TILESIZE - CAMERA->getPosY(), _effectImg->GetFrameX(), _effectImg->GetFrameY(), _effectAngle, 1.0f);
			}
		}
		else if (_inventory[INVEN_WEAPON].itemVal >= ITEM_SWORD_NORMAL && _inventory[INVEN_WEAPON].itemVal <= ITEM_SWORD_OBSIDIAN)
		{
			_effectImg->frameRenderAngle((_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _direction.y - 1) * TILESIZE - CAMERA->getPosY(), _effectImg->GetFrameX(), _effectImg->GetFrameY(), _effectAngle, 1.0f);
		}
	}
	if (_isShowShovel)
	{
		_shovelImg->frameRender(_posCT.x + (_direction.x * TILESIZE) - CAMERA->getPosX() - TILESIZE / 2, _posCT.y + (_direction.y * TILESIZE) - 30 - CAMERA->getPosY() - TILESIZE / 2, _shovelFrameX, _shovelFrameY);
	}
}

void player::brokeWeapon()
{
	_inventory[INVEN_WEAPON].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_WEAPON].slotImg = IMAGEMANAGER->findImage("slot_Weapon");
	_inventory[INVEN_WEAPON].frameX = 6;
	_inventory[INVEN_WEAPON].frameY = 10;
	_inventory[INVEN_WEAPON].isGet = false;
	_inventory[INVEN_WEAPON].isGlass = false;
	_inventory[INVEN_WEAPON].itemKind = INVEN_WEAPON;
	_inventory[INVEN_WEAPON].itemVal = ITEM_GLASS_WEAPON;
	_dmg = 1;
	_effectImg = IMAGEMANAGER->findImage("effect_Dagger");
	SOUNDMANAGER->playEff("brokeItem");
}

void player::brokeTorch()
{
	_inventory[INVEN_TORCH].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_TORCH].slotImg = IMAGEMANAGER->findImage("slot_Torch");
	_inventory[INVEN_TORCH].frameX = NULL;
	_inventory[INVEN_TORCH].frameY = NULL;
	_inventory[INVEN_TORCH].isGet = false;
	_inventory[INVEN_TORCH].isGlass = false;
	_inventory[INVEN_TORCH].itemKind = INVEN_TORCH;
	_inventory[INVEN_TORCH].itemVal = NULL;
	_torchPower = 2;
	SOUNDMANAGER->playEff("brokeItem");
}

void player::brokeShovel()
{
	_inventory[INVEN_SHOVEL].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_SHOVEL].slotImg = IMAGEMANAGER->findImage("slot_Shovel");
	_inventory[INVEN_SHOVEL].frameX = 0;
	_inventory[INVEN_SHOVEL].frameY = 6;
	_inventory[INVEN_SHOVEL].isGet = false;
	_inventory[INVEN_SHOVEL].isGlass = false;
	_inventory[INVEN_SHOVEL].itemKind = INVEN_SHOVEL;
	_inventory[INVEN_SHOVEL].itemVal = ITEM_GLASS_SHOVEL;
	_shovelDmg = 1;
	SOUNDMANAGER->playEff("brokeItem");
}

void player::brokeArmor()
{
	_inventory[INVEN_ARMOR].img = IMAGEMANAGER->findImage("Item");
	_inventory[INVEN_ARMOR].slotImg = IMAGEMANAGER->findImage("slot_Armor");
	_inventory[INVEN_ARMOR].frameX = NULL;
	_inventory[INVEN_ARMOR].frameY = NULL;
	_inventory[INVEN_ARMOR].isGet = false;
	_inventory[INVEN_ARMOR].isGlass = false;
	_inventory[INVEN_ARMOR].itemKind = INVEN_ARMOR;
	_inventory[INVEN_ARMOR].itemVal = NULL;
	_def = 0;
	_bodyAni = KEYANIMANAGER->findAnimation("playerBody", "normalBody");
	_bodyImg->SetFrameX(0);
	_bodyAni->start();
	_headImg->SetFrameX(0);
	_headAni->start();
	SOUNDMANAGER->playEff("brokeItem");
}


