#pragma once
#include "gameNode.h"
#include "tileNode.h"

enum TILE_ATTR
{
	TILE_NONE,
	TILE_GROUND,
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
	OBJ_NONE = 10,
	OBJ_WALL1,
	OBJ_WALL2,
	OBJ_WALL3,
	OBJ_WALL_GOLD,
	OBJ_WALL_BOSS,
	OBJ_WALL_END,
	OBJ_DOOR_FRONT,
	OBJ_DOOR_SIDE,
	ENEMY1_1_1, // ��������
	ENEMY1_1_2,	// ��������
	ENEMY1_2_1,	// �Ͼ��ذ�
	ENEMY1_2_2,	// ��ذ�
	ENEMY1_2_3,	// ����ذ�
	ENEMY1_3_1,	// �ʷϽ�����
	ENEMY1_3_2,	// �Ķ�������
	ENEMY2_1_1,	// Ŭ��
	ENEMY2_1_2,	// ���̽�
	ENEMY2_2_1,	// �Ƹ�������
	ENEMY2_2_2,	// ����
	ENEMY3_1_1,	// �̳�Ÿ��ν�
	ENEMY3_1_2,	// ��
	ENEMY3_2_1,	// Ȳ�ݹ���
	ENEMY3_2_2, // �꽬
	ENEMY4_1_1,	// ��
	ENEMY4_1_2,	// ���
	ENEMY4_1_3,	// ��
	ENEMY4_2_1,	// ����Ʈ
	ENEMY4_2_2,	// ��
	ENEMY4_2_3,	// ŷ


	OBJ_END
};

enum tagItem
{
	ITEM_START = 100,
	ITEM_SPEAR_NORMAL,
	ITEM_SPEAR_BLOOD,
	ITEM_SPEAR_TITANIUM,
	ITEM_SPEAR_GLASS,
	ITEM_SPEAR_OBSIDIAN,
	ITEM_SWORD_NORMAL,
	ITEM_SWORD_BLOOD,
	ITEM_SWORD_TITANIUM,
	ITEM_SWORD_GLASS,
	ITEM_SWORD_OBSIDIAN,
	ITEM_GLASS_WEAPON,
	//====================================
	ITEM_TORCH_1,
	ITEM_TORCH_2,
	ITEM_TORCH_3,
	ITEM_TORCH_GLASS,
	ITEM_TORCH_OBSIDIAN,
	ITEM_GLASS_TORCH,
	//===================================
	ITEM_SHOVEL_NORMAL,
	ITEM_SHOVEL_BLOOD,
	ITEM_SHOVEL_TITANIUM,
	ITEM_SHOVEL_GLASS,
	ITEM_SHOVEL_OBSIDIAN,
	ITEM_GLASS_SHOVEL,
	//============================================
	ITEM_ARMOR_1,
	ITEM_ARMOR_2,
	ITEM_ARMOR_3,
	ITEM_ARMOR_GLASS,
	ITEM_ARMOR_OBSIDIAN,
	//==============================================
	ITEM_HP_APPLE,
	ITEM_HP_CHEESE,
	ITEM_HP_MEAT,
	ITEM_GLASS,
	ITEM_DAGGER,
	ITEM_END
};

enum tagImgNum
{
	IMG_NONE,
	IMG_TILE,
	IMG_WALL,
	IMG_ITEM,
	IMG_ENEMY1,
	IMG_ENEMY2,
	IMG_ENEMY3,
	IMG_ENEMY4,
	IMG_ETC,
	IMG_END
};

enum tagArrow
{
	ARROW_X_DOWN,
	ARROW_X_UP,
	ARROW_Y_DOWN,
	ARROW_Y_UP,
	ARROW_END
};
enum tagPage
{
	PAGE_TILE,	//Ÿ�� �׸��� ���
	PAGE_WALL,	//������Ʈ �׸��� ���
	PAGE_ITEM,
	PAGE_ENEMY,
	PAGE_ETC,
	PAGE_END		//��
};

class tile : public gameNode
{
private:

	POINT _idx;			//�ε���
	tagTileSet _tile;

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

	void objSetDefault();
	void setWaterTile();

};

