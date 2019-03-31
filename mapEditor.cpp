#include "stdafx.h"
#include "mapEditor.h"


mapEditor::mapEditor()
{
}


mapEditor::~mapEditor()
{
}

HRESULT mapEditor::init()
{
	imageInit();
	//페이지 넘기면 커런트 이미지 바낌. or 다른 인트 변수 만들어서 페이지 넘기면 idX랑 idY에 프레임 크기만큼 더해서씀.
	_currentImgNum = IMG_TILE;
	_currentImg = IMAGEMANAGER->findImage("tile");
	_currentPage = PAGE_TILE;

	//기본 사이즈
	_tileSizeX = DEFAULT_MAP_SIZE_X;
	_tileSizeY = DEFAULT_MAP_SIZE_Y;
	_mapNum = 1;
	_count = 0;
	_isChanged = false;
	_isTestGoldBonus = false;
	_testGoldBonus = 0;
	//그릴영역 기초설정
	mapInit();
	//옵젝영역 기초설정
	objInit();
	//샘플영역 기초설정
	sampleInit();
	//기능버튼 기초설정
	buttonInit();

	//SOUNDMANAGER->addSound("mapTool", "sounds/mapTool/boss_9.ogg", true, true);
	//SOUNDMANAGER->addSound("mapTool_shopkeeper", "sounds/mapTool/boss_9_vocal.ogg", true, true);
	
	SOUNDMANAGER->playZone("mapTool", 0.05f);

	_floatingVal = 0;

	return S_OK;
}

void mapEditor::release()
{
}

void mapEditor::update()
{
	blinkMapTileFunc();
	corsorMapIdxCal();

	if (_isShow_Sample)
	{
		mapSizeChange();
		mapSaveBtn();
		mapLoadBtn();
		mapIdxChange();
		eraseMode();
		objEraseMode();
		categoryFunc();

	}

	sampleFunc();
	cameraFunc();
	mapDragDraw();

	_torchCount++;
	if (_torchCount >= 8)
	{
		_torchCount = 0;
		_torchFrameY++;

		if (_torchFrameY == 4)
		{
			_torchFrameY = 0;
		}
	}
}


void mapEditor::render()
{	
	mapTileRender();
	objectTileRender();
			
	if (_isShow_Sample)
	{
		sampleTileRender();

		buttonRender();
		
	}

	linePreview();

	testText();
}

//init----------------------------------------------------------------------------------------------------------------------

