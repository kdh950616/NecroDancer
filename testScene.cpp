#include "stdafx.h"
#include "testScene.h"


testScene::testScene()
{
}


testScene::~testScene()
{
}

HRESULT testScene::init()
{
	imageInit();

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

	_itemPosY = 0;
	return S_OK;
}

void testScene::release()
{
}

void testScene::update()
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
}

void testScene::render()
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
}


//===========================================
//					init
//===========================================

void testScene::imageInit()
{
	IMAGEMANAGER->addImage("black", L"images/mapTool/black.png", 48, 48);

	//타일들 이미지
	IMAGEMANAGER->addFrameImage("tile", L"images/mapTool/tile.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("wall", L"images/mapTool/wall.png", 432, 576, 9, 6);
	IMAGEMANAGER->addFrameImage("item", L"images/mapTool/item.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("etc1", L"images/mapTool/etc1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("etc2", L"images/mapTool/etc2.png", 432, 576, 3, 6);

	//적 이미지
	IMAGEMANAGER->addFrameImage("enemy1", L"images/mapTool/mob1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy2", L"images/mapTool/mob2.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy3", L"images/mapTool/mob3.png", 432, 576, 3, 4);
	IMAGEMANAGER->addFrameImage("enemy4", L"images/mapTool/mob4.png", 432, 576, 3, 6);

	//비트 이미지
	IMAGEMANAGER->addFrameImage("beat_Heart", L"images/ui/beat_Heart.png", 160, 100, 2, 1);
	IMAGEMANAGER->addImage("beat_Green", L"images/ui/beat_Green.png", 12, 64);
	IMAGEMANAGER->addImage("beat_Red", L"images/ui/beat_Red.png", 12, 64);
	//코인배수
	IMAGEMANAGER->addImage("grooveChain", L"images/ui/game_coinmultiplier.png", 81, 18);
	IMAGEMANAGER->addFrameImage("number", L"images/ui/number.png", 40, 18, 4, 1);

	//아이템용 그림자
	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
}

void testScene::mapLoaderInit()
{
	_mapLoader = new mapLoader;

	_mapLoader->mapLoad(&_vvMap, &_vvObj, &_tileSizeX, &_tileSizeY, &_isBeat, 0);
}

void testScene::lightMapInit()
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

void testScene::playerInit()
{
	_player = new player;
	_player->setMapAdressLink(&_vvMap);
	_player->setObjAdressLink(&_vvObj);
	_player->setPosCT({ (float)(_vvMap)[10][10]->getPos().x, (float)(_vvMap)[10][10]->getPos().y });
	_player->init();
}

void testScene::rayCastInit()
{
	_rayCast = new rayCast;
	_rayCast->setLightMapAdress(&_vvLightMap);
	_rayCast->setobjAdress(&_vvObj);
}

void testScene::beatInit()
{
	//_stageKeyName = "mapTool";
	_stageKeyName = "testScene";
	_beatFileName = "sounds/zone/zone1_1.txt";
	SOUNDMANAGER->setVolume(VOLUME);
	_pitch = 1.0f;
	

	_mapLoader->beatInit(_stageKeyName, _beatFileName, SOUNDMANAGER->getVolume(), &_vBeat);

	_heartImg = IMAGEMANAGER->findImage("beat_Heart");

	_rc_Correct = { (float)WINSIZEX / 2 - 80,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 80,
					(float)WINSIZEY - 52 };
	_rc_Wrong = { (float)WINSIZEX / 2 - 120,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 120,
					(float)WINSIZEY - 52 };

	SOUNDMANAGER->setEffectVol(VOLUME_EFF);
}

//===========================================
//					update
//===========================================

void testScene::tileUpdate()
{

	if (_isBeat)
	{
		for (int i = 0; i < _tileSizeY; i++)
		{
			for (int j = 0; j < _tileSizeX; j++)
			{
				_vvMap[i][j]->update();
			}
		}
		_isBeat = false;
	}
}

void testScene::playerUpdate()
{
	_player->update();

	if (_player->getIsArrive() == true)
	{
		//움직임걸자
		_player->setIsArrive(false);
	}
}

void testScene::beatUpdate()
{
	_time = SOUNDMANAGER->getPosition(_stageKeyName);

	if (_heartImg->GetFrameX() == 1)
	{
		_heartImg->SetFrameX(0);
	}

	//if (_vBeat.begin()->isRight == true && _time > _vBeat.begin()->beat)
	//{
	//	_isBeat = true;
	//	_heartImg->SetFrameX(1);
	//	_vBeat.erase(_vBeat.begin());
	//}
	//else if (_vBeat.begin()->isRight == false)
	//{
	//	if (_vBeat.begin()->opacity < 0.2f)
	//	{
	//		_vBeat.erase(_vBeat.begin());
	//	}
	//}

	if (_time - 120  > _vBeat.begin()->beat)
	{
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
	}

	RECT tmp;
	if (_player->getIsPressKey())
	{
		//비트 맞으면
		if(IntersectRect(&tmp, &rectMake(_rc_Correct), &rectMake(_vBeat.begin()->rc_Left)))
		{
			_vBeat.begin()->isRight = false;
			_player->setIsBeat(true);
		}
		//비트 틀리면
		else if (IntersectRect(&tmp, &rectMake(_rc_Wrong), &rectMake(_vBeat.begin()->rc_Left)))
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
		_time = 0;
		SOUNDMANAGER->loadBeat(_beatFileName, _stageKeyName);
	}
}

void testScene::itemFloating()
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

//===========================================
//					render
//===========================================

void testScene::tileRender()
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

void testScene::objRender()
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
				if (_vvObj[i][j]->getAttribute() == ETC_TORCH)
				{
					_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
						- CAMERA->getPosX(),
														_vvObj[i][j]->getRc().top
						- CAMERA->getPosY(),
														_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY() + _torchFrameY);
				}
				else if (_vvObj[i][j]->getAttribute() == ETC_SHOPKEEPER)
				{
					_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
						- CAMERA->getPosX() - TILESIZE,
						_vvObj[i][j]->getRc().top
						- CAMERA->getPosY() - TILESIZE,
						_vvObj[i][j]->getFrameX() + _torchFrameY, _vvObj[i][j]->getFrameY());
				}
				else
				{
					_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
						- CAMERA->getPosX(),
														_vvObj[i][j]->getRc().top
						- CAMERA->getPosY(),
														_vvObj[i][j]->getImg()->GetFrameWidth(),
														_vvObj[i][j]->getImg()->GetFrameHeight(),
														_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
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

void testScene::lightMapRender()
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

void testScene::beatRender()
{
	for (int i = 0; i < _vBeat.size(); i++)
	{
		if (_vBeat[i].rc_Left.left > 0 && _vBeat[i].rc_Left.left < WINSIZEX / 2)
		{
			IMAGEMANAGER->render("beat_Green", _vBeat[i].rc_Left.left, _vBeat[i].rc_Left.top,10,50,_vBeat[i].opacity);
			D2DMANAGER->drawRectangle(0x0000ff, _vBeat[i].rc_Left);
			IMAGEMANAGER->render("beat_Green", _vBeat[i].rc_Right.left, _vBeat[i].rc_Right.top,10,50, _vBeat[i].opacity);
			D2DMANAGER->drawRectangle(0xff0000, _vBeat[i].rc_Right);
		}
	}
	_heartImg->frameRender(WINSIZEX / 2 - _heartImg->GetFrameWidth() / 2, WINSIZEY - 125,_heartImg->GetFrameX(),_heartImg->GetFrameY());
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

void testScene::textRender()
{
	WCHAR str[128];
	swprintf_s(str, L"isChanged : %d", _vBeat.size());
	D2DMANAGER->drawText(str, 300, 260, 20, 0x00ffff);
	swprintf_s(str, L"time : %d", _time);
	D2DMANAGER->drawText(str, 300, 800, 20, 0x00ffff);
	if (_showText)
	{
		for (int i = 0; i < _tileSizeY; i++)
		{
			for (int j = 0; j < _tileSizeX; j++)
			{
				if (!_vvObj[i][j]->getIsAvailMove())
				{
					swprintf_s(str, L"X");
					D2DMANAGER->drawText(str, (int)_vvObj[i][j]->getPos().x - CAMERA->getPosX(), (int)_vvObj[i][j]->getPos().y - CAMERA->getPosY(), 20, 0xff00ff);
				}


			}
		}
	}
}

//===========================================
//					get
//===========================================


//===========================================
//					set
//===========================================





