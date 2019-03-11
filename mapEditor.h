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
	typedef vector<tile*> vLine;	 //x축 갯수
	typedef vector<vLine> vvMap; //x축을 담을 갯수 -> y축
	typedef vector<vLine> vvObj; // 오브젝트 추가
	
	//==========================================
	//				그려질 전체 타일
	//==========================================

	D2D_RECT_F _draw_Area_Rc;		//맵을 출력할 범위를 가진 렉트
	vvMap _vvMap;					//전체 맵 사이즈
	vvObj _vvObj;					//전체 맵사이즈만큼 오브젝트있슴.
	int _tileSizeX;					//x축사이즈
	int _tileSizeY;					//y축사이즈


	bool _isClick;					//마우스 클릭 했는지
	int _count;
	bool _isChanged;
	bool _isTestGoldBonus;
	int _testGoldBonus;
	//==========================================
	//					샘플타일
	//==========================================
	POINT _sample_Move_Cal_Pt;		//샘플창 이동할떄 계산할 변수
	POINT _sample_Area_Pos;			//샘플창 좌표
	bool _isShow_Sample;			//샘플창 on & off
	image* _currentImg;				//샘플의 현재 이미지
	int _currentImgNum;				//샘플 현재 이미지 번호
	tagPage _currentPage;			//샘플의 현재 페이지
	bool _isCollision_SampleTile;	//샘플타일이랑 충돌중인여부 확인용 -> 드래그에 필요
	D2D_RECT_F _sample_Area_Rc;
	tile* _sampleTile[12][9];

	//마우스로 샘플 찝자
	//POINT _corsor_Sample_Select[2];		//[0] == 시작점 // [1] == 끝점  ---------> 드래그
	//										==> 이거 빼고 그냥 맵에다 드래그로 그리는거 하자.
	POINT _corsor_Sample_Select;
	POINT _corsor_Map_Draw[2];
	bool _isShow_Drag_Area;

	//찝은거 붙이자.
	bool _isCollision_Sample_Area_Rc;	//샘플창이랑 충돌중인여부 확인용 -> 이게 트루이면 맵에 그려지면안됌.
	POINT _corsor_Map_Idx;			//샘플창이랑 충돌중이 아니고 클릭하면 현재의 마우스가 어떤 타일을 가리키고있는지 인덱스값 저장함.

	//====================================
	//				카테고리
	//====================================
	tagMaptool_Btn _btn_Category[5];			// [0] == tile || [1] == obj	|| [2] == item || [3] == Enemy || [4] == etc
	tagMaptool_Btn _btn_Page[2];		// [0] == <-   || [1] == ->
	//====================================
	//				사이즈
	//====================================
	bool		   _isCollision_Btn;
	tagMaptool_Btn _btn_SizeChange;		
	tagMaptool_Btn _btn_SizeArrow[4];		// [0] == x Down || [1] == x Up || [2] == y Down || [3] == y Up
	int			   _delayCount;
	//====================================
	//			세이브 & 로드
	//====================================
	tagMaptool_Btn _btn_Save;
	tagMaptool_Btn _btn_Load;
	//====================================
	//				맵 인덱스
	//====================================
	int			   _mapNum;
	tagMaptool_Btn _btn_MapIdx;
	tagMaptool_Btn _btn_MapIdxArrow[2];
	//====================================
	//				지우개
	//====================================
	tagMaptool_Btn _btn_Erase;
	tagMaptool_Btn _btn_ObjErase;
	bool _isErase;
	bool _isObjErase;
	//====================================
	//		타일 랜덤깔기용 변수
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
	void setNewMapTile(tile* tile, int idX, int idY);		//멥에 빈 타일 만들어주는 함쑤	  
	void objInit();
	void setNewObjTile(tile* tile, int idX, int idY);
	void sampleInit();
	void setNewSampleTile(tile* tile, int idX, int idY);
	void buttonInit();
	
	//update----------------------------------------------------------------------------------------------------------------------
		void cameraFunc();
		void blinkMapTileFunc();
		void corsorMapIdxCal();

		//샘플
		void sampleFunc();
		void sampleSelect(int idX, int idY);
		void setMapTile(tile* tile, int idX, int idY);
		void setMapObject(tile* tile, int idX, int idY);
		
		//카테고리
		void categoryFunc();
		void changeCategory(tile* tile, int idX, int idY);	//샘플 타일 만들어주는 함쑤		

		//맵
		void mapDragDraw();
		
		//사이즈
		void mapSizeChange();
		void addMapX();
		void addMapY();
		void eraseMapX();
		void eraseMapY();
		
		//세이브
		void mapSaveBtn();
		void mapLoadBtn();
		void saveMap();
		void loadMap();

		//맵 인덱스
		void mapIdxChange();

		//지우개
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
	
	//기타-----------------------------------------------------------------------------------------------------------------------
	//RECT  rectMake (D2D_RECT_F rc)	{ return RECT { (LONG)rc.left,(LONG)rc.top,(LONG)rc.right,(LONG)rc.bottom }; }
	//POINT pointMake(POINTF pt)		{ return POINT{ (int)pt.x,(int)pt.y }; }

	//접근자-----------------------------------------------------------------------------------------------------------------------






	//=================================================
	//						관
	//=================================================
	//void mapSave();
	//void mapLoad();
};

