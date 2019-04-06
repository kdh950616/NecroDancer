#include "stdafx.h"
#include "stage2.h"


stage2::stage2()
{
}


stage2::~stage2()
{
}

HRESULT stage2::init()
{

	mapLoaderInit();

	lightMapInit();

	playerInit();

	rayCastInit();

	beatInit();

	_em = new enemyMgr;
	_mapLoader->linkenemyMgr(_em);
	_mapLoader->makeEnemy(&_vvObj, &_tileSizeX, &_tileSizeY);
	_em->linkPlayer(_player);
	_em->linkMap(&_vvMap);
	_em->linkObj(&_vvObj);
	_em->linkLightMap(&_vvLightMap);
	_em->linkTime(&_time);
	_em->init();
	_em->linkStageKeyName(&_stageKeyName);

	//이거 링크 순서 핵중요
	_player->linkEnemyMgr(_em);

	delete _mapLoader;
	_mapLoader = nullptr;

	_torchCompare = (_vBeat[1].beat - _vBeat[0].beat) / 4;

	_itemPosY = 0;

	SOUNDMANAGER->ShopVolume(_stageKeyName, 0);
	_aStar = new aStar;
	_aStar->setMap(&_vvObj);
	_lShopVol.clear();

	return S_OK;
}

void stage2::release()
{
}

void stage2::update()
{
	tileUpdate();

	playerUpdate();

	_rayCast->checkMap({ (int)_player->getPosCT().x / TILESIZE, (int)_player->getPosCT().y / TILESIZE }, (*_player).getTorchPower(), _tileSizeX, _tileSizeY);
	_rayCast->rayCasting({ (int)_player->getPosCT().x / TILESIZE, (int)_player->getPosCT().y / TILESIZE }, (*_player).getTorchPower());

	beatUpdate();

	_em->update();
	_em->setIsBeat(_isBeat);

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_showText == true ? _showText = false : _showText = true;
	}

	itemFloating();

	if (_player->getIdx().x == 29 && _player->getIdx().y == 4 && _player->getIsMove() == false)
	{
		SOUNDMANAGER->stop(_stageKeyName);
		SCENEMANAGER->changeScene("lobbyScene");
		//SCENEMANAGER->changeScene("bossScene");
	}
}

void stage2::render()
{
	tileRender();

	objRender();

	_em->render();

	lightMapRender();

	beatRender();

	textRender();

	_player->txtRender();
	_player->effectRender();
	_player->renderUI();
	_player->heartRender();
}

void stage2::mapLoaderInit()
{
	_mapLoader = new mapLoader;

	_mapLoader->mapLoad(&_vvMap, &_vvObj, &_tileSizeX, &_tileSizeY, &_isBeat, 2);
}

void stage2::lightMapInit()
{
	_vvLightMap.clear();

	for (int i = 0; i < _tileSizeY; i++)
	{
		vLightLine lightLine;
		lightLine.clear();
		for (int j = 0; j < _tileSizeX; j++)
		{
			lightMap* lightTile = new lightMap;
			lightTile->setLightMap(j, i);
			lightLine.push_back(lightTile);
		}
		_vvLightMap.push_back(lightLine);
	}
}

void stage2::playerInit()
{
	_player = new player;
	_player->setMapAdressLink(&_vvMap);
	_player->setObjAdressLink(&_vvObj);
	_player->setPosCT({ (float)(_vvMap)[2][2]->getPos().x, (float)(_vvMap)[2][2]->getPos().y });
	_player->init();
}

void stage2::rayCastInit()
{
	_rayCast = new rayCast;
	_rayCast->setLightMapAdress(&_vvLightMap);
	_rayCast->setobjAdress(&_vvObj);
}

