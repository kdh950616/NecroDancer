#include "stdafx.h"
#include "enemy.h"
#include "player.h"


enemy::enemy()
{
}


enemy::~enemy()
{
}

HRESULT enemy::init()
{


	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
}

void enemy::render()
{
}

void enemy::makeEnemyIdx(POINT mapIdx)
{
	_idx = mapIdx;

	_posLT = { (float)(mapIdx.x * TILESIZE), (float)(mapIdx.y * TILESIZE)};

	_posCT = { _posLT.x + (TILESIZE / 2), _posLT.y + (TILESIZE / 2) };

	_rc = { (float)_posLT.x,
			(float)_posLT.y,
			(float)_posLT.x + TILESIZE,
			(float)_posLT.y + TILESIZE };
}

void enemy::unfoundFunc()
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

void enemy::lightFunc()
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

void enemy::showAttackEffect()
{
	if (_direction.x == 0)
	{
		if (_direction.y == 1)
		{
			//EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
		else if (_direction.y == -1)
		{
			//EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
	}
	else if (_direction.y == 0)
	{
		if (_direction.x == 1)
		{
			//EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
		else if (_direction.x == -1)
		{
			//EFFECTMANAGER->play("enemy_attack", (_idx.x + _direction.x) * TILESIZE - CAMERA->getPosX() + TILESIZE / 2, (_idx.y + _direction.y) * TILESIZE - CAMERA->getPosY() + TILESIZE / 2);
		}
	}
}

void enemy::attackPlayer(int dmg)
{
	if ((*_player).getDef() - dmg > 0)
	{
		(*_player).setCurHp((*_player).getCurHp() - ((*_player).getDef() - dmg));
	}

	showAttackEffect();

	int rnd = RND->getFromIntTo(1, 7);
	switch (rnd)
	{
	case 1:
		SOUNDMANAGER->playEff("hit1");
		break;
	case 2:
		SOUNDMANAGER->playEff("hit2");
		break;
	case 3:
		SOUNDMANAGER->playEff("hit3");
		break;
	case 4:
		SOUNDMANAGER->playEff("hit4");
		break;
	case 5:
		SOUNDMANAGER->playEff("hit5");
		break;
	case 6:
		SOUNDMANAGER->playEff("hit6");
		break;
	}
	if ((*_player).getInventory(INVEN_ARMOR).isGlass == true)
	{
		(*_player).removeArmor();
	}
	else if ((*_player).getInventory(INVEN_WEAPON).isGlass == true)
	{
		(*_player).removeWeapon();
	}
	else if ((*_player).getInventory(INVEN_SHOVEL).isGlass == true)
	{
		(*_player).removeShovel();
	}
	else if ((*_player).getInventory(INVEN_TORCH).isGlass == true)
	{
		(*_player).removeTorch();
	}

	SOUNDMANAGER->playEff("hit");

	//���ι�� 0 ����� �߰�================================================================
}
