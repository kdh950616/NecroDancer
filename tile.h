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
	OBJ_NONE = 100,
	OBJ_WALL1,
	OBJ_WALL2,
	OBJ_WALL3,
	OBJ_WALL_GOLD,
	OBJ_WALL_BOSS,
	OBJ_WALL_END,
	OBJ_DOOR_FRONT,
	OBJ_DOOR_SIDE,
	ENEMY1_1_1, // 검은박쥐
	ENEMY1_1_2,	// 빨간박쥐
	ENEMY1_2_1,	// 하얀해골
	ENEMY1_2_2,	// 까만해골
	ENEMY1_2_3,	// 노란해골
	ENEMY1_3_1,	// 초록슬라임
	ENEMY1_3_2,	// 파란슬라임
	ENEMY2_1_1,	// 클론
	ENEMY2_1_2,	// 레이스
	ENEMY2_2_1,	// 아르마딜로
	ENEMY2_2_2,	// 좀비
	ENEMY3_1_1,	// 미노타우로스
	ENEMY3_1_2,	// 용
	ENEMY3_2_1,	// 황금박쥐
	ENEMY3_2_2, // 밴쉬
	ENEMY4_1_1,	// 폰
	ENEMY4_1_2,	// 비숍
	ENEMY4_1_3,	// 룩
	ENEMY4_2_1,	// 나이트
	ENEMY4_2_2,	// 퀸
	ENEMY4_2_3,	// 킹

	OBJ_END
};

enum tagItem
{
	ITEM_START = 200,
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

enum tagEtc
{
	ETC_START = 299,
	ETC_TORCH,			// 땅에 횃불 뿌릴경우 이거
	ETC_TORCH_WALL1,	//횃불을 벽에다가 뿌릴경우 해당 벽의 속성 + 200하면 이속성이된다. 이속성인 벽은 기존 벽의 이미지 출력하면서 동시에 자기위에 횟불프레임 렌더 해야함.
	ETC_TORCH_WALL2,	//				"
	ETC_TORCH_WALL3,	//				"
	ETC_TORCH_GOLD,		//				"
	ETC_TORCH_BOSS,		//				"
	ETC_CHEST,
	ETC_SLOW,
	ETC_FAST,
	ETC_TRAPDOOR,
	ETC_SHOPKEEPER,
	ETC_END
};

enum tagImgNum
{
	IMG_NONE = 400,
	IMG_TILE,
	IMG_WALL,
	IMG_ITEM,
	IMG_ENEMY1,
	IMG_ENEMY2,
	IMG_ENEMY3,
	IMG_ENEMY4,
	IMG_ETC1,
	IMG_ETC2,
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
	PAGE_TILE,	//타일 그리기 모드
	PAGE_WALL,	//오브젝트 그리기 모드
	PAGE_ITEM,
	PAGE_ENEMY,
	PAGE_ETC,
	PAGE_END		//끗
};

class tile : public gameNode
{
private:

	POINT _idx;			//인덱스
	tagTileSet _tile;

	bool _isAvailMove; // 트루면 갈수있슴. -> 위에 오브젝트 올려지거나 못가는거 올려지면 펄스로

	//에이스타용
	tile* _parent;		//엄마
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


	//접근자

	//get
	image*				getImg()			{ return _tile.img; }	
	POINT				getPos()			{ return _tile.pos; }			
	POINT				getIdx()			{ return _idx; }			
	D2D_RECT_F			getRc()				{ return _tile.rc; }		
	int					getFrameX()			{ return _tile.frameX; }		
	int					getFrameY()			{ return _tile.frameY; }		
	int					getAttribute()		{ return _tile.attribute; }		
	int					getImgNum() { return _tile.imgNum; }

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
	void setImgNum(int imageNum) { _tile.imgNum = imageNum; }

	void setIsAvailMove(bool isAvailMove)	{ _isAvailMove = isAvailMove; }

	void setParent		(tile* parent)		{ _parent = parent; }
	void setGScore		(int gScore)		{ _gScore = gScore; }
	void setHScore		(int hScore)		{ _hScore = hScore; }
	void setFScore		(int fScore)		{ _fScore = fScore; }

	void setTileSet			(tagTileSet tileSet){ _tile = tileSet; }

	//=============================
	//			즉석 추가
	//=============================

	void objSetDefault();
	void setWaterTile();
	void openChest();
	void eraseMap();
	void eraseObj();
	void setBossWall();
};

