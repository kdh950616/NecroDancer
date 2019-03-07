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

enum CTRL
{
	CTRL_TILEDRAW,	//타일 그리기 모드
	CTRL_OBJDRAW,	//오브젝트 그리기 모드
	CTRL_ITEMDRAW,	//아이템 그리기 모드
	CTRL_ERASER,	//지우개 모드
	CTRL_END		//끗을 알림.
};

enum tagPage
{
	PAGE_TILE,	//타일 그리기 모드
	PAGE_WALL,	//오브젝트 그리기 모드
	PAGE_ITEM,
	PAGE_ENEMY,
	PAGE_ETC,
	PAGE_NONE		//끗
};