void mapEditor::imageInit()
{
	//샘플이미지
	IMAGEMANAGER->addFrameImage("tile", L"images/mapTool/tile.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("wall", L"images/mapTool/wall.png", 432, 576, 9, 6);
	IMAGEMANAGER->addFrameImage("item", L"images/mapTool/item.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy1", L"images/mapTool/mob1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy2", L"images/mapTool/mob2.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy3", L"images/mapTool/mob3.png", 432, 576, 3, 4);
	IMAGEMANAGER->addFrameImage("enemy4", L"images/mapTool/mob4.png", 432, 576, 3, 6);
	IMAGEMANAGER->addFrameImage("etc1", L"images/mapTool/etc1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("etc2", L"images/mapTool/etc2.png", 432, 576, 3, 6);

	//버튼 이미지

	//==========================
	//			카테고리
	//==========================
	IMAGEMANAGER->addFrameImage("btn_Tile", L"images/mapTool/button/btn_Tile.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Object", L"images/mapTool/button/btn_Object.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Item", L"images/mapTool/button/btn_Item.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Enemy", L"images/mapTool/button/btn_Enemy.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Etc", L"images/mapTool/button/btn_Etc.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Next", L"images/mapTool/button/btn_Next.png", 30, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Prev", L"images/mapTool/button/btn_Prev.png", 30, 60, 1, 2);
	//==========================
	//			버튼
	//==========================
	IMAGEMANAGER->addFrameImage("btn_Erase", L"images/mapTool/button/btn_Erase.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_EraseObj", L"images/mapTool/button/btn_EraseObj.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Save", L"images/mapTool/button/btn_Save.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Load", L"images/mapTool/button/btn_Load.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Size", L"images/mapTool/button/btn_Size.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_MapIdx", L"images/mapTool/button/btn_MapIdx.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Arrow", L"images/mapTool/button/btn_Arrow.png", 40, 80, 1, 2);
	
}

void mapEditor::mapInit()
{
	//그릴영역 기초설정
	_draw_Area_Rc = { 0,0, WINSIZEX - TILESIZE, WINSIZEY - TILESIZE };
	_vvMap.clear();
	for (int i = 0; i < _tileSizeY; i++)
	{
		vLine tmpLine;
		tmpLine.clear();
		for (int j = 0; j < _tileSizeX; j++)
		{
			tile* tmpTile = new tile;
			setNewMapTile(tmpTile, j, i);			//함수씀
			tmpLine.push_back(tmpTile);
		}
		_vvMap.push_back(tmpLine);
	}
}

void mapEditor::setNewMapTile(tile * tile, int idX, int idY)
{
	POINT tmpIdx = { idX,idY };
	tile->setIdx(tmpIdx);

	tagTileSet tmpTileSet;
	tmpTileSet.attribute = TILE_GROUND;
	tile->setIsAvailMove(true);
	if (idX % 2 == 0)
	{
		if (idY % 2 == 0)
		{
			if (_isChanged)
			{
				tmpTileSet.frameX = 4;
			}
			else if (!_isChanged)
			{
				tmpTileSet.frameX = 1;
			}
			tmpTileSet.frameY = RND->getFromIntTo(0,2);
		}
		else if (idY % 2 == 1)
		{
			if (_isChanged)
			{
				tmpTileSet.frameX = 1;
			}
			else if (!_isChanged)
			{
				tmpTileSet.frameX = 4;
			}
			tmpTileSet.frameY = RND->getFromIntTo(0, 2);
		}
	}
	else if (idX % 2 == 1)
	{
		if (idY % 2 == 0)
		{
			if (_isChanged)
			{
				tmpTileSet.frameX = 1;
			}
			else if (!_isChanged)
			{
				tmpTileSet.frameX = 4;
			}
			tmpTileSet.frameY = RND->getFromIntTo(0, 2);
		}
		else if (idY % 2 == 1)
		{
			if (_isChanged)
			{
				tmpTileSet.frameX = 4;
			}
			else if (!_isChanged)
			{
				tmpTileSet.frameX = 1;
			}
			tmpTileSet.frameY = RND->getFromIntTo(0, 2);
		}
	}
	tmpTileSet.imgNum = IMG_TILE;
	tmpTileSet.img = IMAGEMANAGER->findImage("tile");
	tmpTileSet.pos.x = _draw_Area_Rc.left + TILESIZE / 2 + TILESIZE * idX;
	tmpTileSet.pos.y = _draw_Area_Rc.top + TILESIZE / 2 + TILESIZE * idY;
	tmpTileSet.rc = { (float)tmpTileSet.pos.x - TILESIZE / 2,
						(float)tmpTileSet.pos.y - TILESIZE / 2,
						(float)tmpTileSet.pos.x + TILESIZE / 2,
						(float)tmpTileSet.pos.y + TILESIZE / 2 };

	tile->setTileSet(tmpTileSet);
}

void mapEditor::objInit()
{
	_vvObj.clear();
	for (int i = 0; i < _tileSizeY; i++)
	{
		vLine tmpLine;
		tmpLine.clear();
		for (int j = 0; j < _tileSizeX; j++)
		{
			tile* tmpTile = new tile;
			setNewObjTile(tmpTile, j, i);
			tmpLine.push_back(tmpTile);
		}
		_vvObj.push_back(tmpLine);
	}
}

void mapEditor::setNewObjTile(tile * tile, int idX, int idY)
{
	//최초 초기화작업. 다 빈값으로 넣어주자.
	POINT tmpIdx = { idX,idY };
	tile->setIdx(tmpIdx);

	tagTileSet tmpTileSet;
	tmpTileSet.attribute = OBJ_NONE;
	tile->setIsAvailMove(true);		
	tmpTileSet.frameX = NULL;
	tmpTileSet.frameY = NULL;
	tmpTileSet.imgNum = IMG_NONE;
	tmpTileSet.img = nullptr;	
	tmpTileSet.pos.x = _draw_Area_Rc.left + TILESIZE / 2 + TILESIZE * idX;
	tmpTileSet.pos.y = _draw_Area_Rc.top + TILESIZE / 2 + TILESIZE * idY;
	tmpTileSet.rc = { (float)tmpTileSet.pos.x - TILESIZE / 2,
						(float)tmpTileSet.pos.y - TILESIZE / 2,
						(float)tmpTileSet.pos.x + TILESIZE / 2,
						(float)tmpTileSet.pos.y + TILESIZE / 2 };

	tile->setTileSet(tmpTileSet);
}

void mapEditor::sampleInit()
{
	//샘플영역 기초설정
	_isShow_Sample = false;
	_isCollision_SampleTile = false;
	_sample_Area_Pos = { 100,100 };	//LT
	_sample_Area_Rc = { (float)_sample_Area_Pos.x - TILESIZE,
				  (float)_sample_Area_Pos.y - TILESIZE,
				  (float)_sample_Area_Pos.x + TILESIZE + _currentImg->GetWidth(),
				  (float)_sample_Area_Pos.y + TILESIZE + _currentImg->GetHeight() + 100 };


	//_corsor_Sample_Select[0].x = 1;
	//_corsor_Sample_Select[0].y = 0;
	//_corsor_Sample_Select[1].x = 1;
	//_corsor_Sample_Select[1].y = 0;
	_corsor_Sample_Select.x = 1;
	_corsor_Sample_Select.y = 0;

	for (int i = 0; i <= _currentImg->GetMaxFrameY(); i++)
	{
		for (int j = 0; j <= _currentImg->GetMaxFrameX(); j++)
		{
			delete _sampleTile[i][j];
			_sampleTile[i][j] = new tile;
			setNewSampleTile(_sampleTile[i][j], j, i);		//함수씀

		}
	}
}

void mapEditor::setNewSampleTile(tile * tile, int idX, int idY)
{
	POINT tmpIdx = { idX, idY };
	tile->setIdx(tmpIdx);

	tagTileSet tmpTileSet;
	if ((idX == 1 || idX == 4 || idX == 7) && (idY == 0 || idY == 4 || idY == 8))
	{
		tmpTileSet.imgNum = _currentImgNum;
		tmpTileSet.img = _currentImg;
		tmpTileSet.frameX = idX;
		tmpTileSet.frameY = idY;
		switch (idX)
		{
		case 1:
			switch (idY)
			{
			case 0:
				tmpTileSet.attribute = TILE_GROUND;
				tile->setIsAvailMove(true);
				break;
			case 4:
				tmpTileSet.attribute = TILE_BOSS_GROUND;
				tile->setIsAvailMove(true);
				break;
			case 8:
				tmpTileSet.attribute = TILE_UNLOCK_FLOOR;
				tile->setIsAvailMove(true);
				break;
			}
			break;
		case 4:
			switch (idY)
			{
			case 0:
				tmpTileSet.attribute = TILE_GROUND;
				tile->setIsAvailMove(true);
				break;
			case 4:
				tmpTileSet.attribute = TILE_BOSS_GROUND;
				tile->setIsAvailMove(true);
				break;
			case 8:
				tmpTileSet.attribute = TILE_LOCK_FLOOR;
				tile->setIsAvailMove(true);
				break;
			}
			break;
		case 7:
			switch (idY)
			{
			case 0:
				tmpTileSet.attribute = TILE_SHOP;
				tile->setIsAvailMove(true);
				break;
			case 4:
				tmpTileSet.attribute = TILE_WATER;
				tile->setIsAvailMove(true);
				break;
			case 8:
				tmpTileSet.attribute = TILE_BOSS_FLOOR;
				tile->setIsAvailMove(true);
				break;
			}
			break;
		}
	}
	else
	{
		tmpTileSet.imgNum = IMG_NONE;
		tmpTileSet.img = nullptr;
		tmpTileSet.frameX = NULL;
		tmpTileSet.frameY = NULL;
		tmpTileSet.attribute = TILE_NONE;		//이거 해야댐 -> 이프문으로... 물, 계단 등등
		tile->setIsAvailMove(true);		//이것도 일단 갈수있는걸로하고 나중에 이프문으로..
	}
	tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
	tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
	tmpTileSet.rc = { (float)tmpTileSet.pos.x,
						(float)tmpTileSet.pos.y,
						(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
						(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

	tile->setTileSet(tmpTileSet);
}

void mapEditor::buttonInit()
{
	//===========================================
	//			카테고리 버튼 초기화
	//===========================================
	for (int i = 0; i < 5; i++)
	{
		_btn_Category[i].pos = { _sample_Area_Pos.x - TILESIZE + i * 110, _sample_Area_Pos.y - TILESIZE };

		_btn_Category[i].rc = { (float)_btn_Category[i].pos.x,
							(float)_btn_Category[i].pos.y,
							(float)_btn_Category[i].pos.x + 80,
							(float)_btn_Category[i].pos.y + 30 };
		switch (i)
		{
		case 0:
			_btn_Category[i].img = IMAGEMANAGER->findImage("btn_Tile");
			break;
		case 1:
			_btn_Category[i].img = IMAGEMANAGER->findImage("btn_Object");
			break;
		case 2:
			_btn_Category[i].img = IMAGEMANAGER->findImage("btn_Item");
			break;
		case 3:
			_btn_Category[i].img = IMAGEMANAGER->findImage("btn_Enemy");
			break;
		case 4:
			_btn_Category[i].img = IMAGEMANAGER->findImage("btn_Etc");
			break;
		}
		_btn_Category[i].frame = { 0,0 };
		if (i == 0)
		{
			_btn_Category[i].frame.y = 1;
		}
	}

	_btn_Page[0].pos = { _sample_Area_Pos.x - TILESIZE + 10, _sample_Area_Pos.y + 550 };
	_btn_Page[0].rc = {	(float)_btn_Page[0].pos.x,
									(float)_btn_Page[0].pos.y,
									(float)_btn_Page[0].pos.x + 30,
									(float)_btn_Page[0].pos.y + 30 };
	_btn_Page[0].img = IMAGEMANAGER->findImage("btn_Prev");
	_btn_Page[0].frame = { 0,0 };

	_btn_Page[1].pos = { _sample_Area_Pos.x - TILESIZE + 50, _sample_Area_Pos.y + 550 };
	_btn_Page[1].rc = {	(float)_btn_Page[1].pos.x,
									(float)_btn_Page[1].pos.y,
									(float)_btn_Page[1].pos.x + 30,
									(float)_btn_Page[1].pos.y + 30 };
	_btn_Page[1].img = IMAGEMANAGER->findImage("btn_Next");
	_btn_Page[1].frame = { 0,0 };
	//===========================================
	//				사이즈 버튼 초기화
	//===========================================
	_btn_SizeChange.pos = { _sample_Area_Pos.x + 30, _sample_Area_Pos.y + 600 };
	_btn_SizeChange.rc = { (float)_btn_SizeChange.pos.x,
						(float)_btn_SizeChange.pos.y,
						(float)_btn_SizeChange.pos.x + 80,
						(float)_btn_SizeChange.pos.y + 30 };
	_btn_SizeChange.img = IMAGEMANAGER->findImage("btn_Size");
	_btn_SizeChange.frame = { 0,0 };
	_btn_SizeArrow[0].pos = { _btn_SizeChange.pos.x - 50 ,_btn_SizeChange.pos.y - 5 };
	_btn_SizeArrow[1].pos = { _btn_SizeChange.pos.x + 90 ,_btn_SizeChange.pos.y - 5 };
	_btn_SizeArrow[2].pos = { _btn_SizeChange.pos.x + 20 ,_btn_SizeChange.pos.y - 50 };
	_btn_SizeArrow[3].pos = { _btn_SizeChange.pos.x + 20, _btn_SizeChange.pos.y + 40 };
	
	for (int i = 0; i < 4; i++)
	{
		_btn_SizeArrow[i].rc = { (float)_btn_SizeArrow[i].pos.x,
						(float)_btn_SizeArrow[i].pos.y,
						(float)_btn_SizeArrow[i].pos.x + 40,
						(float)_btn_SizeArrow[i].pos.y + 40 };
		_btn_SizeArrow[i].img = IMAGEMANAGER->findImage("btn_Arrow");
		_btn_SizeArrow[i].frame = { 0,0 };
	}
	//===========================================
	//			세이브 & 로드 초기화
	//===========================================
	_btn_Save.pos = { _sample_Area_Pos.x + 200, _sample_Area_Pos.y + 550 };
	_btn_Save.rc = {	(float)_btn_Save.pos.x,
					(float)_btn_Save.pos.y,
					(float)_btn_Save.pos.x + 80,
					(float)_btn_Save.pos.y + 30 };
	_btn_Save.img = IMAGEMANAGER->findImage("btn_Save");
	_btn_Save.frame = { 0,0 };
	_btn_Load.pos = { _sample_Area_Pos.x + 300, _sample_Area_Pos.y + 550 };
	_btn_Load.rc = {	(float)_btn_Load.pos.x,
					(float)_btn_Load.pos.y,
					(float)_btn_Load.pos.x + 80,
					(float)_btn_Load.pos.y + 30 };
	_btn_Load.img = IMAGEMANAGER->findImage("btn_Load");
	_btn_Load.frame = { 0,0 };
	//===========================================
	//			인덱스 변경버튼
	//===========================================
	_mapNum = 0;
	_btn_MapIdx.pos = { _sample_Area_Pos.x + 250, _sample_Area_Pos.y + 600 };
	_btn_MapIdx.rc = { (float)_btn_MapIdx.pos.x,
						 (float)_btn_MapIdx.pos.y,
						 (float)_btn_MapIdx.pos.x + 80,
						 (float)_btn_MapIdx.pos.y + 30 };
	_btn_MapIdx.img = IMAGEMANAGER->findImage("btn_MapIdx");
	_btn_MapIdx.frame = { 0,0 };

	_btn_MapIdxArrow[0].pos = { _sample_Area_Pos.x + 200,  _sample_Area_Pos.y + 650 };
	_btn_MapIdxArrow[1].pos = { _sample_Area_Pos.x + 335,  _sample_Area_Pos.y + 650 };

	for (int i = 0; i < 2; i++)
	{
		_btn_MapIdxArrow[i].rc = { (float)_btn_MapIdxArrow[i].pos.x,
							(float)_btn_MapIdxArrow[i].pos.y,
							(float)_btn_MapIdxArrow[i].pos.x + 40,
							(float)_btn_MapIdxArrow[i].pos.y + 40 };
		_btn_MapIdxArrow[i].img = IMAGEMANAGER->findImage("btn_Arrow");
		_btn_MapIdxArrow[i].frame = { 0,0 };
	}

	//===========================================
	//				지우개 버튼
	//===========================================
	_btn_Erase.pos = { _sample_Area_Pos.x + 390 , _sample_Area_Pos.y + 600 };
	_btn_Erase.rc= { (float)_btn_Erase.pos.x,
				 (float)_btn_Erase.pos.y,
				 (float)_btn_Erase.pos.x + 80,
				 (float)_btn_Erase.pos.y + 30 };
	_btn_Erase.img = IMAGEMANAGER->findImage("btn_Erase");
	_btn_Erase.frame = { 0,0 };

	_btn_ObjErase.pos = { _sample_Area_Pos.x + 390 , _sample_Area_Pos.y + 650 };
	_btn_ObjErase.rc = { (float)_btn_ObjErase.pos.x,
					 (float)_btn_ObjErase.pos.y,
					 (float)_btn_ObjErase.pos.x + 80,
					 (float)_btn_ObjErase.pos.y + 30 };
	_btn_ObjErase.img = IMAGEMANAGER->findImage("btn_EraseObj");
	_btn_ObjErase.frame = { 0,0 };

}

//update----------------------------------------------------------------------------------------------------------------------

void mapEditor::cameraFunc()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		CAMERA->setPosX(CAMERA->getPosX() - CAMERA->getCameraSpeed());
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		CAMERA->setPosY(CAMERA->getPosY() - CAMERA->getCameraSpeed());
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		CAMERA->setPosX(CAMERA->getPosX() + CAMERA->getCameraSpeed());
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		CAMERA->setPosY(CAMERA->getPosY() + CAMERA->getCameraSpeed());
	}
}

void mapEditor::blinkMapTileFunc()
{
	//현재 맵의 무슨타일에 있는지 인덱스 저장용
	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		_isTestGoldBonus == false ? _isTestGoldBonus = true : _isTestGoldBonus = false;
		if (!_isTestGoldBonus)
		{
			_testGoldBonus = 0;
		}
	}
	_count++;
	if (_count > 50)
	{
		if (_isTestGoldBonus)
		{
			if (_testGoldBonus == 0)
			{
				_testGoldBonus = 1;
			}
			else if (_testGoldBonus == 1)
			{
				_testGoldBonus = 2;
			}
			else if (_testGoldBonus == 2)
			{
				_testGoldBonus = 1;
			}
		}
		for (int i = 0; i < _tileSizeY; i++)
		{
			for (int j = 0; j < _tileSizeX; j++)
			{
				_vvMap[i][j]->update();
			}
		}
		_count = 0;
		if (_isChanged)
		{
			_isChanged = false;
		}
		else if (!_isChanged)
		{
			_isChanged = true;
		}
	}
}

void mapEditor::corsorMapIdxCal()
{
	_corsor_Map_Idx = { (int)(_ptMouse.x + CAMERA->getPosX()) / TILESIZE  , (int)(_ptMouse.y + CAMERA->getPosY()) / TILESIZE };

	if (_corsor_Map_Idx.x >= _vvMap[0].size() - 1)
	{
		_corsor_Map_Idx.x = _vvMap[0].size() - 1;
	}
	if (_corsor_Map_Idx.y >= _vvMap.size() - 1)
	{
		_corsor_Map_Idx.y = _vvMap.size() - 1;
	}
}
	
	//샘플

void mapEditor::sampleFunc()
{
	//텝키로 샘플타일 보이기 || 안보이기 전환
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_isShow_Sample == true ? _isShow_Sample = false : _isShow_Sample = true;
	}

	//보이면 
	if (_isShow_Sample)
	{
		//샘플타일전체 영역 내에서 빈공간이 아닌 샘플타일들 영역이면 렉트가 안움직여야함.
		_isCollision_SampleTile = false;	//계속 펄스로 만들어주고
		for (int i = 0; i <= _currentImg->GetMaxFrameY(); i++)
		{
			for (int j = 0; j <= _currentImg->GetMaxFrameX(); j++)
			{
				//이미지가 있는 영역에 마우스 커서가 올려져있으면 
				if (PtInRect(&rectMake(_sampleTile[i][j]->getRc()), pointMake(_ptMouse)) && _sampleTile[i][j]->getImg() != nullptr)
				{
					//샘플타일 위치 변경 못함.
					_isCollision_SampleTile = true;

					// + 해당 샘플을 커서에 담음.
					sampleSelect(j, i); 

					break;
				}
			}
		}
		//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆
		//☆★☆★☆★☆★☆★☆★☆   이동   ★☆★☆★☆★☆★☆★☆★
		//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆
		if (!_isCollision_SampleTile && _isCollision_Sample_Area_Rc && !_isCollision_Btn)
		{
			//샘플영역렉트와 충돌중인지 체크
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				_isClick = true;
				_sample_Move_Cal_Pt.x = _ptMouse.x - (int)_sample_Area_Rc.left - TILESIZE;
				_sample_Move_Cal_Pt.y = _ptMouse.y - (int)_sample_Area_Rc.top - TILESIZE;
			}
			else if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
			{
				_isClick = false;
			}

			//=====================================
			//				이동하는것
			//=====================================
			if (_isClick)
			{
				//=====================================
				//				이동 뼈대
				//=====================================
				_sample_Area_Pos = { (int)_ptMouse.x - _sample_Move_Cal_Pt.x, (int)_ptMouse.y - _sample_Move_Cal_Pt.y };
				_sample_Area_Rc = { (float)_sample_Area_Pos.x - TILESIZE,
							  (float)_sample_Area_Pos.y - TILESIZE,
							  (float)_sample_Area_Pos.x + TILESIZE + _currentImg->GetWidth(),
							  (float)_sample_Area_Pos.y + TILESIZE + _currentImg->GetHeight() + 100 };
				for (int i = 0; i <= _currentImg->GetMaxFrameY(); i++)
				{
					for (int j = 0; j <= _currentImg->GetMaxFrameX(); j++)
					{
						_sampleTile[i][j]->setIdx({ j,i });

						_sampleTile[i][j]->setPos({ (int)_sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * j,(int)_sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * i });
						_sampleTile[i][j]->setRc({ (float)_sampleTile[i][j]->getPos().x,
													(float)_sampleTile[i][j]->getPos().y,
													(float)_sampleTile[i][j]->getPos().x + _currentImg->GetFrameWidth(),
													(float)_sampleTile[i][j]->getPos().y + _currentImg->GetFrameHeight() });
					}
				}
				//=====================================
				//				카테고리 이동
				//=====================================
				for (int i = 0; i < 5; i++)
				{
					_btn_Category[i].pos = { _sample_Area_Pos.x - TILESIZE + i * 110, _sample_Area_Pos.y - TILESIZE };

					_btn_Category[i].rc = { (float)_btn_Category[i].pos.x,
										(float)_btn_Category[i].pos.y,
										(float)_btn_Category[i].pos.x + 80,
										(float)_btn_Category[i].pos.y + 30 };
				}
				_btn_Page[0].pos = { _sample_Area_Pos.x - TILESIZE + 10, _sample_Area_Pos.y + 550 };
				_btn_Page[0].rc = { (float)_btn_Page[0].pos.x,
												(float)_btn_Page[0].pos.y,
												(float)_btn_Page[0].pos.x + 30,
												(float)_btn_Page[0].pos.y + 30 };
				_btn_Page[1].pos = { _sample_Area_Pos.x - TILESIZE + 50, _sample_Area_Pos.y + 550 };
				_btn_Page[1].rc = { (float)_btn_Page[1].pos.x,
												(float)_btn_Page[1].pos.y,
												(float)_btn_Page[1].pos.x + 30,
												(float)_btn_Page[1].pos.y + 30 };
				//=====================================
				//			사이즈 버튼 움직임
				//=====================================
				_btn_SizeChange.pos = { _sample_Area_Pos.x + 30, _sample_Area_Pos.y + 600 };
				_btn_SizeChange.rc = {	(float)_btn_SizeChange.pos.x,
									(float)_btn_SizeChange.pos.y,
									(float)_btn_SizeChange.pos.x + 80,
									(float)_btn_SizeChange.pos.y + 30 };
				_btn_SizeArrow[0].pos = { _btn_SizeChange.pos.x - 50 ,_btn_SizeChange.pos.y - 5 };
				_btn_SizeArrow[1].pos = { _btn_SizeChange.pos.x + 90 ,_btn_SizeChange.pos.y - 5 };
				_btn_SizeArrow[2].pos = { _btn_SizeChange.pos.x + 20 ,_btn_SizeChange.pos.y - 50 };
				_btn_SizeArrow[3].pos = { _btn_SizeChange.pos.x + 20, _btn_SizeChange.pos.y + 40 };
				for (int i = 0; i < 4; i++)
				{
					_btn_SizeArrow[i].rc = { (float)_btn_SizeArrow[i].pos.x,
										 (float)_btn_SizeArrow[i].pos.y,
										 (float)_btn_SizeArrow[i].pos.x + 40,
										 (float)_btn_SizeArrow[i].pos.y + 40 };
				}
				//===========================================
				//			세이브 & 로드 이동
				//===========================================

				_btn_Save.pos = { _sample_Area_Pos.x + 200, _sample_Area_Pos.y + 550 };
				_btn_Save.rc = {	(float)_btn_Save.pos.x,
								(float)_btn_Save.pos.y,
								(float)_btn_Save.pos.x + 80,
								(float)_btn_Save.pos.y + 30 };
				_btn_Load.pos = { _sample_Area_Pos.x + 300, _sample_Area_Pos.y + 550 };
				_btn_Load.rc = {	(float)_btn_Load.pos.x,
								(float)_btn_Load.pos.y,
								(float)_btn_Load.pos.x + 80,
								(float)_btn_Load.pos.y + 30 };
				//===========================================
				//			인덱스 변경 이동
				//===========================================
				_btn_MapIdx.pos = { _sample_Area_Pos.x + 250, _sample_Area_Pos.y + 600 };
				_btn_MapIdx.rc = {	(float)_btn_MapIdx.pos.x,
								(float)_btn_MapIdx.pos.y,
								(float)_btn_MapIdx.pos.x + 80,
								(float)_btn_MapIdx.pos.y + 30 };
				_btn_MapIdxArrow[0].pos = { _sample_Area_Pos.x + 200,  _sample_Area_Pos.y + 650 };
				_btn_MapIdxArrow[1].pos = { _sample_Area_Pos.x + 335,  _sample_Area_Pos.y + 650 };
				for (int i = 0; i < 2; i++)
				{
					_btn_MapIdxArrow[i].rc = {  (float)_btn_MapIdxArrow[i].pos.x,
											(float)_btn_MapIdxArrow[i].pos.y,
											(float)_btn_MapIdxArrow[i].pos.x + 40,
											(float)_btn_MapIdxArrow[0].pos.y + 40 };
				}
				//==========================================
				//			지우개 이동
				//==========================================
				_btn_Erase.pos = { _sample_Area_Pos.x + 390 , _sample_Area_Pos.y + 600 };
				_btn_Erase.rc = { (float)_btn_Erase.pos.x,
							 (float)_btn_Erase.pos.y,
							 (float)_btn_Erase.pos.x + 80,
							 (float)_btn_Erase.pos.y + 30 };
				_btn_ObjErase.pos = { _sample_Area_Pos.x + 390 , _sample_Area_Pos.y + 650 };
				_btn_ObjErase.rc = { (float)_btn_ObjErase.pos.x,
								 (float)_btn_ObjErase.pos.y,
								 (float)_btn_ObjErase.pos.x + 80,
								 (float)_btn_ObjErase.pos.y + 30 };
				//==========================================
				//
				//==========================================
			}
		}
	}
	else if (!_isShow_Sample)
	{
		//샘플 보기모드가 아니라면 샘플타일과 충돌 false
		_isCollision_SampleTile = false;
	}
}

void mapEditor::sampleSelect(int idX, int idY)
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//자 인덱스 찝는것까지 됐으니 이제 맵에 바르는거. + 오브젝트 등등 생각 잘하면서 하자.
		//_corsor_Sample_Select[0] = { _sampleTile[idY][idX]->getIdx().x , _sampleTile[idY][idX]->getIdx().y };
		//_corsor_Sample_Select[1] = { _sampleTile[idY][idX]->getIdx().x , _sampleTile[idY][idX]->getIdx().y };
		//_isClick = true;
		_corsor_Sample_Select = { _sampleTile[idY][idX]->getIdx().x , _sampleTile[idY][idX]->getIdx().y };
	}
	//if (_isClick)
	//{
	//	_corsor_Sample_Select[1] = { _sampleTile[idY][idX]->getIdx().x , _sampleTile[idY][idX]->getIdx().y };
	//}
	//if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	//{
	//	_isClick = false;
	//}
}

