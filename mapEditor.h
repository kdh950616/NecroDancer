#pragma once
#include "gameNode.h"
#include "tile.h"


enum tagPage
{
	PAGE_TILE,	//타일 그리기 모드
	PAGE_WALL,	//오브젝트 그리기 모드
	PAGE_ITEM,
	PAGE_ENEMY,
	PAGE_ETC,
	PAGE_END		//끗
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
	typedef vector<tile*> vLine;	 //x축 갯수
	typedef vector<vLine> vvMap; //x축을 담을 갯수 -> y축
	typedef vector<vLine> vvObj; // 오브젝트 추가
	
	//==========================================
	//				그려질 전체 타일
	//==========================================

	D2D_RECT_F _draw_Area_Rc;		//맵을 출력할 범위를 가진 렉트
	vvMap _vvMap;					//전체 맵 사이즈
	vvObj _vvObj;					//전체 맵사이즈만큼 오브젝트있슴.
	int _tileNumX;					//x축사이즈
	int _tileNumY;					//y축사이즈


	bool _isClick;					//마우스 클릭 했는지
	//==========================================
	//					샘플타일
	//==========================================
	POINT _sample_Move_Cal_Pt;		//샘플창 이동할떄 계산할 변수
	POINT _sample_Area_Pos;			//샘플창 좌표
	bool _isShow_Sample;			//샘플창 on & off
	image* _currentImg;				//샘플의 현재 이미지
	tagPage _currentPage;			//샘플의 현재 페이지
	bool _isCollision_SampleTile;	//샘플타일이랑 충돌중인여부 확인용 -> 드래그에 필요
	D2D_RECT_F _sample_Area_Rc;
	tile* _sampleTile[12][9];
	tagMaptool_Btn _category[5];	// [0] == tile || [1] == obj	|| [2] == item || [3] == Enemy || [4] == etc
	tagMaptool_Btn _size[4];		// [0] == x Up || [1] == x Down || [2] == y Up || [3] == y Down
	tagMaptool_Btn _save;
	tagMaptool_Btn _load;

	//마우스로 샘플 찝자
	//POINT _corsor_Sample_Select[2];		//[0] == 시작점 // [1] == 끝점  ---------> 드래그
	//										==> 이거 빼고 그냥 맵에다 드래그로 그리는거 하자.
	POINT _corsor_Sample_Select;
	POINT _corsor_Map_Draw[2];
	bool _isShow_Drag_Area;

	//찝은거 붙이자.
	bool _isCollision_Sample_Area_Rc;	//샘플창이랑 충돌중인여부 확인용 -> 이게 트루이면 맵에 그려지면안됌.
	POINT _corsor_Map_Idx;			//샘플창이랑 충돌중이 아니고 클릭하면 현재의 마우스가 어떤 타일을 가리키고있는지 인덱스값 저장함.
	
	


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
	void setNewMapTile(tile* tile, int idX, int idY);		//멥에 빈 타일 만들어주는 함쑤	  
	void objTileInit();
	void setNewObjTile(tile* tile, int idX, int idY);
	void sampleTileInit();
	void setNewSampleTile(tile* tile, int idX, int idY);
	
	//update----------------------------------------------------------------------------------------------------------------------
	
	void sampleFunc();
	void changeSampleTile(tile* tile, int idX, int idY);	//샘플 타일 만들어주는 함쑤		
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
	
	//기타-----------------------------------------------------------------------------------------------------------------------
	RECT  rectMake (D2D_RECT_F rc)	{ return RECT { (LONG)rc.left,(LONG)rc.top,(LONG)rc.right,(LONG)rc.bottom }; }
	POINT pointMake(POINTF pt)		{ return POINT{ (int)pt.x,(int)pt.y }; }

	//접근자-----------------------------------------------------------------------------------------------------------------------

};

