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
	IMAGEMANAGER->addFrameImage("enemy_Attack", L"images/effect/swipe_enemy.png", 270, 48, 5, 1);
	//EFFECTMANAGER->addEffect("enemy_attack", "images/effect/swipe_enemy.png", 270, 48, 54, 48, 5, 0.1f, 10);

	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->init();
		_vEnemy[i]->linkPlayer(_player);
		_vEnemy[i]->linkMap(_vvMap);
		_vEnemy[i]->linkObj(_vvObj);
		_vEnemy[i]->linkLightMap(_vvLightMap);

		//생성이 끝나는 시점
		if (i == _vEnemy.size() - 1)
		{
			//에너미 사이즈만큼 포문을 한번 더돔
			for (int j = 0; j < _vEnemy.size(); j++)
			{
				//에너미 타입이 레이스면
				if (_vEnemy[j]->getEnemyType() == WRAITH)
				{
					//비트카운트 랜덤설정 (15초 ~ 음악종료 30초전 사이)
					_vEnemy[j]->setBeatCount(RND->getFromIntTo(15000,SOUNDMANAGER->getVBeat().back().beat - 30000));
					//테스트용 비트카운트 설정
					//_vEnemy[j]->setBeatCount(RND->getFromIntTo(1000,10000));
				}
			}
		}
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
	for (int i = 0; i < _vEnemy.size();)
	{
		if (_isBeat)
		{
			_vEnemy[i]->setIsBeat(_isBeat);
			if (_vEnemy[i]->getIsSpecialAct() == false && _vEnemy[i]->getIsNeedAstar() == true)
			{
				_vEnemy[i]->getListPath()->clear();
				if (_vEnemy[i]->getEnemyType() == MINOTAUR || _vEnemy[i]->getEnemyType() == DRAGON)
				{
					_aStar->pathFinder2(_vEnemy[i]->getIdx(), _player->getIdx(), _vEnemy[i]->getIdx(), *(_vEnemy[i]->getListPath()));
				}
			//	else if (_vEnemy[i]->getEnemyType() == KNIGHT || _vEnemy[i]->getEnemyType() == BISHOP || _vEnemy[i]->getEnemyType() == ROOK || _vEnemy[i]->getEnemyType() == QUEEN)
			//	{
			//
			//	}
				else
				{
					_aStar->pathFinder(_vEnemy[i]->getIdx(), _player->getIdx(), _vEnemy[i]->getIdx(), *(_vEnemy[i]->getListPath()));
				}

			}
			//레이스등장 전용
			if (_vEnemy[i]->getEnemyType() == WRAITH && !_vEnemy[i]->getIsFind() && _vEnemy[i]->getBeatCount() <= *_time)
			{
				_vEnemy[i]->setIsFind(true);
				_vEnemy[i]->setBeatCount(0);
				SOUNDMANAGER->playEff("wraith_Cry");
			}
		}
		_vEnemy[i]->update();

		if (_vEnemy[i]->getCurHp() <= 0)
		{
			(*_vvObj)[_vEnemy[i]->getIdx().y][_vEnemy[i]->getIdx().x]->setIsAvailMove(true);
			(*_player).setKillCombo((*_player).getKillCombo() + 1);
			int rnd;
			switch (_vEnemy[i]->getEnemyType())
			{
				case BAT:
					SOUNDMANAGER->playEff("bat_Death");
				break;
				case SKELETON:
					SOUNDMANAGER->playEff("skeleton_Death");
				break;
				case SLIME:
					rnd = RND->getFromIntTo(1, 5);
					switch (rnd)
					{
					case 1:
						SOUNDMANAGER->playEff("slime_Death1");
						break;
					case 2:
						SOUNDMANAGER->playEff("slime_Death2");
						break;
					case 3:
						SOUNDMANAGER->playEff("slime_Death3");
						break;
					case 4:
						SOUNDMANAGER->playEff("slime_Death4");
						break;
					}
				break;
				case ZOMBIE:
					SOUNDMANAGER->playEff("zombie_Death");
				break;
				case WRAITH:
					SOUNDMANAGER->playEff("wraith_Death");
				break;
				case CLONE:
					SOUNDMANAGER->playEff("clone_Death");
				break;
				case ARMADILLO:
					SOUNDMANAGER->playEff("armadillo_Death");
				break;
				case DRAGON:
					SOUNDMANAGER->playEff("dragon_Death");
				break;
				case BAT_BOSS:
					SOUNDMANAGER->playEff("bat_Boss_Death");
				break;
				case BANSHEE:
					SOUNDMANAGER->stop("banshee_Loop");
					SOUNDMANAGER->setEffectVol(SOUNDMANAGER->getEffectVol() * 10);
					SOUNDMANAGER->setZoneVolume(*_stageKeyName,SOUNDMANAGER->getVolume());
					SOUNDMANAGER->playEff("banshee_Death");
				break;
				case PAWN:
				break;
				case KNIGHT:	//드럼
				break;
				case BISHOP:	//호른
				break;
				case ROOK:		//바이올린
				break;
				case QUEEN:		//키보드
				break;
				case KING:		//베이스(최종)
				break;
			}
			_vEnemy.erase(_vEnemy.begin() + i);
			if ((*_player).getInventory(INVEN_WEAPON).itemVal == ITEM_SPEAR_BLOOD || (*_player).getInventory(INVEN_WEAPON).itemVal == ITEM_SWORD_BLOOD)
			{
				(*_player).addBloodKill(1);
			}
		}
		else i++;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F4))
	{
		for (int i = 0; i < _vEnemy.size();)
		{
			_vEnemy[i]->setCurHp(_vEnemy[i]->getCurHp() - 1);

			//피 다깎이면
			if (_vEnemy[i]->getCurHp() <= 0)
			{
				//자기 주변 검사하는 포문
				for (int j = -1; j <= 1; j++)
				{
					for (int k = -1; k <= 1; k++)
					{
						//벡터 터짐방지
						if ((*_vvObj).size() > _vEnemy[i]->getIdx().y + j && _vEnemy[i]->getIdx().y + j > 0 
							&& (*_vvObj)[0].size() > _vEnemy[i]->getIdx().x + k && _vEnemy[i]->getIdx().x + k > 0)
						{
							//주변녀석의 속성이 없으면 -> 벽일경우를 위해 
							if ((*_vvObj)[_vEnemy[i]->getIdx().y + j][_vEnemy[i]->getIdx().x + k]->getAttribute() == OBJ_NONE)
							{
								//해당 오브젝트 true로 바꿔줌. -> 지워지면서 땅건들던거 초기화하는거임
								(*_vvObj)[_vEnemy[i]->getIdx().y + j][_vEnemy[i]->getIdx().x + k]->setIsAvailMove(true);
							}
						}
					}
				}
				_vEnemy.erase(_vEnemy.begin() + i);
			}
			else i++;
		}
	}
	KEYANIMANAGER->update("skeleton_Normal");
	KEYANIMANAGER->update("skeleton_Normal_Shadow");
	KEYANIMANAGER->update("skeleton_Yellow");
	KEYANIMANAGER->update("skeleton_Yellow_Shadow");
	KEYANIMANAGER->update("skeleton_Black");
	KEYANIMANAGER->update("skeleton_Black_Shadow");
	KEYANIMANAGER->update("bat_Normal");
	KEYANIMANAGER->update("bat_Normal_Shadow");
	KEYANIMANAGER->update("bat_Red");
	KEYANIMANAGER->update("bat_Red_Shadow");
	KEYANIMANAGER->update("slime_Normal");
	KEYANIMANAGER->update("slime_Normal_Shadow");
	KEYANIMANAGER->update("slime_Blue");
	KEYANIMANAGER->update("slime_Blue_Shadow");
	KEYANIMANAGER->update("zombie");
	KEYANIMANAGER->update("zombie_Shadow");
	KEYANIMANAGER->update("wraith");
	KEYANIMANAGER->update("wraith_Shadow");
	KEYANIMANAGER->update("clone");
	KEYANIMANAGER->update("clone_Shadow");
	KEYANIMANAGER->update("armadillo");
	KEYANIMANAGER->update("armadillo_Shadow");
	KEYANIMANAGER->update("minotaur");
	KEYANIMANAGER->update("minotaur_Shadow");
	KEYANIMANAGER->update("dragon");
	KEYANIMANAGER->update("dragon_Shadow");
	KEYANIMANAGER->update("bat_Boss");
	KEYANIMANAGER->update("bat_Boss_Shadow");
	KEYANIMANAGER->update("banshee");
	KEYANIMANAGER->update("banshee_Shadow");
	KEYANIMANAGER->update("horn");
	KEYANIMANAGER->update("keybord");
	KEYANIMANAGER->update("drum");
	KEYANIMANAGER->update("violin");
	KEYANIMANAGER->update("coralriff");


	KEYANIMANAGER->update("dust");
}