void mapEditor::setMapTile(tile* tile, int idX, int idY)
{
	if (_sampleTile[idY][idX]->getImg() == nullptr) return;

	tile->setAttribute(_sampleTile[idY][idX]->getAttribute());

	if (tile->getAttribute() == TILE_GROUND)
	{
		if (tile->getIdx().x % 2 == 0)
		{
			if (tile->getIdx().y % 2 == 0)
			{
				if (_isChanged)
				{
					tile->setFrameX(4);
				}
				else if(!_isChanged)
				{
					tile->setFrameX(1);
				}
			}
			else if (tile->getIdx().y % 2 == 1)
			{
				if (_isChanged)
				{
					tile->setFrameX(1);
				}
				else if (!_isChanged)
				{
					tile->setFrameX(4);
				}
			}
		}
		else if (tile->getIdx().x % 2 == 1)
		{
			if (tile->getIdx().y % 2 == 0)
			{
				if (_isChanged)
				{
					tile->setFrameX(1);
				}
				else if (!_isChanged)
				{
					tile->setFrameX(4);
				}
			}
			else if (tile->getIdx().y % 2 == 1)
			{
				if (_isChanged)
				{
					tile->setFrameX(4);
				}
				else if (!_isChanged)
				{
					tile->setFrameX(1);
				}
			}
		}
		tile->setFrameY(RND->getFromIntTo(0, 2));
	}
	else if (tile->getAttribute() == TILE_BOSS_GROUND)
	{
		if (tile->getIdx().x % 2 == 0)
		{
			if (tile->getIdx().y % 2 == 0)
			{
				if (_isChanged)
				{
					tile->setFrameX(4);
				}
				else if (!_isChanged)
				{
					tile->setFrameX(1);
				}
			}
			else if (tile->getIdx().y % 2 == 1)
			{
				if (_isChanged)
				{
					tile->setFrameX(1);
				}
				else if (!_isChanged)
				{
					tile->setFrameX(4);
				}
			}
		}
		else if (tile->getIdx().x % 2 == 1)
		{
			if (tile->getIdx().y % 2 == 0)
			{
				if (_isChanged)
				{
					tile->setFrameX(1);
				}
				else if (!_isChanged)
				{
					tile->setFrameX(4);
				}
			}
			else if (tile->getIdx().y % 2 == 1)
			{
				if (_isChanged)
				{
					tile->setFrameX(4);
				}
				else if (!_isChanged)
				{
					tile->setFrameX(1);
				}
			}
		}
		tile->setFrameY(4);
	}
	else
	{
		tile->setFrameX(_sampleTile[idY][idX]->getFrameX());
		tile->setFrameY(_sampleTile[idY][idX]->getFrameY());
	}
	tile->setIsAvailMove(_sampleTile[idY][idX]->getIsAvailMove());
	tile->setImgNum(_sampleTile[idY][idX]->getImgNum());
	tile->setImg(_sampleTile[idY][idX]->getImg());
}

void mapEditor::setMapObject(tile * tile, int idX, int idY)
{
	if (_sampleTile[idY][idX]->getImg() == nullptr) return;
	tile->setAttribute(_sampleTile[idY][idX]->getAttribute());
	if (_sampleTile[idY][idX]->getAttribute() == OBJ_WALL1 || _sampleTile[idY][idX]->getAttribute() == ETC_TORCH_WALL1)
	{
		tile->setFrameX(_sampleTile[RND->getFromIntTo(0, 2)][RND->getFromIntTo(0, 3)]->getIdx().x);
		tile->setFrameY(_sampleTile[RND->getFromIntTo(0, 2)][RND->getFromIntTo(0, 3)]->getIdx().y);
	}
	else if (_sampleTile[idY][idX]->getAttribute() == OBJ_WALL_BOSS || _sampleTile[idY][idX]->getAttribute() == ETC_TORCH_BOSS)
	{
		tile->setFrameX(_sampleTile[RND->getFromIntTo(2, 4)][RND->getFromIntTo(3, 5)]->getIdx().x);
		tile->setFrameY(_sampleTile[RND->getFromIntTo(2, 4)][RND->getFromIntTo(3, 5)]->getIdx().y);
	}
	else
	{
		tile->setFrameX(_sampleTile[idY][idX]->getFrameX());
		tile->setFrameY(_sampleTile[idY][idX]->getFrameY());
	}
	

	tile->setIsAvailMove(_sampleTile[idY][idX]->getIsAvailMove());
	tile->setImgNum(_sampleTile[idY][idX]->getImgNum());
	tile->setImg(_sampleTile[idY][idX]->getImg());
}

	//카테고리

void mapEditor::categoryFunc()
{
	for (int i = 0; i < 5; i++)
	{
		//카테고리 렉트와 충돌중이고
		if (PtInRect(&rectMake(_btn_Category[i].rc), pointMake(_ptMouse)))
		{
			//클릭을 했으면
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				//현재 샘플로 선택해논 값을 원래의 갯수인 xMap == 9 yMap == 12로 비율에맞춰 곱해놓고
				switch (_currentImgNum)
				{
				case IMG_TILE:
					_corsor_Sample_Select.x *= 1;
					_corsor_Sample_Select.y *= 1;
					break;
				case IMG_WALL:
					_corsor_Sample_Select.x *= 1;
					_corsor_Sample_Select.y *= 2;
					break;
				case IMG_ITEM:
					_corsor_Sample_Select.x *= 1;
					_corsor_Sample_Select.y *= 1;
					break;
				case IMG_ENEMY1:
					_corsor_Sample_Select.x *= 1;
					_corsor_Sample_Select.y *= 1;
					break;
				case IMG_ENEMY2:
					_corsor_Sample_Select.x *= 1;
					_corsor_Sample_Select.y *= 1;
					break;
				case IMG_ENEMY3:
					_corsor_Sample_Select.x *= 3;
					_corsor_Sample_Select.y *= 3;
					break;
				case IMG_ENEMY4:
					_corsor_Sample_Select.x *= 3;
					_corsor_Sample_Select.y *= 2;
					break;
				case IMG_ETC1:
					_corsor_Sample_Select.x *= 1;
					_corsor_Sample_Select.y *= 1;
					break;
				case IMG_ETC2:
					break;
					_corsor_Sample_Select.x *= 3;
					_corsor_Sample_Select.y *= 2;
				}

				_btn_Category[_currentPage].frame.y = 0;
				//해당거에 맞는 값으로 현재 페이지 변경
				_currentPage = (tagPage)i;

				_btn_Category[_currentPage].frame.y = 1;

				//누른 카테고리에 맞게 이미지도 변경해줌.
				switch (_currentPage)
				{
				case PAGE_TILE:
					_currentImgNum = IMG_TILE;
					break;
				case PAGE_WALL:
					_currentImgNum = IMG_WALL;
					break;
				case PAGE_ITEM:
					_currentImgNum = IMG_ITEM;
					break;
				case PAGE_ENEMY:
					_currentImgNum = IMG_ENEMY1;
					break;
				case PAGE_ETC:
					_currentImgNum = IMG_ETC1;
					break;
				}

				switch (_currentImgNum)
				{
				case IMG_TILE:
					_corsor_Sample_Select.x /= 1;
					_corsor_Sample_Select.y /= 1;
					_currentPage = PAGE_TILE;
					_currentImg = IMAGEMANAGER->findImage("tile");
					break;
				case IMG_WALL:
					_corsor_Sample_Select.x /= 1;
					_corsor_Sample_Select.y /= 2;
					_currentPage = PAGE_WALL;
					_currentImg = IMAGEMANAGER->findImage("wall");
					break;
				case IMG_ITEM:
					_corsor_Sample_Select.x /= 1;
					_corsor_Sample_Select.y /= 1;
					_currentPage = PAGE_ITEM;
					_currentImg = IMAGEMANAGER->findImage("item");
					break;
				case IMG_ENEMY1:
					_corsor_Sample_Select.x /= 1;
					_corsor_Sample_Select.y /= 1;
					_currentPage = PAGE_ENEMY;
					_currentImg = IMAGEMANAGER->findImage("enemy1");
					break;
				case IMG_ENEMY2:
					_corsor_Sample_Select.x /= 1;
					_corsor_Sample_Select.y /= 1;
					_currentPage = PAGE_ENEMY;
					_currentImg = IMAGEMANAGER->findImage("enemy2");
					break;
				case IMG_ENEMY3:
					_corsor_Sample_Select.x /= 3;
					_corsor_Sample_Select.y /= 3;
					_currentPage = PAGE_ENEMY;
					_currentImg = IMAGEMANAGER->findImage("enemy3");
					break;
				case IMG_ENEMY4:
					_corsor_Sample_Select.x /= 3;
					_corsor_Sample_Select.y /= 2;
					_currentPage = PAGE_ENEMY;
					_currentImg = IMAGEMANAGER->findImage("enemy4");
					break;
				case IMG_ETC1:
					_corsor_Sample_Select.x /= 1;
					_corsor_Sample_Select.y /= 1;
					_currentPage = PAGE_ETC;
					_currentImg = IMAGEMANAGER->findImage("etc1");
					break;
				case IMG_ETC2:
					_corsor_Sample_Select.x /= 3;
					_corsor_Sample_Select.y /= 2;
					_currentPage = PAGE_ETC;
					_currentImg = IMAGEMANAGER->findImage("etc2");
				}

				//이미지 변경했으니 이미지 재적용
				for (int i = 0; i <= _currentImg->GetMaxFrameY(); i++)
				{
					for (int j = 0; j <= _currentImg->GetMaxFrameX(); j++)
					{
						delete _sampleTile[i][j];
						_sampleTile[i][j] = new tile;
						changeCategory(_sampleTile[i][j], j, i);
					}
				}
			}
		}
	}
	//prev버튼 눌럿을떄
	if (PtInRect(&rectMake(_btn_Page[0].rc), pointMake(_ptMouse)))
	{
		_btn_Page[0].frame.y = 1;
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			//현재 샘플로 선택해논 값을 원래의 갯수인 xMap == 9 yMap == 12로 비율에맞춰 곱해놓고
			switch (_currentImgNum)
			{
			case IMG_TILE:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_WALL:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 2;
				break;
			case IMG_ITEM:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY1:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY2:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY3:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY4:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ETC1:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ETC2:
				_corsor_Sample_Select.x *= 3;
				_corsor_Sample_Select.y *= 2;
				break;
			}

			int calImgNum;
			calImgNum = (int)_currentImgNum;
			calImgNum -= 1;
			if (calImgNum == IMG_NONE)
			{
				calImgNum = IMG_END - 1;
			}
			else if (calImgNum == IMG_END)
			{
				calImgNum = IMG_NONE + 1;
			}
			_currentImgNum = (tagImgNum)calImgNum;


			_btn_Category[_currentPage].frame.y = 0;

			switch (_currentImgNum)
			{
			case IMG_TILE:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_TILE;
				_currentImg = IMAGEMANAGER->findImage("tile");
				break;
			case IMG_WALL:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 2;
				_currentPage = PAGE_WALL;
				_currentImg = IMAGEMANAGER->findImage("wall");
				break;
			case IMG_ITEM:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ITEM;
				_currentImg = IMAGEMANAGER->findImage("item");
				//임시로 에너미 가게했음
				break;
			case IMG_ENEMY1:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy1");
				break;
			case IMG_ENEMY2:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy2");
				break;
			case IMG_ENEMY3:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy3");
				break;
			case IMG_ENEMY4:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy4");
				break;
			case IMG_ETC1:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ETC;
				_currentImg = IMAGEMANAGER->findImage("etc1");
				break;
			case IMG_ETC2:
				_corsor_Sample_Select.x /= 3;
				_corsor_Sample_Select.y /= 2;
				_currentPage = PAGE_ETC;
				_currentImg = IMAGEMANAGER->findImage("etc2");
				break;
			}
			_btn_Category[_currentPage].frame.y = 1;

			//이미지 변경했으니 이미지 재적용
			for (int i = 0; i <= _currentImg->GetMaxFrameY(); i++)
			{
				for (int j = 0; j <= _currentImg->GetMaxFrameX(); j++)
				{
					delete _sampleTile[i][j];
					_sampleTile[i][j] = new tile;
					changeCategory(_sampleTile[i][j], j, i);
				}
			}
		}
	}
	else
	{
		_btn_Page[0].frame.y = 0;
	}

	//next버튼 눌럿을떄
	if (PtInRect(&rectMake(_btn_Page[1].rc), pointMake(_ptMouse)))
	{
		_btn_Page[1].frame.y = 1;
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			//현재 샘플로 선택해논 값을 원래의 갯수인 xMap == 9 yMap == 12로 비율에맞춰 곱해놓고
			switch (_currentImgNum)
			{
			case IMG_TILE:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_WALL:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 2;
				break;
			case IMG_ITEM:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY1:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY2:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY3:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ENEMY4:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ETC1:
				_corsor_Sample_Select.x *= 1;
				_corsor_Sample_Select.y *= 1;
				break;
			case IMG_ETC2:
				_corsor_Sample_Select.x *= 3;
				_corsor_Sample_Select.y *= 2;
				break;
			}

			int calImgNum;
			calImgNum = (int)_currentImgNum;
			calImgNum += 1;
			if (calImgNum == IMG_NONE)
			{
				calImgNum = IMG_END - 1;
			}
			else if (calImgNum == IMG_END)
			{
				calImgNum = IMG_NONE + 1;
			}
			_currentImgNum = (tagImgNum)calImgNum;


			_btn_Category[_currentPage].frame.y = 0;

			switch (_currentImgNum)
			{
			case IMG_TILE:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_TILE;
				_currentImg = IMAGEMANAGER->findImage("tile");
				break;
			case IMG_WALL:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 2;
				_currentPage = PAGE_WALL;
				_currentImg = IMAGEMANAGER->findImage("wall");
				break;
			case IMG_ITEM:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ITEM;
				_currentImg = IMAGEMANAGER->findImage("item");
				//임시로 에너미 가게했음
				break;
			case IMG_ENEMY1:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy1");
				break;
			case IMG_ENEMY2:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy2");
				break;
			case IMG_ENEMY3:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy3");
				break;
			case IMG_ENEMY4:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ENEMY;
				_currentImg = IMAGEMANAGER->findImage("enemy4");
				break;
			case IMG_ETC1:
				_corsor_Sample_Select.x /= 1;
				_corsor_Sample_Select.y /= 1;
				_currentPage = PAGE_ETC;
				_currentImg = IMAGEMANAGER->findImage("etc1");
				break;
			case IMG_ETC2:
				_corsor_Sample_Select.x /= 3;
				_corsor_Sample_Select.y /= 2;
				_currentPage = PAGE_ETC;
				_currentImg = IMAGEMANAGER->findImage("etc2");
				break;
			}
			_btn_Category[_currentPage].frame.y = 1;

			//이미지 변경했으니 이미지 재적용
			for (int i = 0; i <= _currentImg->GetMaxFrameY(); i++)
			{
				for (int j = 0; j <= _currentImg->GetMaxFrameX(); j++)
				{
					delete _sampleTile[i][j];
					_sampleTile[i][j] = new tile;
					changeCategory(_sampleTile[i][j], j, i);
				}
			}
		}
	}
	else
	{
		_btn_Page[1].frame.y = 0;
	}

}

