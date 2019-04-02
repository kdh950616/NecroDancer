#include "stdafx.h"
#include "slime.h"


slime::slime()
{
}


slime::~slime()
{
}

HRESULT slime::init()
{
	enemy::init();

	imageInit();

	_maxHp = 1;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 9999;
	_enemyType = SLIME;
	_isNeedAstar = false;
	_gold = 1;

	_showPos = { 0,15 };

	return S_OK;
}

void slime::release()
{
}

void slime::update()
{
	if (CAMERA->getPosX() - TILESIZE <= _posLT.x
		&& CAMERA->getPosY() - TILESIZE <= _posLT.y
		&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
		&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
	{
		if (_isBeat)
		{
			_isBeat = false;
			lightFunc();

			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			_jumpPower = JUMPPOWER_HORIZON;
			_posZ = 0;
		}
	}
	if (_jumpPower >= -6.0f && _jumpPower != 0)
	{
		_posZ += _jumpPower;
		_jumpPower -= GRAVITY;
	}
	else
	{
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
	//else
	//{
	//	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	//}
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	//(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);

	if (_curHp <= 0)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
}

void slime::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
}

void slime::imageInit()
{
	IMAGEMANAGER->addFrameImage("slime_Normal", L"images/monster/normal/slime_green.png", 208, 104, 4, 2);
	_img = IMAGEMANAGER->findImage("slime_Normal");

	//EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	KEYANIMANAGER->addAnimationType("slime_Normal");
	KEYANIMANAGER->addAnimationType("slime_Normal_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("slime_Normal", "slime_Normal_Stand", "slime_Normal", stand, 4, ANIMATION_FPS, true);

	int stand_Shadow[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("slime_Normal_Shadow", "slime_Normal_Stand", "slime_Normal", stand_Shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("slime_Normal_Shadow", "slime_Normal_Stand");
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

void slime::lightFunc()
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

	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("slime_Normal", "slime_Normal_Stand");
		_ani->start();
	}
	else if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("slime_Normal_Shadow", "slime_Normal_Stand");
		_ani->start();
	}
}
