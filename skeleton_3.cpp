#include "stdafx.h"
#include "skeleton_3.h"
#include "player.h"


skeleton_3::skeleton_3()
{
}


skeleton_3::~skeleton_3()
{
}

HRESULT skeleton_3::init()
{
	imageInit();

	_maxHp = 3;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 4;
	_posZ = 0;
	_enemyType = SKELETON;
	_isNeedAstar = true;
	_gold = 3;


	return S_OK;
}

void skeleton_3::release()
{
}

void skeleton_3::update()
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
		
		if (_curHp <= 1 && !_isSpecialAct)
		{
			_isSpecialAct = true;
			_direction.x = -1 * (_listPath.begin()->x - _idx.x);
			_direction.y = -1 * (_listPath.begin()->y - _idx.y);

			int rnd = RND->getFromIntTo(1, 4);
			switch (rnd)
			{
			case 1:
				SOUNDMANAGER->playEff("skeleton_HeadLess1");
				break;
			case 2:
				SOUNDMANAGER->playEff("skeleton_HeadLess2");
				break;
			case 3:
				SOUNDMANAGER->playEff("skeleton_HeadLess3");
				break;

			}
		}
		
		if (!_isSpecialAct)
		{
			//��Ʈī��Ʈ�� 1�̸� -> �յ�� �̹����� ����
			if (_beatCount == 1)
			{
				attackAni_LightChange();
			}
			//��Ʈī��Ʈ�� 2�� -> �̵�����
			else if (_beatCount == 2)
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
		else if (_isSpecialAct)
		{
			_savePos = _posLT;
			lightFunc();
			headlessAni_LightChange();

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				(*_player).setCurHp((*_player).getCurHp() - _dmg);

				showAttackEffect();
			}
			else if (_direction.x == 0)
			{
				_dustAni->start();
				if (_direction.y == 1)
				{
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
					_vec.y += SPEED;
					_isMove = true;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
				else if (_direction.y == -1)
				{
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
					_vec.y -= SPEED;
					_isMove = true;
					_jumpPower = JUMPPOWER_VERTICAL;
					_posZ = 0;
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
			}
			else if (_direction.y == 0)
			{
				_dustAni->start();
				if (_direction.x == 1)
				{
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
					_vec.x += SPEED;
					_isMove = true;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
				else if (_direction.x == -1)
				{
					if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
					{
						(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
					}
					_vec.x -= SPEED;
					_isMove = true;
					_jumpPower = JUMPPOWER_HORIZON;
					_posZ = 0;
					_idx.x = (_posLT.x) / TILESIZE;
					_idx.y = (_posLT.y) / TILESIZE;
				}
			}
		}
	}
	//���������ϸ�
	if (_isMove)
	{
		if (!_isSpecialAct)
		{
			moveCal();
		}
		else if (_isSpecialAct)
		{
			specialMoveCal();
		}
	}

	if (_curHp <= 0)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
}

void skeleton_3::render()
{
	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - CAMERA->getPosY(), 1.0f);
	if (_isLeft)
	{
		_img->aniRender(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
	else if (!_isLeft)
	{
		_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX(), _posLT.y - 15 - _posZ - CAMERA->getPosY(), _ani);
	}
}

void skeleton_3::imageInit()
{
	IMAGEMANAGER->addFrameImage("skeleton_Black", L"images/monster/normal/skeleton_black.png", 432, 100, 9, 2);
	_img = IMAGEMANAGER->findImage("skeleton_Black");

	//����Ʈ ���
	//EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	KEYANIMANAGER->addAnimationType("skeleton_Black");
	KEYANIMANAGER->addAnimationType("skeleton_Black_Shadow");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Black", "skeleton_Black_Stand", "skeleton_Black", stand, 4, ANIMATION_FPS, true);

	int attack[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Black", "skeleton_Black_Attack", "skeleton_Black", attack, 4, ANIMATION_FPS, false);

	int headless[] = { 8 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Black", "skeleton_Black_Headless", "skeleton_Black", headless, 1, ANIMATION_FPS, true);

	int stand_Shadow[] = { 9,10,11,12 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Black_Shadow", "skeleton_Black_Stand", "skeleton_Black", stand_Shadow, 4, ANIMATION_FPS, true);

	int attack_Shadow[] = { 13,14,15,16 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Black_Shadow", "skeleton_Black_Attack", "skeleton_Black", attack_Shadow, 4, ANIMATION_FPS, false);

	int headless_Shadow[] = { 17 };
	KEYANIMANAGER->addArrayFrameAnimation("skeleton_Black_Shadow", "skeleton_Black_Headless", "skeleton_Black", headless_Shadow, 1, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("skeleton_Black_Shadow", "skeleton_Black_Stand");
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

void skeleton_3::unfoundFunc()
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
			_ani = KEYANIMANAGER->findAnimation("skeleton_Black", "skeleton_Black_Stand");
			_ani->start();
		}
	}
}

void skeleton_3::lightFunc()
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

void skeleton_3::attackAni_LightChange()
{
	//�������¸�	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Black", "skeleton_Black_Attack");
		_ani->start();
	}
	//�׸��ڻ��¸�
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Black_Shadow", "skeleton_Black_Attack");
		_ani->start();
	}
}

void skeleton_3::standAni_LightChange()
{
	//�������¸�	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Black", "skeleton_Black_Stand");
		_ani->start();
	}
	//�׸��ڻ��¸�
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Black_Shadow", "skeleton_Black_Stand");
		_ani->start();
	}
}

void skeleton_3::headlessAni_LightChange()
{
	//�������¸�	
	if (_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Black", "skeleton_Black_Headless");
		_ani->start();
	}
	//�׸��ڻ��¸�
	if (!_isLight)
	{
		_ani = KEYANIMANAGER->findAnimation("skeleton_Black_Shadow", "skeleton_Black_Headless");
		_ani->start();
	}
}

void skeleton_3::moveCal()
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
	if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
	}
	else
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
}

