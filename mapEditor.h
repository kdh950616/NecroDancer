#pragma once
#include "gameNode.h"
#include "tile.h"


enum tagPage
{
	PAGE_TILE,	//Ÿ�� �׸��� ���
	PAGE_WALL,	//������Ʈ �׸��� ���
	PAGE_ITEM,
	PAGE_ENEMY,
	PAGE_ETC,
	PAGE_END		//��
};

struct tagMaptool_Btn
{
	image* img;
	POINT pos;
	D2D_RECT_F rc;
};

class mapEditor : public gameNode
{
private:
	typedef vector<tile*> vLine;	 //x�� ����
	typedef vector<vLine> vvMap; //x���� ���� ���� -> y��
	typedef vector<vLine> vvObj; // ������Ʈ �߰�
	
	//==========================================
	//				�׷��� ��ü Ÿ��
	//==========================================

	D2D_RECT_F _draw_Area_Rc;		//���� ����� ������ ���� ��Ʈ
	vvMap _vvMap;					//��ü �� ������
	vvObj _vvObj;					//��ü �ʻ����ŭ ������Ʈ�ֽ�.
	int _tileNumX;					//x�������
	int _tileNumY;					//y�������


	bool _isClick;					//���콺 Ŭ�� �ߴ���
	//==========================================
	//					����Ÿ��
	//==========================================
	POINT _sample_Move_Cal_Pt;		//����â �̵��ҋ� ����� ����
	POINT _sample_Area_Pos;			//����â ��ǥ
	bool _isShow_Sample;			//����â on & off
	image* _currentImg;				//������ ���� �̹���
	tagPage _currentPage;			//������ ���� ������
	bool _isCollision_SampleTile;	//����Ÿ���̶� �浹���ο��� Ȯ�ο� -> �巡�׿� �ʿ�
	D2D_RECT_F _sample_Area_Rc;
	tile* _sampleTile[12][9];
	tagMaptool_Btn _category[5];	// [0] == tile || [1] == obj	|| [2] == item || [3] == Enemy || [4] == etc
	tagMaptool_Btn _size[4];		// [0] == x Up || [1] == x Down || [2] == y Up || [3] == y Down
	tagMaptool_Btn _save;
	tagMaptool_Btn _load;

	//���콺�� ���� ����
	//POINT _corsor_Sample_Select[2];		//[0] == ������ // [1] == ����  ---------> �巡��
	//										==> �̰� ���� �׳� �ʿ��� �巡�׷� �׸��°� ����.
	POINT _corsor_Sample_Select;
	POINT _corsor_Map_Draw[2];
	bool _isShow_Drag_Area;

	//������ ������.
	bool _isCollision_Sample_Area_Rc;	//����â�̶� �浹���ο��� Ȯ�ο� -> �̰� Ʈ���̸� �ʿ� �׷�����ȉ�.
	POINT _corsor_Map_Idx;			//����â�̶� �浹���� �ƴϰ� Ŭ���ϸ� ������ ���콺�� � Ÿ���� ����Ű���ִ��� �ε����� ������.
	
	


private:
	const int DEFAULT_MAP_SIZE_X = 40;
	const int DEFAULT_MAP_SIZE_Y = 30;
public:
	mapEditor();
	~mapEditor();

	HRESULT init();
	void release();
	void update();
	void render();

	//init----------------------------------------------------------------------------------------------------------------------
	
	void imageInit();
	void mapTileInit();
	void setNewMapTile(tile* tile, int idX, int idY);		//�㿡 �� Ÿ�� ������ִ� �Ծ�	  
	void objTileInit();
	void setNewObjTile(tile* tile, int idX, int idY);
	void sampleTileInit();
	void setNewSampleTile(tile* tile, int idX, int idY);
	
	//update----------------------------------------------------------------------------------------------------------------------
	
	void sampleFunc();
	void changeSampleTile(tile* tile, int idX, int idY);	//���� Ÿ�� ������ִ� �Ծ�		
	void cameraFunc();
	void sampleDragFcunc(int idX, int idY);
	void setMapTile(tile* tile, int idX, int idY);
	void setMapObject(tile* tile, int idX, int idY);
	void mapDragDraw();
	void mapSizeChange();
	//render----------------------------------------------------------------------------------------------------------------------
	
	void mapTileRender();
	void objectTileRender();
	void sampleTileRender();
	void buttonRender();
	void linePreview();

	void testText();
	
	//��Ÿ-----------------------------------------------------------------------------------------------------------------------
	RECT  rectMake (D2D_RECT_F rc)	{ return RECT { (LONG)rc.left,(LONG)rc.top,(LONG)rc.right,(LONG)rc.bottom }; }
	POINT pointMake(POINTF pt)		{ return POINT{ (int)pt.x,(int)pt.y }; }

	//������-----------------------------------------------------------------------------------------------------------------------

};

