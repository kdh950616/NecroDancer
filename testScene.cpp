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

	_tileCount = 0;

	return S_OK;
}

void testScene::release()
{
}

void testScene::update()
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

void testScene::render()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (_vvMap[i][j]->getImg() != nullptr)
			{
				_vvMap[i][j]->getImg()->frameRender(_vvMap[i][j]->getRc().left, _vvMap[i][j]->getRc().top,
					_vvMap[i][j]->getFrameX(), _vvMap[i][j]->getFrameY());
			}
		}
	}

	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (_vvObj[i][j]->getImg() != nullptr)
			{
				_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left, _vvObj[i][j]->getRc().top,
					_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
			}
		}
	}
}