void skeleton_3::specialMoveCal()
{
	_posZ += _jumpPower;
	_jumpPower -= GRAVITY;
	if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == true)
	{
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
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			_vec.x = 0;
			_savePos = _posLT;
			_isMove = false;
			_jumpPower = 0;
			_posZ = 0;
			if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE)
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			}
			else
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
			}
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		}
		else if (_posLT.y - _savePos.y >= TILESIZE || _savePos.y - _posLT.y >= TILESIZE)
		{
			if (_vec.y > 0)
			{
				_posLT.y = _savePos.y + TILESIZE;
			}
			else if (_vec.y < 0)
			{
				_posLT.y = _savePos.y - TILESIZE;
			}
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			_vec.y = 0;
			_savePos = _posLT;
			_isMove = false;
			_jumpPower = 0;
			_posZ = 0;
			if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE)
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			}
			else
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
			}
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		}
	}
	else if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
	{
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
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
				(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
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
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
				(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
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
				_vec.x = 0;
				_savePos = _posLT;
				_isMove = false;
				_isBackward = false;
				_jumpPower = 0;
				_posZ = 0;
				if ((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
				}
				else
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				}
				_idx.x = (_posLT.x) / TILESIZE;
				_idx.y = (_posLT.y) / TILESIZE;
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
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
				_vec.y = 0;
				_savePos = _posLT;
				_isMove = false;
				_isBackward = false;
				_jumpPower = 0;
				_posZ = 0;
				if((*_vvObj)[_idx.y][_idx.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[_idx.y][_idx.x]->getAttribute() <= OBJ_DOOR_SIDE)
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
				}
				else
				{
					(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				}
				_idx.x = (_posLT.x) / TILESIZE;
				_idx.y = (_posLT.y) / TILESIZE;
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			}
		}
	}
	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;

}

