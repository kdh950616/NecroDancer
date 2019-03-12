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
	tileInit();

	_player = new player;
	_player->setMapAdressLink(&_vvMap);
	_player->setObjAdressLink(&_vvObj);
	_player->init();

	_rayCast = new rayCast;
	_rayCast->setLightMapAdress(&_vvLightMap);
	_rayCast->setobjAdress(&_vvObj);
	

	return S_OK;
}

void testScene::release()
{
}

void testScene::update()
{
	tileUpdate();

	_player->update();

	if (_player->getIsMove() == false)
	{
	}
	_rayCast->rayCasting({(int)_player->getPosCT().x / TILESIZE, (int)_player->getPosCT().y / TILESIZE}, 2);

}

void testScene::render()
{
	tileRender();
}


//===========================================
//					init
//===========================================

void testScene::tileInit()
{
	//타일들 이미지
	IMAGEMANAGER->addFrameImage("tile", L"images/mapTool/tile.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("wall", L"images/mapTool/wall.png", 432, 576, 9, 6);

	//적 이미지
	IMAGEMANAGER->addFrameImage("enemy1", L"images/mapTool/mob1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy2", L"images/mapTool/mob2.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy3", L"images/mapTool/mob3.png", 432, 576, 3, 4);
	IMAGEMANAGER->addFrameImage("enemy4", L"images/mapTool/mob4.png", 432, 576, 3, 6);

	_mapLoader = new mapLoader;

	_mapLoader->mapLoad(&_vvMap, &_vvObj, &_tileSizeX, &_tileSizeY, &_isTileChanged, 0);

	delete _mapLoader;
	_mapLoader = nullptr;

	_tileCount = 0;

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

//===========================================
//					update
//===========================================

void testScene::tileUpdate()
{
	_tileCount++;

	if (_tileCount >= 50)
	{
		for (int i = 0; i < _tileSizeY; i++)
		{
			for (int j = 0; j < _tileSizeX; j++)
			{
				_vvMap[i][j]->update();
			}
		}
		_tileCount = 0;
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

	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (_vvMap[i][j]->getIdx().x == _player->getIdx().x && _vvObj[i][j]->getIdx().y == _player->getIdx().y)
			{
				_player->render();
			}

			if (_vvObj[i][j]->getImg() != nullptr
				&& CAMERA->getPosX() - TILESIZE <= _vvMap[i][j]->getPos().x
				&& CAMERA->getPosY() - TILESIZE <= _vvMap[i][j]->getPos().y
				&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvMap[i][j]->getPos().x
				&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvMap[i][j]->getPos().y
				&& _vvObj[i][j]->getImgNum() == IMG_WALL)
			{
				_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left 
					- CAMERA->getPosX(),
													_vvObj[i][j]->getRc().top - TILESIZE 
					- CAMERA->getPosY(),
					_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
			}
			else if (_vvObj[i][j]->getImg() != nullptr
				&& CAMERA->getPosX() - TILESIZE <= _vvMap[i][j]->getPos().x
				&& CAMERA->getPosY() - TILESIZE <= _vvMap[i][j]->getPos().y
				&& CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvMap[i][j]->getPos().x
				&& CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvMap[i][j]->getPos().y
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
	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (CAMERA->getPosX() - TILESIZE <= _vvLightMap[i][j]->getPos().x
			  &&CAMERA->getPosY() - TILESIZE <= _vvLightMap[i][j]->getPos().y
			  &&CAMERA->getPosX() + TILESIZE + WINSIZEX >= _vvLightMap[i][j]->getPos().x
			  &&CAMERA->getPosY() + TILESIZE + WINSIZEY >= _vvLightMap[i][j]->getPos().y)
			{
				if (_vvLightMap[i][j]->getIsFind() == false)
				{
					D2DMANAGER->fillRectangle(_vvLightMap[i][j]->getBrush(),
						_vvLightMap[i][j]->getRc().left - CAMERA->getPosX(),
						_vvLightMap[i][j]->getRc().top - CAMERA->getPosY(),
						_vvLightMap[i][j]->getRc().right - CAMERA->getPosX(),
						_vvLightMap[i][j]->getRc().bottom - CAMERA->getPosY(),
						1.0f);
				}
				else if (_vvLightMap[i][j]->getIsFind() == true)
				{
					D2DMANAGER->fillRectangle(_vvLightMap[i][j]->getBrush(),
						_vvLightMap[i][j]->getRc().left - CAMERA->getPosX(),
						_vvLightMap[i][j]->getRc().top - CAMERA->getPosY(),
						_vvLightMap[i][j]->getRc().right - CAMERA->getPosX(),
						_vvLightMap[i][j]->getRc().bottom - CAMERA->getPosY(),
						_vvLightMap[i][j]->getOpacity());
				}
			}
		}
	}

			EFFECTMANAGER->render();
}

//===========================================
//					get
//===========================================


//===========================================
//					set
//===========================================