void mapEditor::changeCategory(tile* tile, int idX, int idY)
{
	POINT tmpIdx = { idX, idY };
	tagTileSet tmpTileSet;

	switch (_currentImgNum)
	{
	case IMG_TILE:
		tile->setIdx(tmpIdx);

		if ((idX == 1 || idX == 4 || idX == 7) && (idY == 0 || idY == 4 || idY == 8))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			switch (idX)
			{
			case 1:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = TILE_GROUND;
					tile->setIsAvailMove(true);
					break;
				case 4:
					tmpTileSet.attribute = TILE_BOSS_GROUND;
					tile->setIsAvailMove(true);
					break;
				case 8:
					tmpTileSet.attribute = TILE_UNLOCK_FLOOR;
					tile->setIsAvailMove(true);
					break;
				}
				break;
			case 4:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = TILE_GROUND;
					tile->setIsAvailMove(true);
					break;
				case 4:
					tmpTileSet.attribute = TILE_BOSS_GROUND;
					tile->setIsAvailMove(true);
					break;
				case 8:
					tmpTileSet.attribute = TILE_LOCK_FLOOR;
					tile->setIsAvailMove(true);
					break;
				}
				break;
			case 7:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = TILE_SHOP;
					tile->setIsAvailMove(true);
					break;
				case 4:
					tmpTileSet.attribute = TILE_WATER;
					tile->setIsAvailMove(true);
					break;
				case 8:
					tmpTileSet.attribute = TILE_BOSS_FLOOR;
					tile->setIsAvailMove(true);
					break;
				}
				break;
			}
		}
		else
		{
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
			tmpTileSet.attribute = TILE_NONE;		//이거 해야댐 -> 이프문으로... 물, 계단 등등
			tile->setIsAvailMove(true);		//이것도 일단 갈수있는걸로하고 나중에 이프문으로..
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);

		break;
	case IMG_WALL:
		tile->setIdx(tmpIdx);


		if ((idX == 1 || idX == 4 || idX == 7) && (idY == 0 || idY == 2 || idY == 4))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			switch (idX)
			{
			case 1:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = OBJ_WALL1;
					tile->setIsAvailMove(false);
					break;
				case 2:
					tmpTileSet.attribute = OBJ_WALL_GOLD;
					tile->setIsAvailMove(false);
					break;
				case 4:
					tmpTileSet.attribute = OBJ_DOOR_FRONT;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			case 4:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = OBJ_WALL2;
					tile->setIsAvailMove(false);
					break;
				case 2:
					tmpTileSet.attribute = OBJ_WALL_BOSS;
					tile->setIsAvailMove(false);
					break;
				case 4:
					tmpTileSet.attribute = OBJ_DOOR_SIDE;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			case 7:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = OBJ_WALL3;
					tile->setIsAvailMove(false);
					break;
				case 2:
					tmpTileSet.attribute = OBJ_WALL_END;
					tile->setIsAvailMove(false);
					break;
				case 4:
					tmpTileSet.attribute = OBJ_NONE;
					tile->setIsAvailMove(true);
					tmpTileSet.imgNum = IMG_NONE;
					tmpTileSet.img = nullptr;
					tmpTileSet.frameX = NULL;
					tmpTileSet.frameY = NULL;
					break;
				}
				break;
			}
		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);

		break;
	case IMG_ITEM:
		tile->setIdx(tmpIdx);
		if ((idX == 0 || idX == 2 || idX == 4 || idX == 6 || idX == 8) && (idY == 0 || idY == 2 || idY == 4 || idY == 6 || idY == 8 || idY == 10))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			tile->setIsAvailMove(true);
			//아이템 속성 넣고있었음
			switch (idY)
			{
			case 0:
				switch (idX)
				{
				case 0:
					tmpTileSet.attribute = ITEM_SPEAR_NORMAL;
					break;
				case 2:
					tmpTileSet.attribute = ITEM_SPEAR_BLOOD;
					break;
				case 4:
					tmpTileSet.attribute = ITEM_SPEAR_TITANIUM;
					break;
				case 6:
					tmpTileSet.attribute = ITEM_SPEAR_GLASS;
					break;
				case 8:
					tmpTileSet.attribute = ITEM_SPEAR_OBSIDIAN;
					break;
				}
				break;
			case 2:
				switch (idX)
				{
				case 0:
					tmpTileSet.attribute = ITEM_SWORD_NORMAL;
					break;
				case 2:
					tmpTileSet.attribute = ITEM_SWORD_BLOOD;
					break;
				case 4:
					tmpTileSet.attribute = ITEM_SWORD_TITANIUM;
					break;
				case 6:
					tmpTileSet.attribute = ITEM_SWORD_GLASS;
					break;
				case 8:
					tmpTileSet.attribute = ITEM_SWORD_OBSIDIAN;
					break;
				}
				break;
			case 4:
				switch (idX)
				{
				case 0:
					tmpTileSet.attribute = ITEM_TORCH_1;
					break;
				case 2:
					tmpTileSet.attribute = ITEM_TORCH_2;
					break;
				case 4:
					tmpTileSet.attribute = ITEM_TORCH_3;
					break;
				case 6:
					tmpTileSet.attribute = ITEM_TORCH_GLASS;
					break;
				case 8:
					tmpTileSet.attribute = ITEM_TORCH_OBSIDIAN;
					break;
				}
				break;
			case 6:
				switch (idX)
				{
				case 0:
					tmpTileSet.attribute = ITEM_SHOVEL_NORMAL;
					break;
				case 2:
					tmpTileSet.attribute = ITEM_SHOVEL_BLOOD;
					break;
				case 4:
					tmpTileSet.attribute = ITEM_SHOVEL_TITANIUM;
					break;
				case 6:
					tmpTileSet.attribute = ITEM_SHOVEL_GLASS;
					break;
				case 8:
					tmpTileSet.attribute = ITEM_SHOVEL_OBSIDIAN;
					break;
				}
				break;
			case 8:
				switch (idX)
				{
				case 0:
					tmpTileSet.attribute = ITEM_ARMOR_1;
					break;
				case 2:
					tmpTileSet.attribute = ITEM_ARMOR_2;
					break;
				case 4:
					tmpTileSet.attribute = ITEM_ARMOR_3;
					break;
				case 6:
					tmpTileSet.attribute = ITEM_ARMOR_GLASS;
					break;
				case 8:
					tmpTileSet.attribute = ITEM_ARMOR_OBSIDIAN;
					break;
				}
				break;
			case 10:
				switch (idX)
				{
				case 0:
					tmpTileSet.attribute = ITEM_HP_APPLE;
					break;
				case 2:
					tmpTileSet.attribute = ITEM_HP_CHEESE;
					break;
				case 4:
					tmpTileSet.attribute = ITEM_HP_MEAT;
					break;
				case 6:
					tmpTileSet.attribute = ITEM_GLASS;
					break;
				case 8:
					tmpTileSet.attribute = ITEM_DAGGER;
					break;
				}
				break;
			}


		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);

		break;
	case IMG_ENEMY1:
		tile->setIdx(tmpIdx);

		if ((idX == 1 || idX == 4 || idX == 7) && (idY == 0 || idY == 4 || idY == 8))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			switch (idX)
			{
			case 1:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = ENEMY1_1_1;
					tile->setIsAvailMove(false);
					break;
				case 4:
					tmpTileSet.attribute = ENEMY1_2_1;
					tile->setIsAvailMove(false);
					break;
				case 8:
					tmpTileSet.attribute = ENEMY1_3_1;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			case 4:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = ENEMY1_1_2;
					tile->setIsAvailMove(false);
					break;
				case 4:
					tmpTileSet.attribute = ENEMY1_2_2;
					tile->setIsAvailMove(false);
					break;
				case 8:
					tmpTileSet.attribute = ENEMY1_3_2;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			case 7:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = OBJ_NONE;
					tile->setIsAvailMove(true);
					tmpTileSet.imgNum = IMG_NONE;
					tmpTileSet.img = nullptr;
					tmpTileSet.frameX = NULL;
					tmpTileSet.frameY = NULL;
					break;
				case 4:
					tmpTileSet.attribute = ENEMY1_2_3;
					tile->setIsAvailMove(false);
					break;
				case 8:
					tmpTileSet.attribute = OBJ_NONE;
					tile->setIsAvailMove(true);
					tmpTileSet.imgNum = IMG_NONE;
					tmpTileSet.img = nullptr;
					tmpTileSet.frameX = NULL;
					tmpTileSet.frameY = NULL;
					break;
				}
				break;
			}
		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);
		break;
	case IMG_ENEMY2:
		tile->setIdx(tmpIdx);

		if ((idX == 1 || idX == 4) && (idY == 0 || idY == 4))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			switch (idX)
			{
			case 1:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = ENEMY2_1_1;
					tile->setIsAvailMove(false);
					break;
				case 4:
					tmpTileSet.attribute = ENEMY2_2_1;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			case 4:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = ENEMY2_1_2;
					tile->setIsAvailMove(false);
					break;
				case 4:
					tmpTileSet.attribute = ENEMY2_2_2;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			}
		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);
		break;
	case IMG_ENEMY3:
		tile->setIdx(tmpIdx);

		if ((idX == 0 || idX == 1) && (idY == 0 || idY == 2))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			switch (idX)
			{
			case 0:
				if (idY == 0)
				{
					tmpTileSet.attribute = ENEMY3_1_1;
					tile->setIsAvailMove(false);	
				}
				if (idY == 2)
				{
					tmpTileSet.attribute = ENEMY3_2_1;
					tile->setIsAvailMove(false);
				}
				break;
			case 1:
				if (idY == 0)
				{
					tmpTileSet.attribute = ENEMY3_1_2;
					tile->setIsAvailMove(false);
				}
				if (idY == 2)
				{
					tmpTileSet.attribute = ENEMY3_2_2;
					tile->setIsAvailMove(false);
				}
				break;
			
			}
		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);
		break;
	case IMG_ENEMY4:
		tile->setIdx(tmpIdx);

		if ((idX == 0 || idX == 1 || idX == 2) && (idY == 0 || idY == 2))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			switch (idX)
			{
			case 0:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = ENEMY4_1_1;
					tile->setIsAvailMove(false);
					break;
				case 2:
					tmpTileSet.attribute = ENEMY4_2_1;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			case 1:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = ENEMY4_1_2;
					tile->setIsAvailMove(false);
					break;
				case 2:
					tmpTileSet.attribute = ENEMY4_2_2;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			case 2:
				switch (idY)
				{
				case 0:
					tmpTileSet.attribute = ENEMY4_1_3;
					tile->setIsAvailMove(false);
					break;
				case 2:
					tmpTileSet.attribute = ENEMY4_2_3;
					tile->setIsAvailMove(false);
					break;
				}
				break;
			}
		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);
		break;
	case IMG_ETC1:
		tile->setIdx(tmpIdx);

		if ((idX == 1 || idX == 4 || idX == 7) && (idY == 0 || idY == 4))
		{
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
			switch (idY)
			{
			case 0:
				if (idX == 1)
				{
					tmpTileSet.attribute = ETC_TORCH;
					tile->setIsAvailMove(false);
				}
				else if (idX == 4)
				{
					tmpTileSet.attribute = ETC_CHEST;
					tile->setIsAvailMove(false);
				}
				else if (idX == 7)
				{
					tmpTileSet.attribute = OBJ_NONE;
					tile->setIsAvailMove(true);
					tmpTileSet.imgNum = IMG_NONE;
					tmpTileSet.img = nullptr;
					tmpTileSet.frameX = NULL;
					tmpTileSet.frameY = NULL;
				}
				break;
			case 4:
				if (idX == 1)
				{
					tmpTileSet.attribute = ETC_SLOW;
					tile->setIsAvailMove(true);
				}
				else if (idX == 4)
				{
					tmpTileSet.attribute = ETC_FAST;
					tile->setIsAvailMove(true);
				}
				else if (idX == 7)
				{
					tmpTileSet.attribute = ETC_TRAPDOOR;
					tile->setIsAvailMove(true);
				}
				break;
			}
		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
			tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
			tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
			tmpTileSet.rc = { (float)tmpTileSet.pos.x,
							(float)tmpTileSet.pos.y,
							(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
							(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

			tile->setTileSet(tmpTileSet);
		break;
	case IMG_ETC2:
		tile->setIdx(tmpIdx);
		if ((idX == 0) && (idY == 0))
		{
			tmpTileSet.attribute = ETC_SHOPKEEPER;
			tile->setIsAvailMove(false);
			tmpTileSet.imgNum = _currentImgNum;
			tmpTileSet.img = _currentImg;
			tmpTileSet.frameX = idX;
			tmpTileSet.frameY = idY;
		}
		else
		{
			tmpTileSet.attribute = OBJ_NONE;
			tile->setIsAvailMove(true);
			tmpTileSet.imgNum = IMG_NONE;
			tmpTileSet.img = nullptr;
			tmpTileSet.frameX = NULL;
			tmpTileSet.frameY = NULL;
		}
		tmpTileSet.pos.x = _sample_Area_Rc.left + TILESIZE + _currentImg->GetFrameWidth() * idX;
		tmpTileSet.pos.y = _sample_Area_Rc.top + TILESIZE + _currentImg->GetFrameHeight() * idY;
		tmpTileSet.rc = { (float)tmpTileSet.pos.x,
						(float)tmpTileSet.pos.y,
						(float)tmpTileSet.pos.x + _currentImg->GetFrameWidth(),
						(float)tmpTileSet.pos.y + _currentImg->GetFrameHeight() };

		tile->setTileSet(tmpTileSet);
		break;
	}
}

	//맵

void mapEditor::mapDragDraw()
{
	if (PtInRect(&rectMake(_sample_Area_Rc), pointMake(_ptMouse)))
	{
		_isCollision_Sample_Area_Rc = true;
	}
	else
	{
		_isCollision_Sample_Area_Rc = false;
	}
	if (!_isCollision_Sample_Area_Rc || !_isShow_Sample)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && _draw_Area_Rc.left < _ptMouse.x && _ptMouse.x < _draw_Area_Rc.right &&
			_draw_Area_Rc.top < _ptMouse.y && _ptMouse.y < _draw_Area_Rc.bottom)
		{
			_corsor_Map_Draw[0] = { _vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().x ,
									_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().y };
			_corsor_Map_Draw[1] = { _vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().x ,
									_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().y };
			_isClick = true;
			_isShow_Drag_Area = true;
		}
		if (_isClick)
		{
			_corsor_Map_Draw[1] = { _vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().x ,
									_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().y };
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON) && _draw_Area_Rc.left < _ptMouse.x && _ptMouse.x < _draw_Area_Rc.right &&
			_draw_Area_Rc.top < _ptMouse.y && _ptMouse.y < _draw_Area_Rc.bottom)
		{
			_isClick = false;
			_isShow_Drag_Area = false;

			int yStart = _corsor_Map_Draw[0].y;
			int yEnd = _corsor_Map_Draw[1].y;
			int xStart = _corsor_Map_Draw[0].x;
			int xEnd = _corsor_Map_Draw[1].x;
			int tmpNum;
			if (yStart > yEnd)
			{
				tmpNum = yStart;
				yStart = yEnd;
				yEnd = tmpNum;
			}
			if (xStart > xEnd)
			{
				tmpNum = xStart;
				xStart = xEnd;
				xEnd = tmpNum;
			}
			for (int i = yStart; i <= yEnd; i++)
			{
				for (int j = xStart; j <= xEnd; j++)
				{
					//지우개 모드면
					if (_isErase)
					{
						erase(_vvMap[i][j], _vvObj[i][j]);
					}
					//오브젝트지우개 모드면
					else if (_isObjErase)
					{
						objErase(_vvObj[i][j]);
					}
					//이외
					else
					{
						switch (_currentPage)
						{
							case PAGE_TILE:
								setMapTile(_vvMap[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
							break;
							case PAGE_WALL:
								setMapObject(_vvObj[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
							break;
							case PAGE_ITEM:
								setMapObject(_vvObj[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
							break;
							case PAGE_ENEMY:
								setMapObject(_vvObj[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
							break;
							case PAGE_ETC:
								if (_sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getAttribute() == ETC_TORCH)
								{
									if (_vvObj[i][j]->getAttribute() >= OBJ_WALL1 && _vvObj[i][j]->getAttribute() <= OBJ_WALL_BOSS)
									{
										_vvObj[i][j]->setAttribute(_vvObj[i][j]->getAttribute() + 200);
									}
									else if (_vvObj[i][j]->getAttribute() >= ETC_TORCH_WALL1 && _vvObj[i][j]->getAttribute() <= ETC_TORCH_BOSS) {}
									else
									{
										setMapObject(_vvObj[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
									}
								}
								else
								{
									setMapObject(_vvObj[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
								}
							break;
						}
					}
				}
			}
		}
		else if (KEYMANAGER->isStayKeyDown(VK_RBUTTON) && _draw_Area_Rc.left < _ptMouse.x && _ptMouse.x < _draw_Area_Rc.right &&
			_draw_Area_Rc.top < _ptMouse.y && _ptMouse.y < _draw_Area_Rc.bottom)
		{
			if (_isErase)
			{
				erase(_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x], _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]);
			}
			//오브젝트지우개 모드면
			else if (_isObjErase)
			{
				objErase(_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]);
			}
			//이외
			else
			{
				switch (_currentPage)
				{
				case PAGE_TILE:
					setMapTile(_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
					break;
				case PAGE_WALL:
					setMapObject(_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
					break;
				case PAGE_ITEM:
					setMapObject(_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
					break;
				case PAGE_ENEMY:
					setMapObject(_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
					break;
				case PAGE_ETC:
					if (_sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getAttribute() == ETC_TORCH)
					{
						if (_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getAttribute() >= OBJ_WALL1 && _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getAttribute() <= OBJ_WALL_BOSS)
						{
							_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->setAttribute(_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getAttribute() + 200);
						}
						else if (_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getAttribute() >= ETC_TORCH_WALL1 && _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getAttribute() <= ETC_TORCH_BOSS) {}
						else
						{
							setMapObject(_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
						}
					}
					else
					{
						setMapObject(_vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
					}
					break;

				}
			}
		}
	}
}

	//사이즈

void mapEditor::mapSizeChange()
{
	if (PtInRect(&rectMake(_btn_SizeChange.rc), pointMake(_ptMouse)))
	{	
		_isCollision_Btn = true;
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			//사이즈 버튼 off이면 -> on으로
			if (_btn_SizeChange.frame.y == 0)
			{
				_btn_SizeChange.frame.y = 1;
			}
			//사이즈 버튼 on이면 -> off로
			else if (_btn_SizeChange.frame.y == 1)
			{
				_btn_SizeChange.frame.y = 0;
			}
		}
	}
	else
	{
		_isCollision_Btn = false;
	}
	if (_btn_SizeChange.frame.y == 1)
	{
		for (int j = 0; j < 4 ; j++)
		{
			if (PtInRect(&rectMake(_btn_SizeArrow[j].rc), pointMake(_ptMouse)))
			{
				_isCollision_Btn = true;
				_btn_SizeArrow[j].frame.y = 1;
				if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
				{
					_delayCount++;

					if (_delayCount >= 5)
					{
							switch (j)
						{
						case ARROW_X_DOWN:
							if (_tileSizeX > 1)
							{
								_tileSizeX--;
								eraseMapX();
							}
							break;
						case ARROW_X_UP:
							_tileSizeX++;
							addMapX();
							break;
						case ARROW_Y_DOWN:
							if (_tileSizeY > 1)
							{
								_tileSizeY--;
								eraseMapY();
							}
							break;
						case ARROW_Y_UP:
							_tileSizeY++;
							addMapY();
							break;
						}
						_delayCount = 0;
					}
				}
				break;
			}
			else
			{
				_btn_SizeArrow[j].frame.y = 0;
				_isCollision_Btn = false;
			}
		}
	}
}

void mapEditor::addMapX()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		tile* tmpTile = new tile;
		setNewMapTile(tmpTile, _tileSizeX - 1, i);
		_vvMap[i].push_back(tmpTile);
	}
	for (int i = 0; i < _tileSizeY; i++)
	{
		tile* tmpTile = new tile;
		setNewObjTile(tmpTile, _tileSizeX - 1, i);
		_vvObj[i].push_back(tmpTile);
	}
}

void mapEditor::addMapY()
{
	vLine tmpLine;
	tmpLine.clear();
	for (int i = 0; i < _tileSizeX; i++)
	{
		tile* tmpTile = new tile;
		setNewMapTile(tmpTile, i, _tileSizeY - 1);
		tmpLine.push_back(tmpTile);
	}
	_vvMap.push_back(tmpLine);
	tmpLine.clear();

	for (int i = 0; i < _tileSizeX; i++)
	{
		tile* tmpTile = new tile;
		setNewObjTile(tmpTile, i, _tileSizeY - 1);
		tmpLine.push_back(tmpTile);
	}	
		_vvObj.push_back(tmpLine);
}

void mapEditor::eraseMapX()
{
	//포인터자료형들 딜리트 해주는작업 해야함
	for (int i = 0; i < _tileSizeY; i++)
	{
		_vvMap[i].pop_back();
		_vvObj[i].pop_back();
	}
	_corsor_Map_Idx = { 0,0 };
}

void mapEditor::eraseMapY()
{
	//포인터자료형들 딜리트 해주는작업 해야함
	_vvMap.pop_back();
	_vvObj.pop_back();
	_corsor_Map_Idx = { 0,0 };
}

	//세이브

void mapEditor::mapSaveBtn()
{
	if (PtInRect(&rectMake(_btn_Save.rc), pointMake(_ptMouse)))
	{
		_isCollision_Btn = true;
		_btn_Save.frame.y = 1;
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			saveMap();
		}
	}
	else
	{
		_isCollision_Btn = false;
		_btn_Save.frame.y = 0;
	}
}

void mapEditor::mapLoadBtn()
{
	if (PtInRect(&rectMake(_btn_Load.rc), pointMake(_ptMouse)))
	{
		_isCollision_Btn = true;
		_btn_Load.frame.y = 1;
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			loadMap();
			_isChanged = true;
		}
	}
	else
	{
		_isCollision_Btn = false;
		_btn_Load.frame.y = 0;
	}
}

void mapEditor::saveMap()
{
	HANDLE file;
	DWORD save;

	char fileName[20] = "map";
	char idx[5] = {};
	itoa(_mapNum, idx, 10);
	strcat_s(fileName, sizeof(fileName), idx);
	strcat_s(fileName, sizeof(fileName), ".map");

	file = CreateFile(fileName, GENERIC_WRITE, NULL, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int sizeX;
	int sizeY;
	sizeX = _tileSizeX;
	sizeY = _tileSizeY;

	WriteFile(file, &sizeX, sizeof(int), &save, NULL);
	WriteFile(file, &sizeY, sizeof(int), &save, NULL);

	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			POINT idx[2];
			POINT pos[2];
			D2D_RECT_F rc[2];
			int frameX[2];
			int frameY[2];
			int attribute[2];
			bool isAvailMove[2];
			//string imgKey[2];
			int imgNum[2];

			idx[0] = _vvMap[i][j]->getIdx();
			pos[0] = _vvMap[i][j]->getPos();
			rc[0] = _vvMap[i][j]->getRc();
			frameX[0] = _vvMap[i][j]->getFrameX();
			frameY[0] = _vvMap[i][j]->getFrameY();
			attribute[0] = _vvMap[i][j]->getAttribute();
			isAvailMove[0] = _vvMap[i][j]->getIsAvailMove();
			//imgKey[0] = _vvMap[i][j]->getImgKey();
			imgNum[0] = _vvMap[i][j]->getImgNum();

			idx[1] = _vvObj[i][j]->getIdx();
			pos[1] = _vvObj[i][j]->getPos();
			rc[1] = _vvObj[i][j]->getRc();
			frameX[1] = _vvObj[i][j]->getFrameX();
			frameY[1] = _vvObj[i][j]->getFrameY();
			attribute[1] = _vvObj[i][j]->getAttribute();
			isAvailMove[1] = _vvObj[i][j]->getIsAvailMove();
			//imgKey[1] = _vvObj[i][j]->getImgKey();
			imgNum[1] = _vvObj[i][j]->getImgNum();

			WriteFile(file, &idx[0], sizeof(POINT), &save, NULL);
			WriteFile(file, &pos[0], sizeof(POINT), &save, NULL);
			WriteFile(file, &rc[0], sizeof(D2D_RECT_F), &save, NULL);
			WriteFile(file, &frameX[0], sizeof(int), &save, NULL);
			WriteFile(file, &frameY[0], sizeof(int), &save, NULL);
			WriteFile(file, &attribute[0], sizeof(int), &save, NULL);
			WriteFile(file, &isAvailMove[0], sizeof(bool), &save, NULL);
			//WriteFile(file, &imgKey[0], sizeof(string), &save, NULL);
			WriteFile(file, &imgNum[0], sizeof(int), &save, NULL);

			WriteFile(file, &idx[1], sizeof(POINT), &save, NULL);
			WriteFile(file, &pos[1], sizeof(POINT), &save, NULL);
			WriteFile(file, &rc[1], sizeof(D2D_RECT_F), &save, NULL);
			WriteFile(file, &frameX[1], sizeof(int), &save, NULL);
			WriteFile(file, &frameY[1], sizeof(int), &save, NULL);
			WriteFile(file, &attribute[1], sizeof(int), &save, NULL);
			WriteFile(file, &isAvailMove[1], sizeof(bool), &save, NULL);
			//WriteFile(file, &imgKey[1], sizeof(string), &save, NULL);
			WriteFile(file, &imgNum[1], sizeof(int), &save, NULL);
		}
	}

	CloseHandle(file);
}

void mapEditor::loadMap()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		_vvMap[i].clear();
		_vvObj[i].clear();
	}
	_vvMap.clear();
	_vvObj.clear();
	

	HANDLE file;
	DWORD load;

	char tmp[10] = {};
	char fileName[20] = "map";

	//로드할 파일이름
	itoa(_mapNum, tmp, 10);
	strcat_s(fileName, sizeof(fileName), tmp);
	strcat_s(fileName, sizeof(fileName), ".map");

	file = CreateFile(fileName, GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int sizeX;
	int sizeY;

	ReadFile(file, &sizeX, sizeof(load), &load, NULL);
	ReadFile(file, &sizeY, sizeof(load), &load, NULL);

	_tileSizeX = sizeX;
	_tileSizeY = sizeY;

	for (int i = 0; i < _tileSizeY; i++)
	{
		vLine mapLine;
		vLine objLine;
		mapLine.clear();
		objLine.clear();
		for (int j = 0; j < _tileSizeX; j++)
		{
			tile* mapTile = new tile;
			setNewMapTile(mapTile, j, i);
			mapLine.push_back(mapTile);

			tile* objTile = new tile;
			setNewObjTile(objTile, j, i);
			objLine.push_back(objTile);
		}
		_vvMap.push_back(mapLine);

		_vvObj.push_back(objLine);
	}

	for (int i = 0; i < _tileSizeY; ++i)
	{
		for (int j = 0; j < _tileSizeX; ++j)
		{
			POINT idx[2];
			POINT pos[2];
			D2D_RECT_F rc[2];
			int frameX[2];
			int frameY[2];
			int attribute[2];
			bool isAvailMove[2];
			//string imgKey[2];
			int imgNum[2];

			ReadFile(file, &idx[0], sizeof(POINT), &load, NULL);
			ReadFile(file, &pos[0], sizeof(POINT), &load, NULL);
			ReadFile(file, &rc[0], sizeof(D2D_RECT_F), &load, NULL);
			ReadFile(file, &frameX[0], sizeof(int), &load, NULL);
			ReadFile(file, &frameY[0], sizeof(int), &load, NULL);
			ReadFile(file, &attribute[0], sizeof(int), &load, NULL);
			ReadFile(file, &isAvailMove[0], sizeof(bool), &load, NULL);
			//ReadFile(file, &imgKey[0], sizeof(string), &load, NULL);
			ReadFile(file, &imgNum[0], sizeof(int), &load, NULL);

			_vvMap[i][j]->setIdx(idx[0]);
			_vvMap[i][j]->setPos(pos[0]);
			_vvMap[i][j]->setRc(rc[0]);
			_vvMap[i][j]->setFrameX(frameX[0]);
			_vvMap[i][j]->setFrameY(frameY[0]);
			_vvMap[i][j]->setAttribute(attribute[0]);
			_vvMap[i][j]->setIsAvailMove(isAvailMove[0]);
			//_vvMap[i][j]->setImgKey(imgKey[0]);
			//
			//if (_vvMap[i][j]->getImgKey() == "null")
			//{
			//	_vvMap[i][j]->setImg(nullptr);
			//}
			//else
			//{
			//	image* tmpImg = IMAGEMANAGER->findImage(imgKey[0]);
			//	_vvMap[i][j]->setImg(tmpImg);
			//	_vvMap[i][j]->getImg()->SetFrameX(frameX[0]);
			//	_vvMap[i][j]->getImg()->SetFrameY(frameY[0]);
			//}
			_vvMap[i][j]->setImgNum(imgNum[0]);

			switch (_vvMap[i][j]->getImgNum())
			{
				case IMG_NONE:
					_vvMap[i][j]->setImg(nullptr);
				break;
				case IMG_TILE:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("tile"));
				break;
				case IMG_WALL:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("wall"));
				break;
				case IMG_ITEM:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("item"));
				break;
				case IMG_ENEMY1:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
				break;
				case IMG_ENEMY2:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("enemy2"));
					break;
				case IMG_ENEMY3:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("enemy3"));
					break;
				case IMG_ENEMY4:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
					break;
				case IMG_ETC1:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("etc1"));
					break;
				case IMG_ETC2:
					_vvMap[i][j]->setImg(IMAGEMANAGER->findImage("etc2"));
				break;
			}

			ReadFile(file, &idx[1], sizeof(POINT), &load, NULL);
			ReadFile(file, &pos[1], sizeof(POINT), &load, NULL);
			ReadFile(file, &rc[1], sizeof(D2D_RECT_F), &load, NULL);
			ReadFile(file, &frameX[1], sizeof(int), &load, NULL);
			ReadFile(file, &frameY[1], sizeof(int), &load, NULL);
			ReadFile(file, &attribute[1], sizeof(int), &load, NULL);
			ReadFile(file, &isAvailMove[1], sizeof(bool), &load, NULL);
			//ReadFile(file, &imgKey[1], sizeof(string), &load, NULL);
			ReadFile(file, &imgNum[1], sizeof(int), &load, NULL);

			_vvObj[i][j]->setIdx(idx[1]);
			_vvObj[i][j]->setPos(pos[1]);
			_vvObj[i][j]->setRc(rc[1]);
			_vvObj[i][j]->setFrameX(frameX[1]);
			_vvObj[i][j]->setFrameY(frameY[1]);
			_vvObj[i][j]->setAttribute(attribute[1]);
			_vvObj[i][j]->setIsAvailMove(isAvailMove[1]);
			//_vvObj[i][j]->setImgKey(imgKey[1]);
			//
			//if (_vvObj[i][j]->getImgKey() == "null")
			//{
			//	_vvObj[i][j]->setImg(nullptr);
			//}
			//else
			//{
			//	image* tmpImg = IMAGEMANAGER->findImage(imgKey[1]);
			//	_vvObj[i][j]->setImg(tmpImg);
			//	_vvObj[i][j]->getImg()->SetFrameX(frameX[1]);
			//	_vvObj[i][j]->getImg()->SetFrameY(frameY[1]);
			//}
			_vvObj[i][j]->setImgNum(imgNum[1]);

			switch (_vvObj[i][j]->getImgNum())
			{
			case IMG_NONE:
				_vvObj[i][j]->setImg(nullptr);
				break;
			case IMG_TILE:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("tile"));
				break;
			case IMG_WALL:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("wall"));
				break;
			case IMG_ITEM:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("item"));
				break;
			case IMG_ENEMY1:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
				break;
			case IMG_ENEMY2:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("enemy2"));
				break;
			case IMG_ENEMY3:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("enemy3"));
				break;
			case IMG_ENEMY4:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
				break;
			case IMG_ETC1:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("etc1"));
				break;
			case IMG_ETC2:
				_vvObj[i][j]->setImg(IMAGEMANAGER->findImage("etc2"));
				break;
			}
		}
	}
	CloseHandle(file);

	_corsor_Map_Draw[0] = { 0,0 };
	_corsor_Map_Draw[1] = { 0,0 };
	_corsor_Map_Idx = { 0,0 };
}

	//맵인덱스

