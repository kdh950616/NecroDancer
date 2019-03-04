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
};

enum CTRL
{
	CTRL_TILEDRAW,	//Ÿ�� �׸��� ���
	CTRL_OBJDRAW,	//������Ʈ �׸��� ���
	CTRL_ITEMDRAW,	//������ �׸��� ���
	CTRL_ERASER,	//���찳 ���
	CTRL_END		//���� �˸�.
};

