#include "stdafx.h"
#include "skeleton.h"
#include "player.h"


skeleton::skeleton()
{
}


skeleton::~skeleton()
{
}

HRESULT skeleton::init()
{
	enemy::init();

	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 1;
	_posZ = 0;
	_enemyType = SKELETON;
	_isNeedAstar = true;
	_savePos = _posLT;
	_gold = 1;

	_showPos = { 0,15 };

	return S_OK;
}

void skeleton::release()
{

}

void skeleton::update()
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
		_beatCount++;		//비트카운트 증가
		_isBeat = false;	//비트타이밍 펄스
		
		lightFunc();
	
		//비트카운트가 1이면 -> 손드는 이미지로 변경
		if (_beatCount == 1)
		{
			attackAni_LightChange();
		}
		//비트카운트가 2면 -> 이동하자
		else if (_beatCount >= 2)
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
				}
				else if (_direction.y == -1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
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
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isMove = true;
					_isLeft = false;
					_vec.x += SPEED;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
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
				}
			}
		}
	}
	//움직여야하면
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

void skeleton::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(),_ani);
	}
	else if (!_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
}

//=========================================================
//						init
//=========================================================

void skeleton::imageInit()
{
	_img = IMAGEMANAGER->findImage("skeleton_Normal");

	KEYANIMANAGER->addAnimationType("skeleton_Normal");
	KEYANIMANAGER->addAnimationType("skeleton_Normal_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Normal", "skeleton_Normal_Stand", "skeleton_Normal", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Normal", "skeleton_Normal_Attack", "skeleton_Normal", attack, 4, ANIMATION_FPS, false);

	int stand_Shadow[] = { 8,9,10,11 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Normal_Shadow", "skeleton_Normal_Stand", "skeleton_Normal", stand_Shadow, 4, ANIMATION_FPS, true);

	int attack_Shadow[] = { 12,13,14,15 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Normal_Shadow", "skeleton_Normal_Attack", "skeleton_Normal", attack_Shadow, 4, ANIMATION_FPS, false);
	
	_ani = KEYANIMANAGER->findAnimation("skeleton_Normal_Shadow", "skeleton_Normal_Stand");
	_ani->start();

	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

//=========================================================
//						update
//=========================================================

void skeleton::unfoundFunc()
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
			_ani = KEYANIMANAGER->findAnimation("skeleton_Normal", "skeleton_Normal_Stand");
			_ani->start();
		}
	}
}

void skeleton::lightFunc()
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

void skeleton::attackAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Normal", "skeleton_Normal_Attack");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Normal_Shadow", "skeleton_Normal_Attack");
		_ani->start();
	}
}

void skeleton::standAni_LightChange()
{
	//밝음상태면	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Normal", "skeleton_Normal_Stand");
		_ani->start();
	}
	//그림자상태면
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Normal_Shadow", "skeleton_Normal_Stand");
		_ani->start();
	}
}

void skeleton::moveCal()
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


//=========================================================
//						render
//=========================================================
