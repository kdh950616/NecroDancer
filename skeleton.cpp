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


	return S_OK;
}

void skeleton::release()
{

}

void skeleton::update()
{
	//�߰ߵ��� �ʾҰ� ī�޶���� �ȿ��ִٸ�
	if (!_isFind
		&& CAMERA->getPosX() - TILESIZE <= _posLT.x
		&& CAMERA->getPosY() - TILESIZE <= _posLT.y
		&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _posLT.x
		&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _posLT.y)
	{
		unfoundFunc();
	}
	//�߰ߵư�  ��ƮŸ�̹��̸�
	else if (_isFind && _isBeat)
	{
		_beatCount++;		//��Ʈī��Ʈ ����
		_isBeat = false;	//��ƮŸ�̹� �޽�
		
		lightFunc();
	
		//��Ʈī��Ʈ�� 1�̸� -> �յ�� �̹����� ����
		if (_beatCount == 1)
		{
			attackAni_LightChange();
		}
		//��Ʈī��Ʈ�� 2�� -> �̵�����
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

			//���̽��Ϸ� ���� ���� �����ұ��� �÷��̾��� 
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
	//���������ϸ�
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
	IMAGEMANAGER->addFrameImage("skeleton_Normal", L"images/monster/normal/skeleton.png", 384, 100, 8, 2);
	_img = IMAGEMANAGER->findImage("skeleton_Normal");

	//����Ʈ ���
	//EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.f, 10);

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

//=========================================================
//						update
//=========================================================

void skeleton::unfoundFunc()
{
	//��ƮŸ�ָ̹���
	if (_isBeat)
	{
		_isBeat = false;
		//������ ���� ������ �������ٸ� -> �÷��̾��� Ƚ�ҹ����ȿ� ���´ٸ�
		if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() <= 0.649f)
		{
			//�߰ߵ����� �ٲ�
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
	//Ƚ�ҹ��� �ȿ� ������
	if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() <= 0.64f)
	{
		//����
		_isLight = true;
	}
	//Ƚ�� ���� �ۿ�������
	else if ((*_vvLightMap)[_idx.y][_idx.x]->getOpacity() > 0.64f)
	{
		//��ο�
		_isLight = false;
	}
}

void skeleton::attackAni_LightChange()
{
	//�������¸�	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Normal", "skeleton_Normal_Attack");
		_ani->start();
	}
	//�׸��ڻ��¸�
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Normal_Shadow", "skeleton_Normal_Attack");
		_ani->start();
	}
}

void skeleton::standAni_LightChange()
{
	//�������¸�	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Normal", "skeleton_Normal_Stand");
		_ani->start();
	}
	//�׸��ڻ��¸�
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
