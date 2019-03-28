#include "stdafx.h"
#include "minotaur.h"
#include "player.h"

minotaur::minotaur()
{
}


minotaur::~minotaur()
{
}

HRESULT minotaur::init()
{
	imageInit();

	_isStun = false;
	_maxHp = 3;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 4;
	_posZ = 0; 
	_enemyType = MINOTAUR;
	_isNeedAstar = true;
	_beatCount = 0;
	_isSpecialAct = false;

	return S_OK;
}

void minotaur::release()
{
}

void minotaur::update()
{
	//무브나 이런거 빼고 왠만한건 다 비트마다 한번씩만 검사하자.
	if (_isBeat)
	{
		_isBeat = false;
	
		//안 발견됐을때 카메라 안에 들어오면 find 트루
		if (!_isFind
			&&CAMERA->getPosX() - TILESIZE <= _posLT.x
			&& CAMERA->getPosY() - TILESIZE <= _posLT.y
			&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
			&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
		{
			//미노타우루스 발견 노래 넣자
			_isFind = true;
		}
	
		if (_isFind)
		{
			lightFunc();
			if (_isStun)
			{
				_beatCount++;
	
				if (_beatCount >= 3)
				{
					_isStun = false;
					_isCharging = false;
					_beatCount = 0;
					_savePos = _posLT;
					_direction = { 0,0 };
				}
			}
			else
			{
				if (!_isCharging)
				{
					standAni_LightChange();
				}
				_savePos = _posLT;
				if (_listPath.size() > 0 && !_isCharging)
				{
					_direction = { _listPath.begin()->x - _idx.x, _listPath.begin()->y - _idx.y };
				}
				else if(!_isCharging)
				{
					POINT tmp = { RND->getInt(2),RND->getInt(2) };
					_listPath.push_back(tmp);
					_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
				}
				//에이스타로 구한 갈곳이 벽이었다면
				if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == OBJ_WALL1 ||
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == OBJ_WALL2 ||
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_TORCH_WALL1||
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getAttribute() == ETC_TORCH_WALL2)
				{
					//벽없앰
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->objSetDefault();
					if (_isCharging)
					{
						_isStun = true;
						_isSpecialAct = true;
						stunAni_LightChange();
					}
				}
				else if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE ||
					(*_vvObj)[_idx.y][_idx.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= ETC_TORCH_BOSS)
				{

					if (_direction.x == 0)
					{
						_dustAni->start();
						if (_direction.y == 1)
						{
							_isMove = true;
							_vec.y += SPEED;
							_jumpPower = JUMPPOWER_VERTICAL;
							_posZ = 0;
						}
						else if (_direction.y == -1)
						{
							_isMove = true;
							_vec.y -= SPEED;
							_jumpPower = JUMPPOWER_VERTICAL;
							_posZ = 0;
						}
					}
					else if (_direction.y == 0)
					{
						_dustAni->start();
						if (_direction.x == 1)
						{
							_isMove = true;
							_isLeft = false;
							_vec.x += SPEED;
							_jumpPower = JUMPPOWER_HORIZON;
							_posZ = 0;
						}
						else if (_direction.x == -1)
						{
							_isMove = true;
							_isLeft = true;
							_vec.x -= SPEED;
							_jumpPower = JUMPPOWER_HORIZON;
							_posZ = 0;
						}
					}
				}
				else if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
				{
					(*_player).setCurHp((*_player).getCurHp() - _dmg);
	
					showAttackEffect();
	
					_isStun = true;
					_isSpecialAct = true;
					stunAni_LightChange();

					if (_direction.x == 0)
					{
						_dustAni->start();
						if (_direction.y == 1)
						{
							_isMove = true;
							_vec.y += SPEED;
							_jumpPower = JUMPPOWER_VERTICAL;
							_posZ = 0;
						}
						else if (_direction.y == -1)
						{
							_isMove = true;
							_vec.y -= SPEED;
							_jumpPower = JUMPPOWER_VERTICAL;
							_posZ = 0;
						}
					}
					else if (_direction.y == 0)
					{
						_dustAni->start();
						if (_direction.x == 1)
						{
							_isMove = true;
							_isLeft = false;
							_vec.x += SPEED;
							_jumpPower = JUMPPOWER_HORIZON;
							_posZ = 0;
						}
						else if (_direction.x == -1)
						{
							_isMove = true;
							_isLeft = true;
							_vec.x -= SPEED;
							_jumpPower = JUMPPOWER_HORIZON;
							_posZ = 0;
						}
					}
				}
				//===============================
				//				차징
				//===============================
				//x축이 같으면 y축검사
				else if (_idx.x == (*_player).getIdx().x && !_isCharging)
				{
					//뭐가 더큰지 비교해서 포문만들고 돌리면서 벽있는지 검사함 없으면 불변수 트루
					if ((*_player).getIdx().y > _idx.y)
					{
						for (int i = _idx.y; i <= (*_player).getIdx().y; i++)
						{
							if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE ||
								(*_vvObj)[_idx.y][_idx.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= ETC_TORCH_BOSS) 
							{
								break;
							}
							if (i == (*_player).getIdx().y)
							{
								_isCharging = true;
								attackAni_LightChange();
								_direction = { 0, 1 };
							}
						}
					}
					else if ((*_player).getIdx().y < _idx.y)
					{
						for (int i = (*_player).getIdx().y; i <= _idx.y; i++)
						{
							if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE ||
								(*_vvObj)[_idx.y][_idx.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= ETC_TORCH_BOSS) 
							{
								break;
							}
							if (i == _idx.y)
							{
								_isCharging = true;
								attackAni_LightChange();
								_direction = { 0, -1 };
							}
						}
					}
				}
				//y 축이 같으면 x축검사
				else if (_idx.y == (*_player).getIdx().y && !_isCharging)
				{
					//뭐가 더큰지 비교해서 포문만들고 돌리면서 벽있는지 검사함 없으면 불변수 트루
					if ((*_player).getIdx().x > _idx.x)
					{
						for (int i = _idx.x; i <= (*_player).getIdx().x; i++)
						{
							if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE ||
								(*_vvObj)[_idx.y][_idx.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= ETC_TORCH_BOSS) 
							{
								break;
							}
							if (i == (*_player).getIdx().x)
							{
								_isCharging = true;
								attackAni_LightChange();
								_direction = { 1,0 };
							}
						}
					}
					else if ((*_player).getIdx().x < _idx.x)
					{
						for (int i = (*_player).getIdx().x; i <= _idx.x; i++)
						{
							if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE ||
								(*_vvObj)[_idx.y][_idx.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= ETC_TORCH_BOSS) 
							{
								break;
							}
							if (i == _idx.x)
							{
								_isCharging = true;
								attackAni_LightChange();
								_direction = { -1,0 };
							}
						}
					}
				}
	
				if (_direction.x == 0 && !_isSpecialAct)
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
				else if (_direction.y == 0 && !_isSpecialAct)
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
	}
	if (_isMove)
	{
		if (_isSpecialAct)
		{
			specialMoveCal();
		}
		else if (!_isSpecialAct)
		{
			moveCal();
		}
	}
}

void minotaur::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 10, _posLT.y - 15 - CAMERA->getPosY() + 50, 1.0f);
	if (_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX() - 25, _posLT.y - 15 - _posZ - CAMERA->getPosY() - 40, _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX() - 25, _posLT.y - 15 - _posZ - CAMERA->getPosY() - 40, _ani);
	}
}