void enemyMgr::render()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		if (_vEnemy[i]->getEnemyType() == ZOMBIE || _vEnemy[i]->getEnemyType() == BAT || _vEnemy[i]->getEnemyType() == BAT_BOSS || _vEnemy[i]->getEnemyType() == ARMADILLO)
		{
			_vEnemy[i]->render();
		}
	}
}

void enemyMgr::createEnemy(int enemyType, POINT idx)
{
	enemy* enemy;
	switch (enemyType)
	{
		case ENEMY1_1_1:	// 검은박쥐
			enemy = new bat;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_1_2:	// 빨간박쥐
			enemy = new bat_2;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_2_1:	// 하얀해골
			enemy = new skeleton;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_2_2:	// 노란해골
			enemy = new skeleton_2;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_2_3:	// 까만해골
			enemy = new skeleton_3;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_3_1:	// 초록슬라임
			enemy = new slime;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_3_2:	// 파란슬라임
			enemy = new slime_Blue;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_1_1:	// 클론
			enemy = new clone;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_1_2:	// 레이스
			enemy = new wraith;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_2_1:	// 아르마딜로
			enemy = new armadillo;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_2_2:	// 좀비
			enemy = new zombie;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_1_1:	// 미노타우로스
			enemy = new minotaur;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_1_2:	// 용
			enemy = new dragon;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_2_1:	// 황금박쥐
			enemy = new bat_Boss;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_2_2:	// 밴쉬
			enemy = new banshee;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_1_1:	// 폰

		break;
		case ENEMY4_1_2:	// 비숍		//호른
			enemy = new horn;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_1_3:	// 룩		//바이올린
			enemy = new violin;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_2_1:	// 나이트	//드럼
			enemy = new drum;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_2_2:	// 퀸		//키보드
			enemy = new keybord;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_2_3:	// 킹		//베이스
			enemy = new coralriff;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
	}

}

bool enemyMgr::getIsThereEnemy(int x, int y)
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		if (_vEnemy[i]->getIdx().x == x && _vEnemy[i]->getIdx().y == y)
		{
			return true;
		}
	}
	return false;
}