void mapEditor::mapIdxChange()
{
	if (PtInRect(&rectMake(_btn_MapIdx.rc), pointMake(_ptMouse)))
	{
		_isCollision_Btn = true;
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (_btn_MapIdx.frame.y == 0)
			{
				_btn_MapIdx.frame.y = 1;
			}
			else if (_btn_MapIdx.frame.y == 1)
			{
				_btn_MapIdx.frame.y = 0;
			}
		}
	}
	else
	{
		_isCollision_Btn = false;
	}

	if (_btn_MapIdx.frame.y == 1)
	{
		for (int j = 0; j < 2; j++)
		{
			if (PtInRect(&rectMake(_btn_MapIdxArrow[j].rc), pointMake(_ptMouse)))
			{
				_isCollision_Btn = true;
				_btn_MapIdxArrow[j].frame.y = 1;
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					switch (j)
					{
						case 0:
							if (_mapNum > 0)
							{
								_mapNum--;
							}
						break;
						case 1:
							if (_mapNum < 9)
							{
								_mapNum++;
							}
						break;
					}
				}
			}
			else
			{
				_isCollision_Btn = false;
				_btn_MapIdxArrow[j].frame.y = 0;
			}
		}
	}
}

	//지우개

void mapEditor::eraseMode()
{
	if (PtInRect(&rectMake(_btn_Erase.rc), pointMake(_ptMouse)))
	{
		_isCollision_Btn = true;

		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (!_isErase)
			{
				if (_isObjErase)
				{
					_btn_ObjErase.frame.y = 0;
					_isObjErase = false;
				}
				_isErase = true;	
				_btn_Erase.frame.y = 1;
			}
			else if (_isErase)
			{
				_isErase = false;
				_btn_Erase.frame.y = 0;
			}
		}
	}
	else
	{
		_isCollision_Btn = false;
	}
}