void minotaur::imageInit()
{
	IMAGEMANAGER->addFrameImage("minotaur", L"images/monster/miniboss/minotaur.png", 900, 196, 9, 2);
	_img = IMAGEMANAGER->findImage("minotaur");

	EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	KEYANIMANAGER->addAnimationType("minotaur");
	KEYANIMANAGER->addAnimationType("minotaur_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("minotaur", "minotaur_Stand", "minotaur", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4 };
	KEYANIMANAGER->addArrayFrameAnimation("minotaur", "minotaur_Attack", "minotaur", attack, 1, 1, true);

	int stun[] = { 5,6,7,8 };
	KEYANIMANAGER->addArrayFrameAnimation("minotaur", "minotaur_Stun", "minotaur", stun, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 9,10,11,12 };
	KEYANIMANAGER->addArrayFrameAnimation("minotaur_Shadow", "minotaur_Stand", "minotaur", stand_Shadow, 4, ANIMATION_FPS, true);

	int attack_Shadow[] = { 13 };
	KEYANIMANAGER->addArrayFrameAnimation("minotaur_Shadow", "minotaur_Attack", "minotaur", attack_Shadow, 1, 1, true);

	int stun_Shadow[] = { 14,15,16,17 };
	KEYANIMANAGER->addArrayFrameAnimation("minotaur_Shadow", "minotaur_Stun", "minotaur", stun_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("minotaur_Shadow", "minotaur_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Large", L"images/monster/miniboss/large_shadow.png", 86,28);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Large");

	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);
	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

void minotaur::standAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("minotaur", "minotaur_Stand");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("minotaur_Shadow", "minotaur_Stand");
		_ani->start();
	}
}

void minotaur::stunAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("minotaur", "minotaur_Stun");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("minotaur_Shadow", "minotaur_Stun");
		_ani->start();
	}
}

void minotaur::attackAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("minotaur", "minotaur_Attack");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("minotaur_Shadow", "minotaur_Attack");
		_ani->start();
	}
}

void minotaur::moveCal()
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

void minotaur::specialMoveCal()
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
			stunAni_LightChange();
			_vec.x = 0;
			_savePos = _posLT;
			_isMove = false;
			_isBackward = false;
			_isSpecialAct = false;
			_isCharging = false;
			_isStun = true;
			_jumpPower = 0;
			_posZ = 0;
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			_direction = { 0,0 };
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
			stunAni_LightChange();
			_vec.y = 0;
			_savePos = _posLT;
			_isMove = false;
			_isBackward = false;
			_isSpecialAct = false;
			_isCharging = false;
			_isStun = true;
			_jumpPower = 0;
			_posZ = 0;
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			_direction = { 0,0 };
		}
	}
	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
}
