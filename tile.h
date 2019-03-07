#pragma once
#include "gameNode.h"
#include "tileNode.h"

enum TILE_ATTR
{
	TILE_NONE,
	TILE_GROUND = 10,
	TILE_BOSS_GROUND,
	TILE_SHOP,
	TILE_WATER,
	TILE_UNLOCK_FLOOR,
	TILE_LOCK_FLOOR,
	TILE_BOSS_FLOOR,
	TILE_MAX
};

enum OBJ_ATTR
{
	OBJ_NONE,
	OBJ_WALL1,
	OBJ_WALL2,
	OBJ_WALL3,
	OBJ_WALL_GOLD,
	OBJ_WALL_BOSS,
	OBJ_WALL_END,
	OBJ_DOOR_FRONT,
	OBJ_DOOR_SIDE,


	OBJ_END
};

class tile : public gameNode
{
private:

	POINT _idx;			//�ε���
	tagTileSet _tile;
	//string imgKeyValue;	//�̹��� Ű�� - > �ʿ�����Ű��� ���.
	
	bool _isAvailMove; // Ʈ��� �����ֽ�. -> ���� ������Ʈ �÷����ų� �����°� �÷����� �޽���
	//���̽�Ÿ��
	tile* _parent;		//����
	int _gScore;
	int _hScore;
	int _fScore;

public:
	tile();
	~tile();

	HRESULT init();
	void release();
	void update();
	void render();


	//������

	//get
	image*				getImg()			{ return _tile.img; }	
	POINT				getPos()			{ return _tile.pos; }			
	POINT				getIdx()			{ return _idx; }			
	D2D_RECT_F			getRc()				{ return _tile.rc; }		
	int					getFrameX()			{ return _tile.frameX; }		
	int					getFrameY()			{ return _tile.frameY; }		
	int					getAttribute()		{ return _tile.attribute; }		
	//string				getImgKey()		{ return _tile.imgKey; }
	int				getImgNum() { return _tile.imgNum; }

	bool				getIsAvailMove()	{ return _isAvailMove; }

	tile*				getParent()			{ return _parent; }		
	int					getGScore()			{return _gScore; }
	int					getHScore()			{return _hScore; }
	int					getFScore()			{return _fScore; }

	//set
	void setImg			(image* img)		{ _tile.img = img; }
	void setPos			(POINT pos)			{ _tile.pos = pos; }
	void setIdx			(POINT idx)			{ _idx = idx; }
	void setRc			(D2D_RECT_F rc)		{ _tile.rc = rc; }
	void setFrameX		(int frameX)		{ _tile.frameX = frameX; }
	void setFrameY		(int frameY)		{ _tile.frameY = frameY; }
	void setAttribute	(int attribute)		{ _tile.attribute = attribute; }
	//void setImgKey		(string imgKey)		{ _tile.imgKey = imgKey; }
	void setImgNum(int imageNum) { _tile.imgNum = imageNum; }

	void setIsAvailMove(bool isAvailMove)	{ _isAvailMove = isAvailMove; }

	void setParent		(tile* parent)		{ _parent = parent; }
	void setGScore		(int gScore)		{ _gScore = gScore; }
	void setHScore		(int hScore)		{ _hScore = hScore; }
	void setFScore		(int fScore)		{ _fScore = fScore; }

	void setTileSet			(tagTileSet tileSet){ _tile = tileSet; }

	//=============================
	//			�Ｎ �߰�
	//=============================
};

