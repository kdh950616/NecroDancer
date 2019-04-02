#include "stdafx.h"
#include "stage1.h"


stage1::stage1()
{
}


stage1::~stage1()
{
}

HRESULT stage1::init()
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

	_torchCompare = (_vBeat[1].beat - _vBeat[0].beat) / 4;

	_itemPosY = 0;

	SOUNDMANAGER->ShopVolume(_stageKeyName, 0);
	_aStar = new aStar;
	_aStar->setMap(&_vvObj);
	_lShopVol.clear();

	return S_OK;
}

void stage1::release()
{
}

void stage1::update()
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

void stage1::render()
{
	tileRender();

	objRender();

	_em->render();

	//lightMapRender();

	beatRender();

	textRender();

	_player->txtRender();
	_player->effectRender();
	_player->renderUI();
}

void stage1::imageInit()
{
	IMAGEMANAGER->addImage("black", L"images/mapTool/black.png", 48, 48);

	//타일들 이미지
	IMAGEMANAGER->addFrameImage("tile", L"images/mapTool/tile.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("wall", L"images/mapTool/wall.png", 432, 576, 9, 6);
	IMAGEMANAGER->addFrameImage("item", L"images/mapTool/item.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("etc1", L"images/mapTool/etc1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("etc2", L"images/mapTool/etc2.png", 432, 576, 3, 6);
	IMAGEMANAGER->addFrameImage("shopkeeper", L"images/item/shopkeeper2.png", 752, 152, 8, 2);

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

void stage1::mapLoaderInit()
{
	_mapLoader = new mapLoader;

	_mapLoader->mapLoad(&_vvMap, &_vvObj, &_tileSizeX, &_tileSizeY, &_isBeat, 1);
}

void stage1::lightMapInit()
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

void stage1::playerInit()
{
	_player = new player;
	_player->setMapAdressLink(&_vvMap);
	_player->setObjAdressLink(&_vvObj);
	_player->setPosCT({ (float)(_vvMap)[10][10]->getPos().x, (float)(_vvMap)[10][10]->getPos().y });
	_player->init();
}

void stage1::rayCastInit()
{
	_rayCast = new rayCast;
	_rayCast->setLightMapAdress(&_vvLightMap);
	_rayCast->setobjAdress(&_vvObj);
}

void stage1::beatInit()
{
	_stageKeyName = "stage1";
	_beatFileName = "sounds/zone/zone1_1.txt";
	SOUNDMANAGER->setVolume(VOLUME);
	_pitch = 1.0f;


	_mapLoader->beatInit(_stageKeyName, _beatFileName, SOUNDMANAGER->getVolume(), &_vBeat);

	_heartImg = IMAGEMANAGER->findImage("beat_Heart");

	_rc_Correct = { (float)WINSIZEX / 2 - 100,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 100,
					(float)WINSIZEY - 52 };
	_rc_Wrong = { (float)WINSIZEX / 2 - 120,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 120,
					(float)WINSIZEY - 52 };

	SOUNDMANAGER->setEffectVol(VOLUME_EFF);
}

void stage1::tileUpdate()
{
}

void stage1::playerUpdate()
{
}

void stage1::beatUpdate()
{
}

void stage1::itemFloating()
{
}

void stage1::tileRender()
{
}

void stage1::objRender()
{
}

void stage1::lightMapRender()
{
}

void stage1::beatRender()
{
}

void stage1::textRender()
{
}
