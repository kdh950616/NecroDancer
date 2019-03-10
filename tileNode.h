#pragma once

const int TILESIZE = 48;

struct tagTileSet
{
	image* img;		//이미지
	POINT pos;			//좌표
	D2D_RECT_F rc;		//렉트
	int frameX;			//프레임X
	int frameY;			//프레임Y
	int attribute;		//속성
	//string imgKey;
	int imgNum;
};
