#pragma once

const int TILESIZE = 48;

struct tagTileSet
{
	image* img;		//�̹���
	POINT pos;			//��ǥ
	D2D_RECT_F rc;		//��Ʈ
	int frameX;			//������X
	int frameY;			//������Y
	int attribute;		//�Ӽ�
	//string imgKey;
	int imgNum;
};

enum CTRL
{
	CTRL_TILEDRAW,	//Ÿ�� �׸��� ���
	CTRL_OBJDRAW,	//������Ʈ �׸��� ���
	CTRL_ITEMDRAW,	//������ �׸��� ���
	CTRL_ERASER,	//���찳 ���
	CTRL_END		//���� �˸�.
};

enum tagPage
{
	PAGE_TILE,	//Ÿ�� �׸��� ���
	PAGE_WALL,	//������Ʈ �׸��� ���
	PAGE_ITEM,
	PAGE_ENEMY,
	PAGE_ETC,
	PAGE_NONE		//��
};