void stage2::beatInit()
{
	//_stageKeyName = "mapTool";
	_stageKeyName = "stage1";
	_beatFileName = "sounds/zone/zone1_1.txt";
	SOUNDMANAGER->setVolume(VOLUME);
	_pitch = 1.0f;


	_mapLoader->beatInit(_stageKeyName, _beatFileName, SOUNDMANAGER->getVolume(), &_vBeat);

	_heartImg = IMAGEMANAGER->findImage("beat_Heart");

	_rc_Correct = { (float)WINSIZEX / 2 - 150,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 150,
					(float)WINSIZEY - 52 };
	_rc_Wrong = { (float)WINSIZEX / 2 - 170,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 170,
					(float)WINSIZEY - 52 };

	SOUNDMANAGER->setEffectVol(VOLUME_EFF);
}

void stage2::tileUpdate()
{
	if (_isBeat)
	{
		for (int i = 0; i < _tileSizeY; i++)
		{
			for (int j = 0; j < _tileSizeX; j++)
			{
				_vvMap[i][j]->update();
				if (_vvObj[i][j]->getAttribute() == ETC_SHOPKEEPER)
				{
					_aStar->pathFinderShopkeeper({ j,i }, { _player->getIdx().x, _player->getIdx().y }, { j,i }, *(getShopVol()));

					if (_lShopVol.size() <= 5)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 10);
					}
					else if (_lShopVol.size() <= 6)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 9);
					}
					else if (_lShopVol.size() <= 7)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 8);
					}
					else if (_lShopVol.size() <= 8)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 7);
					}
					else if (_lShopVol.size() <= 9)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 6);
					}
					else if (_lShopVol.size() <= 10)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 5);
					}
					else if (_lShopVol.size() <= 11)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 4);
					}
					else if (_lShopVol.size() <= 12)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 3);
					}
					else if (_lShopVol.size() <= 13)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 2);
					}
					else if (_lShopVol.size() <= 14)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 1);
					}
					else if (_lShopVol.size() <= 15)
					{
						SOUNDMANAGER->ShopVolume(_stageKeyName, VOLUME / 10 * 0);
					}
				}
			}
		}
		_isBeat = false;
	}
}

void stage2::playerUpdate()
{
	_player->update();

	if (_player->getIsArrive() == true)
	{
		//움직임걸자
		_player->setIsArrive(false);
	}
}