void mapEditor::objEraseMode()
{
	if (PtInRect(&rectMake(_btn_ObjErase.rc), pointMake(_ptMouse)))
	{
		_isCollision_Btn = true;

		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (!_isObjErase)
			{
				if (_isErase)
				{
					_btn_Erase.frame.y = 0;
					_isErase = false;
				}
				_isObjErase = true;
				_btn_ObjErase.frame.y = 1;
			}
			else if (_isObjErase)
			{
				_isObjErase = false;
				_btn_ObjErase.frame.y = 0;
			}
		}
	}
	else
	{
		_isCollision_Btn = false;
	}
}

void mapEditor::erase(tile* mapTile, tile* objTile)
{
	mapTile->setAttribute(TILE_NONE);
	mapTile->setFrameX(NULL);
	mapTile->setFrameY(NULL);
	mapTile->setImgNum(IMG_NONE);
	mapTile->setImg(nullptr);
	mapTile->setIsAvailMove(false);

	objTile->setAttribute(OBJ_NONE);
	objTile->setFrameX(NULL);
	objTile->setFrameY(NULL);
	objTile->setImgNum(IMG_NONE);
	objTile->setImg(nullptr);
	objTile->setIsAvailMove(true);
}

void mapEditor::objErase(tile* objTile)
{
	objTile->setAttribute(OBJ_NONE);
	objTile->setFrameX(NULL);
	objTile->setFrameY(NULL);
	objTile->setImgNum(IMG_NONE);
	objTile->setImg(nullptr);
	objTile->setIsAvailMove(true);
}

//render----------------------------------------------------------------------------------------------------------------------

