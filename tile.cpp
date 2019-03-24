#include "stdafx.h"
#include "tile.h"


tile::tile()
{
}


tile::~tile()
{
}

HRESULT tile::init()
{
	return S_OK;
}

void tile::release()
{
}

void tile::update()
{
	if (getAttribute() == TILE_GROUND)
	{
		if (getFrameX() == 1)
		{
			setFrameX(4);
			//return;
		}
		else if (getFrameX() == 4)
		{
			setFrameX(1);
			//return;
		}
	}
	else if (getAttribute() == TILE_BOSS_GROUND)
	{
		if (getFrameX() == 1)
		{
			setFrameX(4);
			//return;
		}
		else if (getFrameX() == 4)
		{
			setFrameX(1);
			//return;
		}
	}
	else if (getAttribute() == TILE_WATER)
	{
		//만약 시야거리에 잇다면
		if (getFrameY() == 4)
		{
			setFrameY(5);
			//return;
		}
		else if (getFrameY() == 5)
		{
			setFrameY(4);
			//return;
		}
		//시야 거리내에 없다면 5 ~ 6으로 순환
	}
}

void tile::render()
{
	//렌더는 나중에 추가하자
	if (_tile.img != nullptr)
	{
	//	_tile.img->frameRender(_tile.rc.left, _tile.rc.top, _tile.rc.right, _tile.rc.bottom);
	}
}

void tile::objSetDefault()
{
	setImg(nullptr);
	setFrameX(NULL);
	setFrameY(NULL);
	setAttribute(OBJ_NONE);
	setImgNum(IMG_NONE);
	setIsAvailMove(true);
}

void tile::setWaterTile()
{
	setImg(IMAGEMANAGER->findImage("tile"));
	setFrameX(7);
	setFrameY(4);
	setAttribute(TILE_WATER);
	setImgNum(IMG_TILE);
	setIsAvailMove(true);
}
