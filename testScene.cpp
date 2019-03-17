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
	_em->init();



	delete _mapLoader;
	_mapLoader = nullptr;
	return S_OK;
}

void testScene::release()
{
}

void testScene::update()
{
	tileUpdate();

	playerUpdate();
	
	_rayCast->rayCasting({ (int)_player->getPosCT().x / TILESIZE, (int)_player->getPosCT().y / TILESIZE }, 2);

	beatUpdate();
	
	_em->update();
	_em->setIsBeat(_isBeat);
}

void testScene::render()
{
	tileRender();

	objRender();

	lightMapRender();

	beatRender();

	textRender();

	_em->render();
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

	//적 이미지
	IMAGEMANAGER->addFrameImage("enemy1", L"images/mapTool/mob1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy2", L"images/mapTool/mob2.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy3", L"images/mapTool/mob3.png", 432, 576, 3, 4);
	IMAGEMANAGER->addFrameImage("enemy4", L"images/mapTool/mob4.png", 432, 576, 3, 6);

	//비트 이미지
	IMAGEMANAGER->addFrameImage("beat_Heart", L"images/ui/beat_Heart.png", 160, 100, 2, 1);
	IMAGEMANAGER->addImage("beat_Green", L"images/ui/beat_Green.png", 12, 64);
	IMAGEMANAGER->addImage("beat_Red", L"images/ui/beat_Red.png", 12, 64);

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
	_volume = 0.1f;
	_pitch = 1.0f;

	_mapLoader->beatInit(_stageKeyName, _beatFileName, _volume, &_vBeat);

	_heartImg = IMAGEMANAGER->findImage("beat_Heart");

	_rc_Correct = { (float)WINSIZEX / 2 - 80,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 80,
					(float)WINSIZEY - 52 };
	_rc_Wrong = { (float)WINSIZEX / 2 - 150,
					(float)WINSIZEY - 100,
					(float)WINSIZEX / 2 + 150,
					(float)WINSIZEY - 52 };


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

	if (_time  > _vBeat.begin()->beat)
	{
		_isBeat = true;
		_heartImg->SetFrameX(1);
		_vBeat.erase(_vBeat.begin());
	}

	RECT tmp;
	if (_player->getIsPressKey())
	{
		//if (_vBeat.begin()->beat - 200 < _time && _vBeat.begin()->beat + 100 > _time)
		if(IntersectRect(&tmp, &rectMake(_rc_Correct), &rectMake(_vBeat.begin()->rc_Left)))// && _vBeat.begin()->isRight == true)
		{
			_vBeat.begin()->isRight = false;
			_player->setIsBeat(true);
		}
		else if (IntersectRect(&tmp, &rectMake(_rc_Wrong), &rectMake(_vBeat.begin()->rc_Left)))// && _vBeat.begin()->isRight == true)
		{
			_vBeat.begin()->isRight = false;
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
			if (_vBeat[i].isRight == false)
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

//===========================================
//					render
//===========================================

void testScene::tileRender()
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

void testScene::objRender()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (_vvObj[i][j]->getIdx().x == _player->getIdx().x && _vvObj[i][j]->getIdx().y == _player->getIdx().y)
			{
				_player->render();
			}

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
			}
			else if (_vvObj[i][j]->getImg() != nullptr
				&& CAMERA->getPosX() - TILESIZE <= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() - TILESIZE <= _vvObj[i][j]->getPos().y
				&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvObj[i][j]->getPos().x
				&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvObj[i][j]->getPos().y
				&& _vvObj[i][j]->getImgNum() != IMG_WALL)
			{
				_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
					- CAMERA->getPosX(),
					_vvObj[i][j]->getRc().top
					- CAMERA->getPosY(),
					_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
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
				//if (_vvLightMap[i][j]->getIsFind() == false)
				//{
				//	D2DMANAGER->fillRectangle(_vvLightMap[i][j]->getBrush(),
				//		_vvLightMap[i][j]->getTileRc().left - CAMERA->getPosX(),
				//		_vvLightMap[i][j]->getTileRc().top - CAMERA->getPosY(),
				//		_vvLightMap[i][j]->getTileRc().right - CAMERA->getPosX(),
				//		_vvLightMap[i][j]->getTileRc().bottom - CAMERA->getPosY(),
				//		1.0f);
				//}
				//else if (_vvLightMap[i][j]->getIsFind() == true)
				//{
				//	D2DMANAGER->fillRectangle(_vvLightMap[i][j]->getBrush(),
				//		_vvLightMap[i][j]->getTileRc().left - CAMERA->getPosX(),
				//		_vvLightMap[i][j]->getTileRc().top - CAMERA->getPosY(),
				//		_vvLightMap[i][j]->getTileRc().right - CAMERA->getPosX(),
				//		_vvLightMap[i][j]->getTileRc().bottom - CAMERA->getPosY(),
				//		_vvLightMap[i][j]->getOpacity());
				//}

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
		if (_vBeat[i].rc_Left.left > 0)
		{
			IMAGEMANAGER->render("beat_Green", _vBeat[i].rc_Left.left, _vBeat[i].rc_Left.top,10,50,_vBeat[i].opacity);
			D2DMANAGER->drawRectangle(0x0000ff, _vBeat[i].rc_Left);
		}
		if (_vBeat[i].rc_Right.right < WINSIZEX)
		{
			IMAGEMANAGER->render("beat_Green", _vBeat[i].rc_Right.left, _vBeat[i].rc_Right.top,10,50, _vBeat[i].opacity);
			D2DMANAGER->drawRectangle(0xff0000, _vBeat[i].rc_Right);
		}
	}
	_heartImg->frameRender(WINSIZEX / 2 - _heartImg->GetFrameWidth() / 2, WINSIZEY - 125,_heartImg->GetFrameX(),_heartImg->GetFrameY());
	 D2DMANAGER->drawRectangle(0xff0000, _rc_Correct);
	 D2DMANAGER->drawRectangle(0x0000ff, _rc_Wrong);
	//IMAGEMANAGER->frameRender("beat_Heart", WINSIZEX / 2 - IMAGEMANAGER->findImage("beat_Heart")->GetFrameWidth() / 2, WINSIZEY - 125, 0, 0);

}

void testScene::textRender()
{
	WCHAR str[128];
	swprintf_s(str, L"isChanged : %d", _vBeat.size());
	D2DMANAGER->drawText(str, 300, 260, 20, 0x00ffff);
}

//===========================================
//					get
//===========================================


//===========================================
//					set
//===========================================





