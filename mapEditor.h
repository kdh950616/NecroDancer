#pragma once
#include "gameNode.h"
#include "tile.h"
#pragma warning(disable: 4996)

struct tagMaptool_Btn
{
	image* img;
	POINT pos;
	D2D_RECT_F rc;
	POINT frame;
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
	int _tileSizeX;					//x�������
	int _tileSizeY;					//y�������


	bool _isClick;					//���콺 Ŭ�� �ߴ���
	int _count;
	bool _isChanged;
	bool _isTestGoldBonus;
	int _testGoldBonus;
	//==========================================
	//					����Ÿ��
	//==========================================
	POINT _sample_Move_Cal_Pt;		//����â �̵��ҋ� ����� ����
	POINT _sample_Area_Pos;			//����â ��ǥ
	bool _isShow_Sample;			//����â on & off
	image* _currentImg;				//������ ���� �̹���
	int _currentImgNum;				//���� ���� �̹��� ��ȣ
	tagPage _currentPage;			//������ ���� ������
	bool _isCollision_SampleTile;	//����Ÿ���̶� �浹���ο��� Ȯ�ο� -> �巡�׿� �ʿ�
	D2D_RECT_F _sample_Area_Rc;
	tile* _sampleTile[12][9];

	//���콺�� ���� ����
	//POINT _corsor_Sample_Select[2];		//[0] == ������ // [1] == ����  ---------> �巡��
	//										==> �̰� ���� �׳� �ʿ��� �巡�׷� �׸��°� ����.
	POINT _corsor_Sample_Select;
	POINT _corsor_Map_Draw[2];
	bool _isShow_Drag_Area;

	//������ ������.
	bool _isCollision_Sample_Area_Rc;	//����â�̶� �浹���ο��� Ȯ�ο� -> �̰� Ʈ���̸� �ʿ� �׷�����ȉ�.
	POINT _corsor_Map_Idx;			//����â�̶� �浹���� �ƴϰ� Ŭ���ϸ� ������ ���콺�� � Ÿ���� ����Ű���ִ��� �ε����� ������.

	//====================================
	//				ī�װ�
	//====================================
	tagMaptool_Btn _btn_Category[5];			// [0] == tile || [1] == obj	|| [2] == item || [3] == Enemy || [4] == etc
	tagMaptool_Btn _btn_Page[2];		// [0] == <-   || [1] == ->
	//====================================
	//				������
	//====================================
	bool		   _isCollision_Btn;
	tagMaptool_Btn _btn_SizeChange;		
	tagMaptool_Btn _btn_SizeArrow[4];		// [0] == x Down || [1] == x Up || [2] == y Down || [3] == y Up
	int			   _delayCount;
	//====================================
	//			���̺� & �ε�
	//====================================
	tagMaptool_Btn _btn_Save;
	tagMaptool_Btn _btn_Load;
	//====================================
	//				�� �ε���
	//====================================
	int			   _mapNum;
	tagMaptool_Btn _btn_MapIdx;
	tagMaptool_Btn _btn_MapIdxArrow[2];
	//====================================
	//				���찳
	//====================================
	tagMaptool_Btn _btn_Erase;
	tagMaptool_Btn _btn_ObjErase;
	bool _isErase;
	bool _isObjErase;
	//====================================
	//		Ÿ�� �������� ����
	//====================================
	
	
	


private:
	const int DEFAULT_MAP_SIZE_X = 40;
	const int DEFAULT_MAP_SIZE_Y = 40;
public:
	mapEditor();
	~mapEditor();

	HRESULT init();
	void release();
	void update();
	void render();

	//init----------------------------------------------------------------------------------------------------------------------
	
	void imageInit();
	void mapInit();
	void setNewMapTile(tile* tile, int idX, int idY);		//�㿡 �� Ÿ�� ������ִ� �Ծ�	  
	void objInit();
	void setNewObjTile(tile* tile, int idX, int idY);
	void sampleInit();
	void setNewSampleTile(tile* tile, int idX, int idY);
	void buttonInit();
	
	//update----------------------------------------------------------------------------------------------------------------------
		void cameraFunc();
		void blinkMapTileFunc();
		void corsorMapIdxCal();

		//����
		void sampleFunc();
		void sampleSelect(int idX, int idY);
		void setMapTile(tile* tile, int idX, int idY);
		void setMapObject(tile* tile, int idX, int idY);
		
		//ī�װ�
		void categoryFunc();
		void changeCategory(tile* tile, int idX, int idY);	//���� Ÿ�� ������ִ� �Ծ�		

		//��
		void mapDragDraw();
		
		//������
		void mapSizeChange();
		void addMapX();
		void addMapY();
		void eraseMapX();
		void eraseMapY();
		
		//���̺�
		void mapSaveBtn();
		void mapLoadBtn();
		void saveMap();
		void loadMap();

		//�� �ε���
		void mapIdxChange();

		//���찳
		void eraseMode();
		void objEraseMode();
		void erase(tile* mapTile, tile* objTile);
		void objErase(tile* objTile);

	//render----------------------------------------------------------------------------------------------------------------------
	
	void mapTileRender();
	void objectTileRender();
	void sampleTileRender();
	void buttonRender();
	void linePreview();
	void testText();
	
	//��Ÿ-----------------------------------------------------------------------------------------------------------------------
	//RECT  rectMake (D2D_RECT_F rc)	{ return RECT { (LONG)rc.left,(LONG)rc.top,(LONG)rc.right,(LONG)rc.bottom }; }
	//POINT pointMake(POINTF pt)		{ return POINT{ (int)pt.x,(int)pt.y }; }

	//������-----------------------------------------------------------------------------------------------------------------------






	//=================================================
	//						��
	//=================================================
	//void mapSave();
	//void mapLoad();
};

