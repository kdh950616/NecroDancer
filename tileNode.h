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