void stage2::beatUpdate()
{
	_time = SOUNDMANAGER->getPosition(_stageKeyName);

	//횃불 프레임 변경해주는거
	if (_vBeat.begin()->beat - _time <= _torchCompare)
	{
		_torchFrameY = 3;
	}
	else if (_vBeat.begin()->beat - _time <= _torchCompare * 2)
	{
		_torchFrameY = 2;
	}
	else if (_vBeat.begin()->beat - _time <= _torchCompare * 3)
	{
		_torchFrameY = 1;
	}
	else if (_vBeat.begin()->beat - _time <= _torchCompare * 4)
	{
		_torchFrameY = 0;
	}

	if (_heartImg->GetFrameX() == 1)
	{
		_heartImg->SetFrameX(0);
	}

	if (_time > _vBeat.begin()->beat)
	{
		(*_player).bounceHeart();

		_isBeat = true;
		_heartImg->SetFrameX(1);
		_vBeat.erase(_vBeat.begin());

		if ((*_player).getGrooveChain() > 1)
		{
			if (_tileBlinkX == 0)
			{
				_tileBlinkX = 1;
			}
			else if (_tileBlinkX == 1)
			{
				_tileBlinkX = 2;
			}
			else if (_tileBlinkX == 2)
			{
				_tileBlinkX = 1;
			}
		}
		else
		{
			_tileBlinkX = 0;
		}

		if ((*_player).getIsSlow())
		{
			_trapCount++;

			if (_trapCount >= 15)
			{
				(*_player).setIsSlow(false);
				SOUNDMANAGER->setPitch(1);
				_vvObj[(*_player).getTrapIdx().y][(*_player).getTrapIdx().x]->setFrameX(_vvObj[(*_player).getTrapIdx().y][(*_player).getTrapIdx().x]->getFrameX() - 1);
				_trapCount = 0;
			}
		}
		if ((*_player).getIsFast())
		{
			_trapCount++;

			if (_trapCount >= 20)
			{
				(*_player).setIsFast(false);
				SOUNDMANAGER->setPitch(1);
				_vvObj[(*_player).getTrapIdx().y][(*_player).getTrapIdx().x]->setFrameX(_vvObj[(*_player).getTrapIdx().y][(*_player).getTrapIdx().x]->getFrameX() - 1);
				_trapCount = 0;
			}
		}
	}

	RECT tmp;
	if (_player->getIsPressKey())
	{
		//비트 맞으면
		if (IntersectRect(&tmp, &rectMake(_rc_Correct), &rectMake(_vBeat.begin()->rc_Left)) && _vBeat.begin()->isRight == true)
		{
			_vBeat.begin()->isRight = false;
			_player->setIsBeat(true);
		}
		//비트 틀리면
		else if (IntersectRect(&tmp, &rectMake(_rc_Wrong), &rectMake(_vBeat.begin()->rc_Left)) && _vBeat.begin()->isRight == true)
		{
			_vBeat.begin()->isRight = false;
			(*_player).setKillCombo(0);

			if ((*_player).getGrooveChain() > 1)
			{
				SOUNDMANAGER->playEff("grooveChainFail");
			}
			else
			{
				SOUNDMANAGER->playEff("missBeat");
			}
			//여기에 비트 틀릴때 사운드 넣어주자
		}
	}


	if (_vBeat.size() > 0)
	{
		for (int i = 0; i < _vBeat.size(); i++)
		{
			if (_vBeat[i].isRight == true)
			{
				_vBeat[i].rc_Left = { (float)(WINSIZEX / 2) - (_vBeat[i].beat - _time) / 3, (float)WINSIZEY - 100, (float)(WINSIZEX / 2) - (_vBeat[i].beat - _time) / 3 + 10, (float)WINSIZEY - 52 };
				_vBeat[i].rc_Right = { (float)(WINSIZEX / 2) + (_vBeat[i].beat - _time) / 3, (float)WINSIZEY - 100, (float)(WINSIZEX / 2) + (_vBeat[i].beat - _time) / 3 + 10, (float)WINSIZEY - 52 };
				if (_vBeat[i].rc_Left.left > 0)
				{
					if (_vBeat[i].opacity < 1.0f)
					{
						_vBeat[i].opacity += 0.08f;
					}
				}
			}
			else if (_vBeat[i].isRight == false)
			{
				_vBeat[i].opacity -= 0.2f;
			}
		}
	}
	else if (_vBeat.size() <= 0)
	{
		//비트 새로 불러오기
		//다음스테이지 or 게임종료
		_time = 0;
		SOUNDMANAGER->loadBeat(_beatFileName, _stageKeyName);
		_vBeat = SOUNDMANAGER->getVBeat();
	}
}

void stage2::itemFloating()
{
	if (_isNeedIncress) //증가해야하면
	{
		if (_itemPosY >= 5.0f)
		{
			_isNeedIncress = false;
		}
		_itemPosY += 0.2f;
	}
	else if (!_isNeedIncress) //감소해야하면
	{
		if (_itemPosY <= -5.0f)
		{
			_isNeedIncress = true;
		}
		_itemPosY -= 0.2f;
	}
}

