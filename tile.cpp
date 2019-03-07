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
			return;
		}
		else if (getFrameX() == 4)
		{
			setFrameX(1);
			return;
		}
	}
	else if (getAttribute() == TILE_BOSS_GROUND)
	{
		if (getFrameX() == 1)
		{
			setFrameX(4);
			return;
		}
		else if (getFrameX() == 4)
		{
			setFrameX(1);
			return;
		}
	}
	else if (getAttribute() == TILE_WATER)
	{
		//���� �þ߰Ÿ��� �մٸ�
		if (getFrameY() == 4)
		{
			setFrameY(5);
			return;
		}
		else if (getFrameY() == 5)
		{
			setFrameY(4);
			return;
		}
		//�þ� �Ÿ����� ���ٸ� 5 ~ 6���� ��ȯ
	}
}

void tile::render()
{
	//������ ���߿� �߰�����
	if (_tile.img != nullptr)
	{
	//	_tile.img->frameRender(_tile.rc.left, _tile.rc.top, _tile.rc.right, _tile.rc.bottom);
	}
}
