#include "stdafx.h"
#include "enemyMgr.h"
#include "player.h"

enemyMgr::enemyMgr()
{
}


enemyMgr::~enemyMgr()
{
}

HRESULT enemyMgr::init()
{
	EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.5f, 10);

	enemy* test = new skeleton;
	test->makeEnemyIdx({ 5, 5 });
	_vEnemy.push_back(test);
	
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->init();
		_vEnemy[i]->linkPlayer(_player);
		_vEnemy[i]->linkMap(_vvMap);
		_vEnemy[i]->linkObj(_vvObj);
		_vEnemy[i]->linkLightMap(_vvLightMap);
	}

	_aStar = new aStar;
	_aStar->setMap(_vvObj);

	return S_OK;
}

void enemyMgr::release()
{
}

void enemyMgr::update()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->update();
		if (_isBeat)
		{
			_vEnemy[i]->setIsBeat(_isBeat);
			_vEnemy[i]->getListPath()->clear();
			_aStar->pathFinder(_vEnemy[i]->getIdx(), _player->getIdx(), _vEnemy[i]->getIdx(), *(_vEnemy[i]->getListPath()));
		}
	}
	KEYANIMANAGER->update("skeleton_Normal");
	KEYANIMANAGER->update("skeleton_Normal_Shadow");
	KEYANIMANAGER->update("dust");
}

void enemyMgr::render()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->render();
	}
}

void enemyMgr::createEnemy(int enemyType, POINT idx)
{
	enemy* enemy;
	switch (enemyType)
	{
		case ENEMY1_1_1:	// 검은박쥐

		break;
		case ENEMY1_1_2:	// 빨간박쥐

		break;
		case ENEMY1_2_1:	// 하얀해골
			enemy = new skeleton;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_2_2:	// 까만해골

		break;
		case ENEMY1_2_3:	// 노란해골

		break;
		case ENEMY1_3_1:	// 초록슬라임

		break;
		case ENEMY1_3_2:	// 파란슬라임

		break;
		case ENEMY1_3_3:	// 노란슬라임

		break;
		case ENEMY2_1_1:	// 유령

		break;
		case ENEMY2_1_2:	// 레이스

		break;
		case ENEMY2_2_1:	// 원숭이

		break;
		case ENEMY2_2_2:	// 좀비

		break;
		case ENEMY3_1_1:	// 미노타우로스

		break;
		case ENEMY3_1_2:	// 용

		break;
		case ENEMY3_1_3:	// 황금박쥐

		break;
		case ENEMY4_1_1:	// 폰

		break;
		case ENEMY4_1_2:	// 비숍

		break;
		case ENEMY4_1_3:	// 룩

		break;
		case ENEMY4_2_1:	// 나이트

		break;
		case ENEMY4_2_2:	// 퀸

		break;
		case ENEMY4_2_3:	// 킹

		break;
	}

}
