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
	imageInit();

	//타입마다 다름 일단 기본해골 체력 1짜리로
	_maxHp = 1;
	_curHp = 1;
	_speed = SPEED;
	_dmg = 1;

	_posZ = 0;

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
	//발견됐고  비트타이밍이면
	else if (_isFind && _isBeat)
	{
		_beatCount++;		//비트카운트 증가
		_isBeat = false;	//비트타이밍 펄스
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
		//비트카운트 증가
	
		//비트카운트가 1이면 -> 손드는 이미지로 변경
		if (_beatCount == 1)
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
		//비트카운트가 2면 -> 이동하자
		else if (_beatCount == 2)
		{
			_beatCount = 0;
			_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
			_savePos = _posLT;
			//밝음상태면
			if (_isLight)
			{
				_ani = KEYANIMANAGER->findAnimation("skeleton_Normal", "skeleton_Normal_Stand");
				_ani->start();
			}
			//그림자상태면
			else if (!_isLight)
			{
				_ani = KEYANIMANAGER->findAnimation("skeleton_Normal_Shadow", "skeleton_Normal_Stand");
				_ani->start();
			}

			//에이슷하로 구한 내가 가야할길이 플레이어라면 
			if ((*_player).getIdx().x == _listPath.begin()->x && (*_player).getIdx().y == _listPath.begin()->y)
			{
				(*_player).setCurHp((*_player).getCurHp() - _dmg);

				if (_direction.x == 0)
				{
					if (_direction.y == 1)
					{
						EFFECTMANAGER->play("enemy_attack", _listPath.begin()->x * TILESIZE, _listPath.begin()->y * TILESIZE);
					}
					else if (_direction.y == -1)
					{
						EFFECTMANAGER->play("enemy_attack", _listPath.begin()->x * TILESIZE, _listPath.begin()->y * TILESIZE);

					}
				}
				else if (_direction.y == 0)
				{
					if (_direction.x == 1)
					{
						EFFECTMANAGER->play("enemy_attack", _listPath.begin()->x * TILESIZE, _listPath.begin()->y * TILESIZE);

					}
					else if (_direction.x == -1)
					{
						EFFECTMANAGER->play("enemy_attack", _listPath.begin()->x * TILESIZE, _listPath.begin()->y * TILESIZE);

					}
				}
			}
			else if (_direction.x == 0)
			{
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
				if (_direction.x == 1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isLeft = false;
					_isMove = true;
					_vec.x += SPEED;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
				}
				else if (_direction.x == -1)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
					_isLeft = true;
					_isMove = true;
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
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;

		
	}

}

void skeleton::render()
{
	_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(),_ani);
	}
	else if (!_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
}

void skeleton::imageInit()
{
	IMAGEMANAGER->addFrameImage("skeleton_Normal", L"images/monster/normal/skeleton.png", 384, 100, 8, 2);
	_img = IMAGEMANAGER->findImage("skeleton_Normal");
	//이펙트 등록

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


	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");


	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);
	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}
