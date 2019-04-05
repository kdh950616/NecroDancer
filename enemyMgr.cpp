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


	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->init();
		_vEnemy[i]->linkPlayer(_player);
		_vEnemy[i]->linkMap(_vvMap);
		_vEnemy[i]->linkObj(_vvObj);
		_vEnemy[i]->linkLightMap(_vvLightMap);

		//������ ������ ����
		if (i == _vEnemy.size() - 1)
		{
			//���ʹ� �����ŭ ������ �ѹ� ����
			for (int j = 0; j < _vEnemy.size(); j++)
			{
				//���ʹ� Ÿ���� ���̽���
				if (_vEnemy[j]->getEnemyType() == WRAITH)
				{
					//��Ʈī��Ʈ �������� (15�� ~ �������� 30���� ����)
					_vEnemy[j]->setBeatCount(RND->getFromIntTo(15000,SOUNDMANAGER->getVBeat().back().beat - 30000));
					//�׽�Ʈ�� ��Ʈī��Ʈ ����
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
		if (_isBeat && 
			_vEnemy[i]->getPosLT().x >= CAMERA->getPosX() && _vEnemy[i]->getPosLT().x < CAMERA->getPosX() + WINSIZEX &&
			_vEnemy[i]->getPosLT().y >= CAMERA->getPosY() && _vEnemy[i]->getPosLT().y < CAMERA->getPosY() + WINSIZEY)
		{
			_vEnemy[i]->setIsBeat(_isBeat);
			if (_vEnemy[i]->getIsSpecialAct() == false && _vEnemy[i]->getIsNeedAstar() == true)
			{
				_vEnemy[i]->getListPath()->clear();
				if (_vEnemy[i]->getEnemyType() == MINOTAUR || _vEnemy[i]->getEnemyType() == DRAGON)
				{
					_aStar->pathFinder2(_vEnemy[i]->getIdx(), _player->getIdx(), _vEnemy[i]->getIdx(), *(_vEnemy[i]->getListPath()));
				}
				else if (_vEnemy[i]->getEnemyType() == QUEEN)
				{
					_aStar->pathFinder8(_vEnemy[i]->getIdx(), _player->getIdx(), _vEnemy[i]->getIdx(), *(_vEnemy[i]->getListPath()));
				}
				else
				{
					if (i == 4)
					{
						int abc = 0;
					}
					_aStar->pathFinder(_vEnemy[i]->getIdx(), _player->getIdx(), _vEnemy[i]->getIdx(), *(_vEnemy[i]->getListPath()));
				}

			}
			//���̽����� ����
			if (_vEnemy[i]->getEnemyType() == WRAITH && !_vEnemy[i]->getIsFind() && _vEnemy[i]->getBeatCount() <= *_time)
			{
				_vEnemy[i]->setIsFind(true);
				_vEnemy[i]->setBeatCount(0);
				SOUNDMANAGER->playEff("wraith_Cry");
			}

			if (_vEnemy[i]->getEnemyType() == PAWN && _vEnemy[i]->getIsSpecialAct())
			{
				POINT tmp = _vEnemy[i]->getIdx();
				_vEnemy.erase(_vEnemy.begin() + i);

				createEnemy(ENEMY4_2_2, tmp);
				_vEnemy.back()->init();
				_vEnemy.back()->linkPlayer(_player);
				_vEnemy.back()->linkMap(_vvMap);
				_vEnemy.back()->linkObj(_vvObj);
				_vEnemy.back()->linkLightMap(_vvLightMap);
				_vEnemy.back()->setIsFind(true);
			}

			if (_vEnemy[i]->getEnemyType() == KING && !_vEnemy[i]->getIsCastling() && _vEnemy[i]->getIsFind() == false)
			{
				if ((*_vvObj)[6][2]->getIsAvailMove() == true && (*_vvObj)[6][3]->getIsAvailMove() == true)
				{
					_vEnemy[i]->setIsCastling(true);
					_vEnemy[i]->setDirection({ -2,0 });
					for (int j = 0; j < _vEnemy.size(); j++)
					{
						if (_vEnemy[j]->getEnemyType() == ROOK && _vEnemy[j]->getIdx().x == 1 && _vEnemy[j]->getIdx().y == 6)
						{
							_vEnemy[j]->setIsCastling(true);
							_vEnemy[j]->setDirection({ 2,0 });

							for (int k = 0; k < _vEnemy.size(); k++)
							{
								if (_vEnemy[k]->getEnemyType() == ROOK && _vEnemy[k]->getIdx().x == 8 && _vEnemy[k]->getIdx().y == 6)
								{
									_vEnemy[k]->setIsFind(true);
									break;
								}
							}
							break;
						}
					
					}
				}
				else if ((*_vvObj)[6][5]->getIsAvailMove() == true && (*_vvObj)[6][6]->getIsAvailMove() == true&& (*_vvObj)[6][7]->getIsAvailMove() == true)
				{
					_vEnemy[i]->setIsCastling(true);
					_vEnemy[i]->setDirection({ 3,0 });
					for (int j = 0; j < _vEnemy.size(); j++)
					{
						if (_vEnemy[j]->getEnemyType() == ROOK && _vEnemy[j]->getIdx().x == 8 && _vEnemy[j]->getIdx().y == 6)
						{
							_vEnemy[j]->setIsCastling(true);
							_vEnemy[j]->setDirection({ -2,0 });

							for (int k = 0; k < _vEnemy.size(); k++)
							{
								if (_vEnemy[k]->getEnemyType() == ROOK && _vEnemy[k]->getIdx().x == 1 && _vEnemy[k]->getIdx().y == 6)
								{
									_vEnemy[k]->setIsFind(true);
									break;
								}
							}
							break;
						}
					}
				}

			}
			if (_vEnemy.size() == 1 && _vEnemy[0]->getEnemyType() == KING && _vEnemy[0]->getIsSpecialAct() == false)
			{
				_vEnemy[0]->setIsSpecialAct(true);
				SOUNDMANAGER->playEff("king_Cry");
			}
			else if (_vEnemy.size() == 1 && _vEnemy[0]->getEnemyType() == KING && _vEnemy[0]->getIsSpecialAct() == true)
			{
				_vEnemy[0]->getListPath()->clear();
				_aStar->pathFinder8(_vEnemy[i]->getIdx(), _player->getIdx(), _vEnemy[i]->getIdx(), *(_vEnemy[i]->getListPath()));
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
					rnd = RND->getFromIntTo(1, 4);

					switch (rnd)
					{
					case 1:
						SOUNDMANAGER->playEff("piece_death1");
						break;
					case 2:
						SOUNDMANAGER->playEff("piece_death2");
						break;
					case 3:
						SOUNDMANAGER->playEff("piece_death3");
						break;
					}
				break;
				case KNIGHT:	
					rnd = RND->getFromIntTo(1, 4);

					switch (rnd)
					{
					case 1:
						SOUNDMANAGER->playEff("piece_death1");
						break;
					case 2:
						SOUNDMANAGER->playEff("piece_death2");
						break;
					case 3:
						SOUNDMANAGER->playEff("piece_death3");
						break;
					}
				break;
				case BISHOP:
					rnd = RND->getFromIntTo(1, 4);

					switch (rnd)
					{
					case 1:
						SOUNDMANAGER->playEff("piece_death1");
						break;
					case 2:
						SOUNDMANAGER->playEff("piece_death2");
						break;
					case 3:
						SOUNDMANAGER->playEff("piece_death3");
						break;
					}
				break;
				case ROOK:	
					rnd = RND->getFromIntTo(1, 4);

					switch (rnd)
					{
					case 1:
						SOUNDMANAGER->playEff("piece_death1");
						break;
					case 2:
						SOUNDMANAGER->playEff("piece_death2");
						break;
					case 3:
						SOUNDMANAGER->playEff("piece_death3");
						break;
					}
				break;
				case QUEEN:	
					rnd = RND->getFromIntTo(1, 4);

					switch (rnd)
					{
					case 1:
						SOUNDMANAGER->playEff("queen_Death1");
						break;
					case 2:
						SOUNDMANAGER->playEff("queen_Death2");
						break;
					case 3:
						SOUNDMANAGER->playEff("queen_Death3");
						break;
					}
				break;
				case KING:		
					SOUNDMANAGER->playEff("king_Death");
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

			//�� �ٱ��̸�
			if (_vEnemy[i]->getCurHp() <= 0)
			{
				//�ڱ� �ֺ� �˻��ϴ� ����
				for (int j = -1; j <= 1; j++)
				{
					for (int k = -1; k <= 1; k++)
					{
						//���� ��������
						if ((*_vvObj).size() > _vEnemy[i]->getIdx().y + j && _vEnemy[i]->getIdx().y + j > 0 
							&& (*_vvObj)[0].size() > _vEnemy[i]->getIdx().x + k && _vEnemy[i]->getIdx().x + k > 0)
						{
							//�ֺ��༮�� �Ӽ��� ������ -> ���ϰ�츦 ���� 
							if ((*_vvObj)[_vEnemy[i]->getIdx().y + j][_vEnemy[i]->getIdx().x + k]->getAttribute() == OBJ_NONE)
							{
								//�ش� ������Ʈ true�� �ٲ���. -> �������鼭 ���ǵ���� �ʱ�ȭ�ϴ°���
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
	KEYANIMANAGER->update("pawn");
	KEYANIMANAGER->update("rook");
	KEYANIMANAGER->update("bishop");
	KEYANIMANAGER->update("knight");
	KEYANIMANAGER->update("king");



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
		if (_vEnemy[i]->getIsMove())
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
		case ENEMY1_1_1:	// ��������
			enemy = new bat;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_1_2:	// ��������
			enemy = new bat_2;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_2_1:	// �Ͼ��ذ�
			enemy = new skeleton;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_2_2:	// ����ذ�
			enemy = new skeleton_2;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_2_3:	// ��ذ�
			enemy = new skeleton_3;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_3_1:	// �ʷϽ�����
			enemy = new slime;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY1_3_2:	// �Ķ�������
			enemy = new slime_Blue;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_1_1:	// Ŭ��
			enemy = new clone;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_1_2:	// ���̽�
			enemy = new wraith;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_2_1:	// �Ƹ�������
			enemy = new armadillo;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY2_2_2:	// ����
			enemy = new zombie;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_1_1:	// �̳�Ÿ��ν�
			enemy = new minotaur;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_1_2:	// ��
			enemy = new dragon;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_2_1:	// Ȳ�ݹ���
			enemy = new bat_Boss;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY3_2_2:	// �꽬
			enemy = new banshee;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_1_1:	// ��
			enemy = new pawn;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_1_2:	// ���		//ȣ��
			enemy = new bishop;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_1_3:	// ��		//���̿ø�
			enemy = new rook;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_2_1:	// ����Ʈ	//�巳
			enemy = new knight;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_2_2:	// ��		//Ű����
			enemy = new queen;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
		case ENEMY4_2_3:	// ŷ		//���̽�
			enemy = new king;
			enemy->makeEnemyIdx(idx);
			_vEnemy.push_back(enemy);
		break;
	}

}

void enemyMgr::createPawn(int enemyType, POINT idx, int pawnBeat)
{
	enemy* enemy;
	switch (enemyType)
	{
	case ENEMY4_1_1:	// ��
		enemy = new pawn;
		enemy->makeEnemyIdx(idx);
		enemy->setBeatCount(pawnBeat);
		_vEnemy.push_back(enemy);
		break;
	case ENEMY4_1_2:	// ���		//ȣ��
		//enemy = new horn;
		//enemy->makeEnemyIdx(idx);
		//_vEnemy.push_back(enemy);
		break;
	case ENEMY4_1_3:	// ��		//���̿ø�
		//enemy = new violin;
		//enemy->makeEnemyIdx(idx);
		//_vEnemy.push_back(enemy);
		break;
	case ENEMY4_2_1:	// ����Ʈ	//�巳
		//enemy = new drum;
		//enemy->makeEnemyIdx(idx);
		//_vEnemy.push_back(enemy);
		break;
	case ENEMY4_2_2:	// ��		//Ű����
		//enemy = new keybord;
		//enemy->makeEnemyIdx(idx);
		//_vEnemy.push_back(enemy);
		break;
	case ENEMY4_2_3:	// ŷ		//���̽�
		//enemy = new coralriff;
		//enemy->makeEnemyIdx(idx);
		//_vEnemy.push_back(enemy);
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