void stage2::tileRender()
{
	//골드배수가 1과 같거나 작으면 반짝이지 않음
	if ((*_player).getGrooveChain() <= 1)
	{
		for (int i = 0; i < _tileSizeY; i++)
		{
			for (int j = 0; j < _tileSizeX; j++)
			{
				if (_vvMap[i][j]->getImg() != nullptr
					&& CAMERA->getPosX() - TILESIZE <= _vvMap[i][j]->getPos().x
					&& CAMERA->getPosY() - TILESIZE <= _vvMap[i][j]->getPos().y
					&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvMap[i][j]->getPos().x
					&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvMap[i][j]->getPos().y)
				{
					_vvMap[i][j]->getImg()->frameRender(_vvMap[i][j]->getRc().left
						- CAMERA->getPosX(),
						_vvMap[i][j]->getRc().top
						- CAMERA->getPosY(),
						_vvMap[i][j]->getFrameX(), _vvMap[i][j]->getFrameY());
				}
			}
		}
	}
	//골드배수가 1보다 크면 타일이 반짝임
	else
	{
		for (int i = 0; i < _tileSizeY; i++)
		{
			for (int j = 0; j < _tileSizeX; j++)
			{
				if (_vvMap[i][j]->getImg() != nullptr
					&& CAMERA->getPosX() - TILESIZE <= _vvMap[i][j]->getPos().x
					&& CAMERA->getPosY() - TILESIZE <= _vvMap[i][j]->getPos().y
					&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvMap[i][j]->getPos().x
					&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvMap[i][j]->getPos().y)
				{
					//타일의 정보가 땅 or 보스땅이고 프레임x가 4면 기존 프레임 x에 타일업데이트에서 구한 tileBlinkX를 더한것을출력함.
					if ((_vvMap[i][j]->getAttribute() == TILE_GROUND || _vvMap[i][j]->getAttribute() == TILE_BOSS_GROUND)
						&& _vvMap[i][j]->getFrameX() == 4)
					{
						_vvMap[i][j]->getImg()->frameRender(_vvMap[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvMap[i][j]->getRc().top
							- CAMERA->getPosY(),
							_vvMap[i][j]->getImg()->GetFrameWidth(),
							_vvMap[i][j]->getImg()->GetFrameHeight(),
							_vvMap[i][j]->getFrameX() + _tileBlinkX, _vvMap[i][j]->getFrameY());
					}
					//그게 아니면 그냥 출력
					else
					{
						_vvMap[i][j]->getImg()->frameRender(_vvMap[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvMap[i][j]->getRc().top
							- CAMERA->getPosY(),
							_vvMap[i][j]->getFrameX(), _vvMap[i][j]->getFrameY());
					}
				}
			}
		}
	}
}

void stage2::objRender()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (_vvObj[i][j]->getImg() != nullptr
				&& CAMERA->getPosX() - TILESIZE <= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() - TILESIZE <= _vvObj[i][j]->getPos().y
				&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvObj[i][j]->getPos().y &&
				_vvObj[i][j]->getAttribute() == ETC_TORCH || (_vvObj[i][j]->getAttribute() >= ETC_CHEST && _vvObj[i][j]->getAttribute() <= ETC_SHOPKEEPER))
			{
				switch (_vvObj[i][j]->getAttribute())
				{
				case ETC_TORCH:
					_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
						- CAMERA->getPosX(),
						_vvObj[i][j]->getRc().top*
						-CAMERA->getPosY(),
						_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY() + _torchFrameY);
					break;
				case ETC_CHEST: case ETC_TRAPDOOR:	case ETC_SLOW:	case ETC_FAST:
					if (_vvLightMap[i][j]->getOpacity() < 0.64f)
					{
						_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvObj[i][j]->getRc().top
							- CAMERA->getPosY(),
							_vvObj[i][j]->getImg()->GetFrameWidth(),
							_vvObj[i][j]->getImg()->GetFrameHeight(),
							_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
					}
					else
					{
						_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvObj[i][j]->getRc().top
							- CAMERA->getPosY(),
							_vvObj[i][j]->getImg()->GetFrameWidth(),
							_vvObj[i][j]->getImg()->GetFrameHeight(),
							_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY() + 1);
					}
					break;
				case ETC_SHOPKEEPER:
					_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
						- CAMERA->getPosX() - TILESIZE / 2,
						_vvObj[i][j]->getRc().top
						- CAMERA->getPosY() - TILESIZE,
						_vvObj[i][j]->getFrameX() + _torchFrameY, _vvObj[i][j]->getFrameY());
					break;
				}
			}

			if (_vvObj[i][j]->getIdx().x == _player->getIdx().x && _vvObj[i][j]->getIdx().y == _player->getIdx().y)
			{
				_player->render();
			}

			if (_vvObj[i][j]->getIsAvailMove() == false && _vvObj[i][j]->getAttribute() == OBJ_NONE && _vvObj[i][j]->getImgNum() == IMG_NONE)
			{
				for (int k = 0; k < _em->getVEnemy().size(); k++)
				{
					if (_vvObj[i][j]->getIdx().x == _em->getVEnemy()[k]->getIdx().x && _vvObj[i][j]->getIdx().y == _em->getVEnemy()[k]->getIdx().y)
					{
						_em->getVEnemy()[k]->render();
						break;
					}
				}
			}

			//벽이면 일정수치 위로 띄워서 출력
			if (_vvObj[i][j]->getImg() != nullptr
				&& CAMERA->getPosX() - TILESIZE <= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() - TILESIZE <= _vvObj[i][j]->getPos().y
				&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvObj[i][j]->getPos().y
				&& _vvObj[i][j]->getImgNum() == IMG_WALL)
			{
				_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
					- CAMERA->getPosX(),
					_vvObj[i][j]->getRc().top - TILESIZE
					- CAMERA->getPosY(),
					_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());

				if (_vvObj[i][j]->getAttribute() >= ETC_TORCH_WALL1 && _vvObj[i][j]->getAttribute() <= ETC_TORCH_BOSS)
				{
					IMAGEMANAGER->frameRender("etc1", _vvObj[i][j]->getRc().left - CAMERA->getPosX(), _vvObj[i][j]->getRc().top - CAMERA->getPosY() - TILESIZE, 1, _torchFrameY);
				}
			}
			//오브젝트가 이미지는 있는데 벽이 아니면
			else if (_vvObj[i][j]->getImg() != nullptr
				&& CAMERA->getPosX() - TILESIZE <= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() - TILESIZE <= _vvObj[i][j]->getPos().y
				&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvObj[i][j]->getPos().y
				&& _vvObj[i][j]->getImgNum() != IMG_WALL)
			{
				//아이템이면
				if (_vvObj[i][j]->getAttribute() > ITEM_START && _vvObj[i][j]->getAttribute() < ITEM_END)
				{
					//플레이어의 시야범위 안에 있다면 
					if (_vvLightMap[i][j]->getOpacity() < 0.64f)
					{
						_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvObj[i][j]->getRc().top
							- CAMERA->getPosY() - TILESIZE / 2 + _itemPosY,
							_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());

						IMAGEMANAGER->findImage("shadow_Standard")->render(_vvObj[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvObj[i][j]->getRc().top
							- CAMERA->getPosY() - 14);
					}
					//플레이어의 시야범위 밖에 있다면
					else
					{
						_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvObj[i][j]->getRc().top
							- CAMERA->getPosY() - TILESIZE / 2 + _itemPosY,
							_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY() + 1);

						IMAGEMANAGER->findImage("shadow_Standard")->render(_vvObj[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvObj[i][j]->getRc().top
							- CAMERA->getPosY() - 14);
					}
				}
			}
		}
	}
}

