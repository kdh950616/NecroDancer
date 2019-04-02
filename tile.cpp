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

void tile::openChest()
{
	int idX;
	while (1)
	{
		idX = RND->getFromIntTo(0, 9);

		if (idX % 2 == 0)
		{
			break;
		}
	}

	int idY;
	while (1)
	{
		idY = RND->getFromIntTo(0, 11);

		if (idY % 2 == 0)
		{
			break;
		}
	}
	setImg(IMAGEMANAGER->findImage("Item"));
	setImgNum(IMG_ITEM);
	setFrameX(idX);
	setFrameY(idY);
	setIsAvailMove(true);

	switch (idY)
	{
	case 0:
		switch (idX)
		{
		case 0:
			setAttribute(ITEM_SPEAR_NORMAL);
			break;
		case 2:
			setAttribute(ITEM_SPEAR_BLOOD);
			break;
		case 4:
			setAttribute(ITEM_SPEAR_TITANIUM);
			break;
		case 6:
			setAttribute(ITEM_SPEAR_GLASS);
			break;
		case 8:
			setAttribute(ITEM_SPEAR_OBSIDIAN);
			break;
		}
		break;
	case 2:
		switch (idX)
		{
		case 0:
			setAttribute(ITEM_SWORD_NORMAL);
			break;
		case 2:
			setAttribute(ITEM_SWORD_BLOOD);
			break;
		case 4:
			setAttribute(ITEM_SWORD_TITANIUM);
			break;
		case 6:
			setAttribute(ITEM_SWORD_GLASS);
			break;
		case 8:
			setAttribute(ITEM_SWORD_OBSIDIAN);
			break;
		}
		break;
	case 4:
		switch (idX)
		{
		case 0:
			setAttribute(ITEM_TORCH_1);
			break;
		case 2:
			setAttribute(ITEM_TORCH_2);
			break;
		case 4:
			setAttribute(ITEM_TORCH_3);
			break;
		case 6:
			setAttribute(ITEM_TORCH_GLASS);
			break;
		case 8:
			setAttribute(ITEM_TORCH_OBSIDIAN);
			break;
		}
		break;
	case 6:
		switch (idX)
		{
		case 0:
			setAttribute(ITEM_SHOVEL_NORMAL);
			break;
		case 2:
			setAttribute(ITEM_SHOVEL_BLOOD);
			break;
		case 4:
			setAttribute(ITEM_SHOVEL_TITANIUM);
			break;
		case 6:
			setAttribute(ITEM_SHOVEL_GLASS);
			break;
		case 8:
			setAttribute(ITEM_SHOVEL_OBSIDIAN);
			break;
		}
		break;
	case 8:
		switch (idX)
		{
		case 0:
			setAttribute(ITEM_ARMOR_1);
			break;
		case 2:
			setAttribute(ITEM_ARMOR_2);
			break;
		case 4:
			setAttribute(ITEM_ARMOR_3);
			break;
		case 6:
			setAttribute(ITEM_ARMOR_GLASS);
			break;
		case 8:
			setAttribute(ITEM_ARMOR_OBSIDIAN);
			break;
		}
		break;
	case 10:
		switch (idX)
		{
		case 0:
			setAttribute(ITEM_HP_APPLE);
			break;
		case 2:
			setAttribute(ITEM_HP_CHEESE);
			break;
		case 4:
			setAttribute(ITEM_HP_MEAT);
			break;
		case 6:
			setAttribute(ITEM_GLASS);
			break;
		case 8:
			setAttribute(ITEM_DAGGER);
			break;
		}
		break;
	}
}

void tile::eraseMap()
{
	setAttribute(TILE_NONE);
	setFrameX(NULL);
	setFrameY(NULL);
	setImgNum(IMG_NONE);
	setImg(nullptr);
	setIsAvailMove(false);
}

void tile::eraseObj()
{
	setAttribute(OBJ_NONE);
	setFrameX(NULL);
	setFrameY(NULL);
	setImgNum(IMG_NONE);
	setImg(nullptr);
	setIsAvailMove(false);
}

void tile::setBossWall()
{
	setAttribute(OBJ_WALL_BOSS);
	setFrameX(RND->getFromIntTo(3, 5));
	setFrameY(RND->getFromIntTo(2, 4));
	setImgNum(IMG_WALL);
	setImg(IMAGEMANAGER->findImage("wall"));
	setIsAvailMove(false);
}