void mapEditor::mapTileRender()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (_draw_Area_Rc.left		< _vvMap[i][j]->getRc().right	- CAMERA->getPosX() && 
				_draw_Area_Rc.right		> _vvMap[i][j]->getRc().right	- CAMERA->getPosX() &&
				_draw_Area_Rc.top		< _vvMap[i][j]->getRc().bottom	- CAMERA->getPosY() && 
				_draw_Area_Rc.bottom	> _vvMap[i][j]->getRc().bottom	- CAMERA->getPosY())
			{
				if (_vvMap[i][j]->getImg() == nullptr)
				{
					D2DMANAGER->drawRectangle(	_vvMap[i][j]->getRc().left 
						- CAMERA->getPosX(),
												_vvMap[i][j]->getRc().top 
						- CAMERA->getPosY(),
												_vvMap[i][j]->getRc().right 
						- CAMERA->getPosX(),
												_vvMap[i][j]->getRc().bottom 
						- CAMERA->getPosY());
				}
				else if (_vvMap[i][j]->getImg() != nullptr)
				{
					if (!_isTestGoldBonus)
					{
						_vvMap[i][j]->getImg()->frameRender(_vvMap[i][j]->getRc().left
							- CAMERA->getPosX(),
															_vvMap[i][j]->getRc().top
							- CAMERA->getPosY(),
															_vvMap[i][j]->getImg()->GetFrameWidth(),
															_vvMap[i][j]->getImg()->GetFrameHeight(),
															_vvMap[i][j]->getFrameX(), _vvMap[i][j]->getFrameY());
					}
					else if (_isTestGoldBonus)
					{
						if ((_vvMap[i][j]->getAttribute() == TILE_GROUND || _vvMap[i][j]->getAttribute() == TILE_BOSS_GROUND)
							&& _vvMap[i][j]->getFrameX() == 4)
						{
							_vvMap[i][j]->getImg()->frameRender(_vvMap[i][j]->getRc().left
								- CAMERA->getPosX(),
																_vvMap[i][j]->getRc().top
								- CAMERA->getPosY(),
																_vvMap[i][j]->getImg()->GetFrameWidth(),
																_vvMap[i][j]->getImg()->GetFrameHeight(),
																_vvMap[i][j]->getFrameX() + _testGoldBonus, _vvMap[i][j]->getFrameY());
						}
						else
						{
							_vvMap[i][j]->getImg()->frameRender(_vvMap[i][j]->getRc().left
								- CAMERA->getPosX(),
																_vvMap[i][j]->getRc().top
								- CAMERA->getPosY(),
																_vvMap[i][j]->getImg()->GetFrameWidth(),
																_vvMap[i][j]->getImg()->GetFrameHeight(),
																_vvMap[i][j]->getFrameX(), _vvMap[i][j]->getFrameY());
						}
					}
				}
			}
		}
	}

	
}

void mapEditor::objectTileRender()
{
	for (int i = 0; i < _tileSizeY; i++)
	{
		for (int j = 0; j < _tileSizeX; j++)
		{
			if (_draw_Area_Rc.left < _vvObj[i][j]->getRc().right - CAMERA->getPosX() && 
				_vvObj[i][j]->getRc().right - CAMERA->getPosX() < _draw_Area_Rc.right &&
				_draw_Area_Rc.top < _vvObj[i][j]->getRc().bottom - CAMERA->getPosY() && 
				_vvObj[i][j]->getRc().bottom - CAMERA->getPosY() < _draw_Area_Rc.bottom)
			{
				if (_vvObj[i][j]->getImg() != nullptr && _vvObj[i][j]->getImgNum() == IMG_ENEMY4)
				{
					_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
						- CAMERA->getPosX(),
						_vvObj[i][j]->getRc().top
						- CAMERA->getPosY() - TILESIZE,
						_vvObj[i][j]->getImg()->GetFrameWidth(),
						_vvObj[i][j]->getImg()->GetFrameHeight(),
						_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
				}
				else if (_vvObj[i][j]->getImg() != nullptr && _vvObj[i][j]->getImgNum() != IMG_WALL)
				{
					if (_vvObj[i][j]->getAttribute() == ETC_TORCH)
					{
						_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
							- CAMERA->getPosX(),
							_vvObj[i][j]->getRc().top
							- CAMERA->getPosY(),
							_vvObj[i][j]->getImg()->GetFrameWidth(),
							_vvObj[i][j]->getImg()->GetFrameHeight(),
							_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY() + _torchFrameY);
					}
					else
					{
						_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left 
							- CAMERA->getPosX(),
															_vvObj[i][j]->getRc().top 
							- CAMERA->getPosY(),
															_vvObj[i][j]->getImg()->GetFrameWidth(), 
															_vvObj[i][j]->getImg()->GetFrameHeight(),
															_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
					}
				}
				else if (_vvObj[i][j]->getImg() != nullptr && _vvObj[i][j]->getImgNum() == IMG_WALL)
				{
					_vvObj[i][j]->getImg()->frameRender(_vvObj[i][j]->getRc().left
						- CAMERA->getPosX(),
														_vvObj[i][j]->getRc().top - TILESIZE
						- CAMERA->getPosY(),
														_vvObj[i][j]->getImg()->GetFrameWidth(),
														_vvObj[i][j]->getImg()->GetFrameHeight(),
														_vvObj[i][j]->getFrameX(), _vvObj[i][j]->getFrameY());
					if (_vvObj[i][j]->getAttribute() >= ETC_TORCH_WALL1 && _vvObj[i][j]->getAttribute() <= ETC_TORCH_BOSS)
					{
						IMAGEMANAGER->frameRender("etc1", _vvObj[i][j]->getRc().left - CAMERA->getPosX(), _vvObj[i][j]->getRc().top - CAMERA->getPosY() - TILESIZE, 1, _torchFrameY);
					}
				}
			}
		}
	}
}

void mapEditor::sampleTileRender()
{
	D2DMANAGER->fillRectangle(RGB(85,85,85), _sample_Area_Rc, 0.7);
	for (int i = 0; i <= _currentImg->GetMaxFrameY(); i++)
	{
		for (int j = 0; j <= _currentImg->GetMaxFrameX(); j++)
		{
			//if (_sampleTile[i][j]->getImg() == nullptr)
			//{
			//	D2DMANAGER->drawRectangle(_sampleTile[i][j]->getRc());
			//}
			if (_sampleTile[i][j]->getImg() != nullptr)
			{
				_sampleTile[i][j]->getImg()->frameRender(_sampleTile[i][j]->getRc().left, _sampleTile[i][j]->getRc().top,
					_sampleTile[i][j]->getFrameX(), _sampleTile[i][j]->getFrameY(), 1);
			}
		}
	}
}

void mapEditor::buttonRender()
{
	//===========================================
	//					카테고리
	//===========================================
	for (int i = 0; i < 5; i++)
	{
		_btn_Category[i].img->frameRender(_btn_Category[i].rc.left, _btn_Category[i].rc.top, _btn_Category[i].frame.x, _btn_Category[i].frame.y);
		if (i < 2)
		{
			_btn_Page[i].img->frameRender(_btn_Page[i].rc.left, _btn_Page[i].rc.top, _btn_Page[i].frame.x, _btn_Page[i].frame.y);
		}
	}

	//===========================================
	//					맵사이즈버튼
	//===========================================
	_btn_SizeChange.img->frameRender(_btn_SizeChange.rc.left, _btn_SizeChange.rc.top, _btn_SizeChange.frame.x, _btn_SizeChange.frame.y);
	_btn_SizeArrow[0].img->frameRenderAngle(_btn_SizeArrow[0].rc.left, _btn_SizeArrow[0].rc.top, _btn_SizeArrow[0].frame.x, _btn_SizeArrow[0].frame.y,180);
	_btn_SizeArrow[1].img->frameRenderAngle(_btn_SizeArrow[1].rc.left, _btn_SizeArrow[1].rc.top, _btn_SizeArrow[1].frame.x, _btn_SizeArrow[1].frame.y, 0);
	_btn_SizeArrow[2].img->frameRenderAngle(_btn_SizeArrow[2].rc.left, _btn_SizeArrow[2].rc.top, _btn_SizeArrow[2].frame.x, _btn_SizeArrow[2].frame.y, 270);
	_btn_SizeArrow[3].img->frameRenderAngle(_btn_SizeArrow[3].rc.left, _btn_SizeArrow[3].rc.top, _btn_SizeArrow[3].frame.x, _btn_SizeArrow[3].frame.y, 90);
	//===========================================
	//				세이브 & 로드 버튼
	//===========================================
	_btn_Save.img->frameRender(_btn_Save.rc.left, _btn_Save.rc.top, _btn_Save.frame.x, _btn_Save.frame.y);
	_btn_Load.img->frameRender(_btn_Load.rc.left, _btn_Load.rc.top, _btn_Load.frame.x, _btn_Load.frame.y);
	//===========================================
	//				맵 인덱스 변경 버튼
	//===========================================
	_btn_MapIdx.img->frameRender(_btn_MapIdx.rc.left, _btn_MapIdx.rc.top, _btn_MapIdx.frame.x, _btn_MapIdx.frame.y);
	_btn_MapIdxArrow[0].img->frameRenderAngle(_btn_MapIdxArrow[0].rc.left, _btn_MapIdxArrow[0].rc.top, _btn_MapIdxArrow[0].frame.x, _btn_MapIdxArrow[0].frame.y,180);
	_btn_MapIdxArrow[1].img->frameRenderAngle(_btn_MapIdxArrow[1].rc.left, _btn_MapIdxArrow[1].rc.top, _btn_MapIdxArrow[1].frame.x, _btn_MapIdxArrow[1].frame.y,0);
	WCHAR str[128];
	swprintf_s(str, L"%d", _mapNum);
	D2DMANAGER->drawText(str, _btn_MapIdxArrow[0].pos.x + 80, _btn_MapIdxArrow[0].pos.y + 2, 40, 0xff00ff);
	D2DMANAGER->drawText(str, _btn_MapIdxArrow[0].pos.x + 78, _btn_MapIdxArrow[0].pos.y, 40, 0x00ff00);
	//===========================================
	//				지우개 버튼
	//===========================================
	_btn_Erase.img->frameRender(_btn_Erase.rc.left, _btn_Erase.rc.top, _btn_Erase.frame.x, _btn_Erase.frame.y);
	_btn_ObjErase.img->frameRender(_btn_ObjErase.rc.left, _btn_ObjErase.rc.top, _btn_ObjErase.frame.x, _btn_ObjErase.frame.y);


			
	swprintf_s(str, L"X : %d", _tileSizeX);
	D2DMANAGER->drawText(str, _btn_SizeArrow[0].pos.x - 10, _btn_SizeArrow[0].pos.y + 55, 25, 0xff00ff);
	swprintf_s(str, L"Y : %d", _tileSizeY);
	D2DMANAGER->drawText(str, _btn_SizeArrow[1].pos.x - 10, _btn_SizeArrow[1].pos.y + 55, 25, 0xff00ff);
	swprintf_s(str, L"X : %d", _tileSizeX);
	D2DMANAGER->drawText(str, _btn_SizeArrow[0].pos.x - 12, _btn_SizeArrow[0].pos.y + 54, 25, 0x00ff00);
	swprintf_s(str, L"Y : %d", _tileSizeY);
	D2DMANAGER->drawText(str, _btn_SizeArrow[1].pos.x - 12, _btn_SizeArrow[1].pos.y + 54, 25, 0x00ff00);
}

void mapEditor::linePreview()
{
	//===========================================
	//					맵타일
	//===========================================

	if (_isShow_Sample && !_isCollision_Sample_Area_Rc && !_isShow_Drag_Area)
	{
		D2DMANAGER->drawRectangle(0x00ff00, _vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().left
			- CAMERA->getPosX(),
			_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().top
			- CAMERA->getPosY(),
			_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().right
			- CAMERA->getPosX(),
			_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().bottom
			- CAMERA->getPosY());
	}
	else if (!_isShow_Sample && !_isShow_Drag_Area)
	{
		D2DMANAGER->drawRectangle(0x00ff00, _vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().left
			- CAMERA->getPosX(),
			_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().top
			- CAMERA->getPosY(),
			_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().right
			- CAMERA->getPosX(),
			_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getRc().bottom
			- CAMERA->getPosY());
	}

	if (_isShow_Drag_Area)
	{
		POINT dragPoint[2];

		dragPoint[0].x = _corsor_Map_Draw[0].x;
		dragPoint[0].y = _corsor_Map_Draw[0].y;
		dragPoint[1].x = _corsor_Map_Draw[1].x;
		dragPoint[1].y = _corsor_Map_Draw[1].y;
		int tmp;

		if (dragPoint[1].x - dragPoint[0].x < 0)
		{
			tmp = dragPoint[0].x;
			dragPoint[0].x = dragPoint[1].x;
			dragPoint[1].x = tmp;
		}
		if (dragPoint[1].y - dragPoint[0].y < 0)
		{
			tmp = dragPoint[0].y;
			dragPoint[0].y = dragPoint[1].y;
			dragPoint[1].y = tmp;
		}
		//D2DMANAGER->drawRectangle(0xff00ff, _vvMap[_corsor_Map_Draw[0].y][_corsor_Map_Draw[0].x]->getRc().left 
		//	- CAMERA->getPosX(),
		//									_vvMap[_corsor_Map_Draw[0].y][_corsor_Map_Draw[0].x]->getRc().top 
		//	- CAMERA->getPosY(),
		//									_vvMap[_corsor_Map_Draw[1].y][_corsor_Map_Draw[1].x]->getRc().right 
		//	- CAMERA->getPosX(),
		//									_vvMap[_corsor_Map_Draw[1].y][_corsor_Map_Draw[1].x]->getRc().bottom 
		//	- CAMERA->getPosY());
		D2DMANAGER->drawRectangle(0xff00ff, _vvMap[dragPoint[0].y][dragPoint[0].x]->getRc().left
			- CAMERA->getPosX(),
			_vvMap[dragPoint[0].y][dragPoint[0].x]->getRc().top
			- CAMERA->getPosY(),
			_vvMap[dragPoint[1].y][dragPoint[1].x]->getRc().right
			- CAMERA->getPosX(),
			_vvMap[dragPoint[1].y][dragPoint[1].x]->getRc().bottom
			- CAMERA->getPosY());
	}
	//===========================================
	//					샘플타일
	//===========================================
	if (_isShow_Sample)
	{
		D2DMANAGER->drawRectangle(0xff0000, _sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getRc());
	}

	if (_sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getImg() != nullptr)
	{
		_sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getImg()->frameRender(_ptMouse.x + TILESIZE, _ptMouse.y + TILESIZE, _sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getFrameX(), _sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getFrameY());
		D2DMANAGER->drawRectangle(0xffff00, _ptMouse.x + TILESIZE, _ptMouse.y + TILESIZE,
			_ptMouse.x + TILESIZE + _sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getImg()->GetFrameWidth(),
			_ptMouse.y + TILESIZE + _sampleTile[_corsor_Sample_Select.y][_corsor_Sample_Select.x]->getImg()->GetFrameHeight());
	}
}