void stage2::lightMapRender()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (CAMERA->getPosX() - TILESIZE <= j * TILESIZE
				&&CAMERA->getPosY() - TILESIZE <= i * TILESIZE
				&&CAMERA->getPosX() + TILESIZE + WINSIZEX >= j * TILESIZE
				&&CAMERA->getPosY() + TILESIZE + WINSIZEY >= i * TILESIZE)
			{
				if (_vvLightMap[i][j]->getIsFind() == false)
				{
					IMAGEMANAGER->render("black", j * TILESIZE - CAMERA->getPosX(), i*TILESIZE - CAMERA->getPosY(), 1.0f);
				}
				if (_vvLightMap[i][j]->getIsFind() == true)
				{
					IMAGEMANAGER->render("black", j * TILESIZE - CAMERA->getPosX(), i*TILESIZE - CAMERA->getPosY(), _vvLightMap[i][j]->getOpacity());
				}
				if (_vvLightMap[i][j]->getIdx().y == _player->getIdx().y - 1 && _vvLightMap[i][j]->getIdx().x == _player->getIdx().x)
				{
					if (_player->getIsReverse() == false)
					{
						_player->getHeadImg()->aniRender(_player->getPosLT().x - CAMERA->getPosX(), _player->getPosLT().y - 15 - _player->getPosZ() - CAMERA->getPosY(), _player->getHeadAni());
					}
					else if (_player->getIsReverse() == true)
					{
						_player->getHeadImg()->aniRenderReverseX(_player->getPosLT().x - CAMERA->getPosX(), _player->getPosLT().y - 15 - _player->getPosZ() - CAMERA->getPosY(), _player->getHeadAni());
					}
				}
			}
		}
	}
}

void stage2::beatRender()
{
	for (int i = 0; i < _vBeat.size(); i++)
	{
		if (_vBeat[i].rc_Left.left > 0 && _vBeat[i].rc_Left.left < WINSIZEX / 2)
		{
			if (_vBeat.back().beat - 30000 > _vBeat[i].beat)
			{
				IMAGEMANAGER->render("beat_Green", _vBeat[i].rc_Left.left, _vBeat[i].rc_Left.top, 10, 50, _vBeat[i].opacity);
				IMAGEMANAGER->render("beat_Green", _vBeat[i].rc_Right.left, _vBeat[i].rc_Right.top, 10, 50, _vBeat[i].opacity);
				//D2DMANAGER->drawRectangle(0x0000ff, _vBeat[i].rc_Left);
				//D2DMANAGER->drawRectangle(0xff0000, _vBeat[i].rc_Right);
			}
			else
			{
				IMAGEMANAGER->render("beat_Red", _vBeat[i].rc_Left.left, _vBeat[i].rc_Left.top, 10, 50, _vBeat[i].opacity);
				IMAGEMANAGER->render("beat_Red", _vBeat[i].rc_Right.left, _vBeat[i].rc_Right.top, 10, 50, _vBeat[i].opacity);
			}
		}
	}
	_heartImg->frameRender(WINSIZEX / 2 - _heartImg->GetFrameWidth() / 2, WINSIZEY - 125, _heartImg->GetFrameX(), _heartImg->GetFrameY());
	D2DMANAGER->drawRectangle(0xff0000, _rc_Correct);
	D2DMANAGER->drawRectangle(0x0000ff, _rc_Wrong);
	//IMAGEMANAGER->frameRender("beat_Heart", WINSIZEX / 2 - IMAGEMANAGER->findImage("beat_Heart")->GetFrameWidth() / 2, WINSIZEY - 125, 0, 0);

	 //코인배수
	IMAGEMANAGER->render("grooveChain", WINSIZEX / 2 - 40, WINSIZEY - 20);
	//코인배수 숫자
	WCHAR str[128];
	swprintf_s(str, L"%d", (*_player).getGrooveChain());
	if ((*_player).getGrooveChain() > 2)
	{
		D2DMANAGER->drawText(str, WINSIZEX / 2 + 45, WINSIZEY - 20, 15, 0xff0000);
	}
	else
	{
		D2DMANAGER->drawText(str, WINSIZEX / 2 + 45, WINSIZEY - 20, 15, 0xffffff);
	}
}

void stage2::textRender()
{
}