void mapEditor::testText()
{
	WCHAR str[128];

	swprintf_s(str, L"isChanged : %d", _isChanged);
	D2DMANAGER->drawText(str, 0, 260, 20, 0x00ffff);

	swprintf_s(str, L"현재 이미지값 : %d", _currentImgNum);
	D2DMANAGER->drawText(str, 0, 280, 20, 0x00ffff);
	swprintf_s(str, L"선택한 샘플 IdX : % d  idy : % d", _corsor_Sample_Select.x, _corsor_Sample_Select.y);
	D2DMANAGER->drawText(str, 0, 300, 20, 0x00ffff);
	swprintf_s(str, L"현재 커서 IdX : % d  idy : % d", _corsor_Map_Idx.x, _corsor_Map_Idx.y);
	D2DMANAGER->drawText(str, 0, 320, 20, 0x00ffff);

	swprintf_s(str, L"Map IdX : %d , IdY : %d",_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().x, _vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().y);
	D2DMANAGER->drawText(str, 0, 340, 15, 0x00ffff);
	swprintf_s(str, L"Map ImgNum : %d",_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getImgNum());
	D2DMANAGER->drawText(str, 0, 355, 15, 0x00ffff);
	swprintf_s(str, L"Map FrameX : %d",_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getFrameX());
	D2DMANAGER->drawText(str, 0, 370, 15, 0x00ffff);
	swprintf_s(str, L"Map FrameY : %d",_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getFrameY());
	D2DMANAGER->drawText(str, 0, 385, 15, 0x00ffff);
	swprintf_s(str, L"Map Attribute : %d",_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getAttribute());
	D2DMANAGER->drawText(str, 0, 400, 15, 0x00ffff);
	swprintf_s(str, L"Map AvailMove : % d",_vvMap[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIsAvailMove());
	D2DMANAGER->drawText(str, 0, 415, 15, 0x00ffff);
					
	swprintf_s(str, L"obj IdX : %d , IdY : %d", _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().x, _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIdx().y);
	D2DMANAGER->drawText(str, 0, 440, 15, 0x00ffff);
	swprintf_s(str, L"obj ImgNum : %d", _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getImgNum());
	D2DMANAGER->drawText(str, 0, 455, 15, 0x00ffff);
	swprintf_s(str, L"obj FrameX : %d", _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getFrameX());
	D2DMANAGER->drawText(str, 0, 470, 15, 0x00ffff);
	swprintf_s(str, L"obj FrameY : %d", _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getFrameY());
	D2DMANAGER->drawText(str, 0, 485, 15, 0x00ffff);
	swprintf_s(str, L"obj Attribute : %d", _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getAttribute());
	D2DMANAGER->drawText(str, 0, 500, 15, 0x00ffff);
	swprintf_s(str, L"obj AvailMove : % d", _vvObj[_corsor_Map_Idx.y][_corsor_Map_Idx.x]->getIsAvailMove());
	D2DMANAGER->drawText(str, 0, 515, 15, 0x00ffff);


}

//=================================================
//						관
//=================================================
//토큰 세이브 & 로드

//void mapEditor::mapSave()
//{
//	HANDLE file;
//	DWORD write;
//
//	//itoa(대상, 어디에저장?, 몇진수?)
//	//strcat_s(대상, sizeof(대상), 붙일것.) 대상 뒤에 '\0'에 뒤에 붙이기
//
//	char fileName[20] = "map";
//	char idx[5] = {};
//	itoa(_mapNum, idx, 10);
//	strcat_s(fileName, sizeof(fileName), idx);
//	strcat_s(fileName, sizeof(fileName), ".map");
//
//	char save[5000000] = {};
//	char token[100000] = {};
//	char tmp[1000] = {};
//
//	itoa(_tileSizeX, tmp, 10);
//	strcat_s(save, sizeof(save), tmp);
//	strcat_s(save, sizeof(save), "]");
//
//	itoa(_tileSizeY, tmp, 10);
//	strcat_s(save, sizeof(save), tmp);
//	strcat_s(save, sizeof(save), "]");
//
//
//	for (int i = 0; i < _tileSizeY; i++)
//	{
//		for (int j = 0; j < _tileSizeX; j++)
//		{
//			//====================================
//			//			타일 정보들 저장
//			//====================================
//			{
//				//인덱스
//				itoa((_vvMap[i][j]->getIdx().x), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa((_vvMap[i][j]->getIdx().y), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//pos
//				itoa((_vvMap[i][j]->getPos().x), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa((_vvMap[i][j]->getPos().y), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//rect
//				itoa(_vvMap[i][j]->getRc().left, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvMap[i][j]->getRc().top, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvMap[i][j]->getRc().right, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvMap[i][j]->getRc().bottom, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//frame
//				itoa(_vvMap[i][j]->getFrameX(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvMap[i][j]->getFrameY(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//Attribute
//				itoa(_vvMap[i][j]->getAttribute(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//isAvailMove
//				itoa(_vvMap[i][j]->getIsAvailMove(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//imgKey
//			//	strcat_s(token, sizeof(token), _vvMap[i][j]->getImgKey().c_str());
//			//	strcat_s(token, sizeof(token), "/");
//
//				//imgNum
//			}
//
//			//====================================
//			//			오브젝트 정보들 저장
//			//====================================
//			{
//				//인덱스
//				itoa(_vvObj[i][j]->getIdx().x, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvObj[i][j]->getIdx().y, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//pos
//				itoa(_vvObj[i][j]->getPos().x, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvObj[i][j]->getPos().y, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//rect
//				itoa(_vvObj[i][j]->getRc().left, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvObj[i][j]->getRc().top, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvObj[i][j]->getRc().right, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvObj[i][j]->getRc().bottom, tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//frame
//				itoa(_vvObj[i][j]->getFrameX(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//				itoa(_vvObj[i][j]->getFrameY(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//Attribute
//				itoa(_vvObj[i][j]->getAttribute(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//isAvlilMove
//				itoa(_vvObj[i][j]->getIsAvailMove(), tmp, 10);
//				strcat_s(token, sizeof(token), tmp);
//				strcat_s(token, sizeof(token), "/");
//
//				//imgKey
//				//strcat_s(token, sizeof(token), _vvObj[i][j]->getImgKey().c_str());
//				//strcat_s(token, sizeof(token), "/");
//
//				//imgNum
//			}
//
//			//--------- 위에서 자른거 묶고 token에 null 넣어주기--------
//			strcat_s(save, sizeof(save), token);
//			for (int k = 0; k < 10000; k++)
//			{
//				token[k] = '\0';
//			}
//		}
//	}
//
//	file = CreateFile(fileName, GENERIC_WRITE, NULL, NULL,
//		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	WriteFile(file, save, sizeof(save), &write, NULL);
//	CloseHandle(file);
//}
//토큰 로드
//void mapEditor::mapLoad()
//{
//	//로드에서 안된다 이말이야
//	//세이브 순서보고 로드 순서 봐보자.
//	for (int i = 0; i < _tileSizeY; i++)
//	{
//		for (int j = 0; j < _tileSizeX; j++)
//		{
//			delete _vvMap[i][j];
//			delete _vvObj[i][j];
//			_vvMap[i][j] = nullptr;
//			_vvObj[i][j] = nullptr;
//		}
//		_vvMap[i].clear();
//		_vvObj[i].clear();
//	}
//	_vvMap.clear();
//	_vvObj.clear();
//
//	//잘린것 = strtok_s(무엇을/ seperator/ 남은거 저장)
//	//strtok_s 가 실행될대마다 원본의 seperator부분을 null로 만듬.
//	//연속해서 사용시 token = strtok_s(원본, seperator, & context)는 한번만사용,
//	//		이후엔 token = strtor_s(NULL, seperator, & context)로 계속 진행한다.
//	// char* token, context에 주의
//
//	HANDLE file;
//	DWORD read;
//
//	char load[5000000] = {};
//	char* token;	//1번 잘려진 문자열의 주소
//	char* context;	//2번 잘려진 문자열의 주소
//
//	char tmp[10] = {};
//	const char* seperator = "/";	// 구분자
//	char fileName[20] = "map";
//	int tmpInt;
//
//	//로드할 파일이름
//	itoa(_mapNum, tmp, 10);
//	strcat_s(fileName, sizeof(fileName), tmp);
//	strcat_s(fileName, sizeof(fileName), ".map");
//
//	file = CreateFile(fileName, GENERIC_READ, NULL, NULL,
//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	ReadFile(file, load, sizeof(load), &read, NULL);
//	CloseHandle(file);
//
//	// 여기까지 load에대한 데이터 입력받은 상태 
//
//	//맵 크기 가져오자
//	token = strtok_s(load, "]", &context);
//	tmpInt = atoi(token);
//	_tileSizeX = tmpInt;
//	token = strtok_s(NULL, "]", &context);
//	tmpInt = atoi(token);
//	_tileSizeY = tmpInt;
//
//	tile* tmpTileMap;
//	tile* tmpTileObj;
//	for (int i = 0; i < _tileSizeY; i++)
//	{
//		vLine vLineMap;
//		vLineMap.clear();
//		vLine vLineObj;
//		vLineObj.clear();
//		for (int j = 0; j < _tileSizeX; j++)
//		{
//
//
//			POINT tmpIdx;
//			POINT tmpPos;
//			D2D_RECT_F tmpRc;
//			//====================================
//			//		타일 정보들 불러오기
//			//====================================
//			{
//				tmpTileMap = new tile;
//
//				//idx
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpIdx.x = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpIdx.y = tmpInt;
//				tmpTileMap->setIdx(tmpIdx);
//
//				//pos
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpPos.x = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpPos.y = tmpInt;
//				tmpTileMap->setPos(tmpPos);
//
//				//rect
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.left = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.top = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.right = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.bottom = tmpInt;
//				tmpTileMap->setRc(tmpRc);
//
//				//frame
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileMap->setFrameX(tmpInt);
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileMap->setFrameY(tmpInt);
//
//				//attribute
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileMap->setAttribute(tmpInt);
//
//				//isAvailMove
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileMap->setIsAvailMove(tmpInt);
//
//				//imgKey
//				//token = strtok_s(NULL, "/", &context);
//				//tmpTileMap->setImgKey((string)token);			
//				//
//				//tmpTileMap->setImg(nullptr);
//				//
//				//	if (tmpTileMap->getImgKey() != "null")
//				//{
//				//	image* tempImg = IMAGEMANAGER->findImage(tmpTileMap->getImgKey());
//				//	tmpTileMap->setImg(tempImg);
//				//
//				//	tmpTileMap->getImg()->SetFrameX(tmpTileMap->getFrameX());
//				//	tmpTileMap->getImg()->SetFrameY(tmpTileMap->getFrameY());
//				//}
//
//				//imgNum
//			}
//			//====================================
//			//		오브젝트 정보들 불러오기
//			//====================================
//			{
//				tmpTileObj = new tile;
//				//idx
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpIdx.x = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpIdx.y = tmpInt;
//				tmpTileObj->setIdx(tmpIdx);
//
//				//pos
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpPos.x = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpPos.y = tmpInt;
//				tmpTileObj->setPos(tmpPos);
//
//				//rect
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.left = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.top = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.right = tmpInt;
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpRc.bottom = tmpInt;
//				tmpTileObj->setRc(tmpRc);
//
//				//frame
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileMap->setFrameX(tmpInt);
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileObj->setFrameY(tmpInt);
//
//				//attribute
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileObj->setAttribute(tmpInt);
//
//				//isAvailMove
//				token = strtok_s(NULL, "/", &context);
//				tmpInt = atoi(token);
//				tmpTileObj->setIsAvailMove(tmpInt);
//
//				//imgKey
//				//token = strtok_s(NULL, "/", &context);
//				//tmpTileObj->setImgKey((string)token);
//				//
//				//tmpTileObj->setImg(nullptr);
//				//
//				//if(tmpTileObj->getImgKey() != "null")
//				//{
//				//	image* tempImg = IMAGEMANAGER->findImage(tmpTileObj->getImgKey());
//				//	tmpTileObj->setImg(tempImg);
//				//	tmpTileObj->getImg()->SetFrameX(tmpTileMap->getFrameX());
//				//	tmpTileObj->getImg()->SetFrameY(tmpTileMap->getFrameY());
//				//}
//
//				//imgNum
//			}
//			vLineMap.push_back(tmpTileMap);
//			vLineObj.push_back(tmpTileObj);
//		}
//		_vvMap.push_back(vLineMap);
//		_vvObj.push_back(vLineObj);
//	}
//	_corsor_Map_Idx = _vvMap[0][0]->getIdx();
//	imageInit();
//}