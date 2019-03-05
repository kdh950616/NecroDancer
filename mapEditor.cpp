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
	_currentImg = IMAGEMANAGER->findImage("tile");
	_currentPage = PAGE_TILE;

	//기본 사이즈
	_tileNumX = DEFAULT_MAP_SIZE_X;
	_tileNumY = DEFAULT_MAP_SIZE_Y;
	
	//그릴영역 기초설정
	mapInit();
	//옵젝영역 기초설정
	objInit();
	//샘플영역 기초설정
	sampleInit();
	//기능버튼 기초설정
	buttonInit();
	


	return S_OK;
}

void mapEditor::release()
{
}

void mapEditor::update()
{
	//현재 맵의 무슨타일에 있는지 인덱스 저장용
	
	_corsor_Map_Idx = { (int)(_ptMouse.x + CAMERA->getPosX()) / TILESIZE  , (int)(_ptMouse.y + CAMERA->getPosY()) / TILESIZE };
	
	if (_corsor_Map_Idx.x >= _vvMap[0].size() - 1)
	{
		_corsor_Map_Idx.x = _vvMap[0].size() - 1;
	}
	if (_corsor_Map_Idx.y >= _vvMap.size() - 1)
	{
		_corsor_Map_Idx.y = _vvMap.size() - 1;
	}

	sampleFunc();
	cameraFunc();
	mapDragDraw();

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
	//타일들 이미지
	IMAGEMANAGER->addFrameImage("tile", L"images/mapTool/tile.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("wall", L"images/mapTool/wall.png", 432, 576, 9, 6);

	//버튼 이미지

	//==========================
	//			카테고리
	//==========================
	IMAGEMANAGER->addFrameImage("btn_Tile", L"images/mapTool/button/btn_Tile.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Object", L"images/mapTool/button/btn_Object.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Item", L"images/mapTool/button/btn_Item.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Enemy", L"images/mapTool/button/btn_Enemy.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Etc", L"images/mapTool/button/btn_Etc.png", 80, 60, 1, 2);
	//==========================
	//			기능
	//==========================
	IMAGEMANAGER->addFrameImage("btn_Erase", L"images/mapTool/button/btn_Erase.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_EraseObj", L"images/mapTool/button/btn_EraseObj.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Save", L"images/mapTool/button/btn_Save.png", 80, 60, 1, 2);
	IMAGEMANAGER->addFrameImage("btn_Load", L"images/mapTool/button/btn_Load.png", 80, 60, 1, 2);
}

void mapEditor::mapInit()
{
	//그릴영역 기초설정
	_draw_Area_Rc = { 0,0, WINSIZEX - TILESIZE, WINSIZEY - TILESIZE };
	_vvMap.clear();
	for (int i = 0; i < _tileNumY; i++)
	{
		vLine tmpLine;
		tmpLine.clear();
		for (int j = 0; j < _tileNumX; j++)
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
			tmpTileSet.frameX = 1;
			tmpTileSet.frameY = 0;
		}
		if (idY % 2 == 1)
		{
			tmpTileSet.frameX = 4;
			tmpTileSet.frameY = 0;
		}
	}
	if (idX % 2 == 1)
	{
		if (idY % 2 == 0)
		{
			tmpTileSet.frameX = 4;
			tmpTileSet.frameY = 0;
		}
		if (idY % 2 == 1)
		{
			tmpTileSet.frameX = 1;
			tmpTileSet.frameY = 0;
		}
	}
	tmpTileSet.img = _currentImg;
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
	for (int i = 0; i < _tileNumY; i++)
	{
		vLine tmpLine;
		tmpLine.clear();
		for (int j = 0; j < _tileNumX; j++)
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
				tmpTileSet.attribute = TILE_GROUND;
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
				tmpTileSet.attribute = TILE_GROUND;
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
				tmpTileSet.attribute = TILE_GROUND;
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
		_category[i].pos = { _sample_Area_Pos.x - TILESIZE + i * 110, _sample_Area_Pos.y - TILESIZE };

		_category[i].rc = { (float)_category[i].pos.x,
							(float)_category[i].pos.y,
							(float)_category[i].pos.x + 80,
							(float)_category[i].pos.y + 30 };
		switch (i)
		{
		case 0:
			_category[i].img = IMAGEMANAGER->findImage("btn_Tile");
			break;
		case 1:
			_category[i].img = IMAGEMANAGER->findImage("btn_Object");
			break;
		case 2:
			_category[i].img = IMAGEMANAGER->findImage("btn_Item");
			break;
		case 3:
			_category[i].img = IMAGEMANAGER->findImage("btn_Enemy");
			break;
		case 4:
			_category[i].img = IMAGEMANAGER->findImage("btn_Etc");
			break;
		}
		_category[i].img->SetFrameX(0);
		_category[i].img->SetFrameY(0);
		if (i == 0)
		{
			_category[i].img->SetFrameY(1);
		}
	}
	//===========================================
	//			사이즈변경 버튼 초기화
	//===========================================
	_sizeArrow[0].pos.x = _sample_Area_Rc.left;
	_sizeArrow[0].pos.y = _sample_Area_Rc.bottom - 200;
	_sizeArrow[0].rc = {	(float)_sizeArrow[0].pos.x,
							(float)_sizeArrow[0].pos.y,
							(float)_sizeArrow[0].pos.x + 80,
							(float)_sizeArrow[0].pos.y + 30 };
}

//update----------------------------------------------------------------------------------------------------------------------

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
		//카테고리 기능
		//카테고리 갯수만큼 포문
		for (int i = PAGE_TILE; i < PAGE_END; i++)
		{
			//카테고리 렉트와 충돌중이고
			if (PtInRect(&rectMake(_category[i].rc), pointMake(_ptMouse)))
			{
				//클릭을 했으면
				if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				{
					_category[_currentPage].img->SetFrameY(0);

					//해당거에 맞는 값으로 현재 페이지 변경
					_currentPage = (tagPage)i;

					_category[_currentPage].img->SetFrameY(1);
					//누른 카테고리에 맞게 이미지도 변경해줌.
					switch (_currentPage)
					{
					case 0:
						_currentImg = IMAGEMANAGER->findImage("tile");
						_corsor_Sample_Select.y *= 2;
						break;
					case 1:
						_currentImg = IMAGEMANAGER->findImage("wall");
						_corsor_Sample_Select.y /= 2;
						break;
					case 2:
						_currentImg = IMAGEMANAGER->findImage("item");
						break;
					case 3:
						_currentImg = IMAGEMANAGER->findImage("enemy");
						break;
					case 4:
						_currentImg = IMAGEMANAGER->findImage("etc");
						break;
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
		//---------------------------------------------------------------------------------------------------------
		//샘플들과 충돌중이 아니면-> 창이동할수있음
		if (!_isCollision_SampleTile && _isCollision_Sample_Area_Rc)
		{
			//샘플영역렉트와 충돌중인지 체크
			//if (PtInRect(&rectMake(_sample_Area_Rc), pointMake(_ptMouse)))
			//{
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

				if (_isClick)
				{

					_sample_Area_Pos = { (int)_ptMouse.x - _sample_Move_Cal_Pt.x, (int)_ptMouse.y - _sample_Move_Cal_Pt.y };
					_sample_Area_Rc = { (float)_sample_Area_Pos.x - TILESIZE,
								  (float)_sample_Area_Pos.y - TILESIZE,
								  (float)_sample_Area_Pos.x + TILESIZE + _currentImg->GetWidth(),
								  (float)_sample_Area_Pos.y + TILESIZE + _currentImg->GetHeight() + 100 };

					for (int i = 0; i < 5; i++)
					{
						_category[i].pos = { _sample_Area_Pos.x - TILESIZE + i * 110, _sample_Area_Pos.y - TILESIZE };

						_category[i].rc = { (float)_category[i].pos.x,
											(float)_category[i].pos.y,
											(float)_category[i].pos.x + 80,
											(float)_category[i].pos.y + 30 };
					}

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
				}
			//}
		}
		//else if (_isCollision_SampleTile)
		//{
		//	selectSampleTile();
		//}
	}
	//안보이면
	else if (!_isShow_Sample)
	{
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

void mapEditor::changeCategory(tile* tile, int idX, int idY)
{
	POINT tmpIdx = { idX, idY };
	tagTileSet tmpTileSet;

	switch (_currentPage)
	{
	case PAGE_TILE:
		tile->setIdx(tmpIdx);

		if ((idX == 1 || idX == 4 || idX == 7) && (idY == 0 || idY == 4 || idY == 8))
		{
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
					tmpTileSet.attribute = TILE_GROUND;
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
					tmpTileSet.attribute = TILE_GROUND;
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
					tmpTileSet.attribute = TILE_GROUND;
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
	case PAGE_WALL:
		tile->setIdx(tmpIdx);


		if ((idX == 1 || idX == 4 || idX == 7) && (idY == 0 || idY == 2 || idY == 4))
		{
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
	case PAGE_ITEM:
		break;
	case PAGE_ENEMY:
		break;
	case PAGE_ETC:
		break;
	case PAGE_END:
		break;
	}
}

void mapEditor::cameraFunc()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		CAMERA->setPosX(CAMERA->getPosX() - 2);
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		CAMERA->setPosY(CAMERA->getPosY() - 2);
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		CAMERA->setPosX(CAMERA->getPosX() + 2);
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		CAMERA->setPosY(CAMERA->getPosY() + 2);
	}
}

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

					switch (_currentPage)
					{
					case PAGE_TILE:
						setMapTile(_vvMap[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
						break;
					case PAGE_WALL:
						setMapObject(_vvObj[i][j], _corsor_Sample_Select.x, _corsor_Sample_Select.y);
						break;
					case PAGE_ITEM:

						break;
					case PAGE_ENEMY:

						break;
					case PAGE_ETC:

						break;

					}
				}
			}
		}
	}
	//if (!_isCollision_Sample_Area_Rc || !_isShow_Sample)
	//{
	//	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON) && _draw_Area_Rc.left < _ptMouse.x && _ptMouse.x < _draw_Area_Rc.right &&
	//		_draw_Area_Rc.top < _ptMouse.y && _ptMouse.y < _draw_Area_Rc.bottom)
	//	{
	//		//int yStart = _corsor_Sample_Select[0].y;
	//		//int yEnd = _corsor_Sample_Select[1].y;
	//		//int xStart = _corsor_Sample_Select[0].x;
	//		//int xEnd = _corsor_Sample_Select[1].x;
	//		int yStart = _corsor_Map_Draw[0].y;
	//		int yEnd = _corsor_Map_Draw[1].y;
	//		int xStart = _corsor_Map_Draw[0].x;
	//		int xEnd = _corsor_Map_Draw[1].x;
	//		int tmpNum;
	//		if (yStart > yEnd)
	//		{
	//			tmpNum = yStart;
	//			yStart = yEnd;
	//			yEnd = tmpNum;
	//		}
	//		if (xStart > xEnd)
	//		{
	//			tmpNum = xStart;
	//			xStart = xEnd;
	//			xEnd = tmpNum;
	//		}
	//
	//		int mapIdY = _corsor_Map_Idx.y;
	//		int mapIdX = _corsor_Map_Idx.x;
	//
	//		for (int i = yStart; i <= yEnd; i++, mapIdY++)
	//		{
	//			for (int j = xStart; j <= xEnd; j++, mapIdX++)
	//			{
	//				//맵밖에나가면 터지는거방지
	//				if (mapIdY + (i - yStart) > _tileNumY - 1 || mapIdX + (j - xStart) > _tileNumX - 1)
	//				{
	//					continue;
	//				}
	//				switch (_currentPage)
	//				{
	//				case PAGE_TILE:
	//					setMapTile(_vvMap[mapIdY][mapIdX], j, i);
	//					break;
	//				case PAGE_WALL:
	//					setMapObject(_vvObj[mapIdY][mapIdX], j, i);
	//					break;
	//				case PAGE_ITEM:
	//
	//					break;
	//				case PAGE_ENEMY:
	//
	//					break;
	//				case PAGE_ETC:
	//
	//					break;
	//
	//				}
	//			}
	//			mapIdX = _corsor_Map_Idx.x;
	//		}
	//	}
	//}
	
}

void mapEditor::mapSizeChange()
{
	if (_isShow_Sample)
	{

	}
}

void mapEditor::save()
{
	if (_isShow_Sample)
	{

	}
}

void mapEditor::load()
{
	if (_isShow_Sample)
	{

	}
}

void mapEditor::setMapTile(tile* tile, int idX, int idY)
{
	if (_sampleTile[idY][idX]->getImg() == nullptr) return;

	tile->setAttribute(_sampleTile[idY][idX]->getAttribute());
	tile->setFrameX(_sampleTile[idY][idX]->getFrameX());
	tile->setFrameY(_sampleTile[idY][idX]->getFrameY());
	tile->setImg(_sampleTile[idY][idX]->getImg());
}

void mapEditor::setMapObject(tile * tile, int idX, int idY)
{
	if (_sampleTile[idY][idX]->getImg() == nullptr) return;

	tile->setAttribute(_sampleTile[idY][idX]->getAttribute());
	tile->setFrameX(_sampleTile[idY][idX]->getFrameX());
	tile->setFrameY(_sampleTile[idY][idX]->getFrameY());
	tile->setImg(_sampleTile[idY][idX]->getImg());
}

//render----------------------------------------------------------------------------------------------------------------------

void mapEditor::mapTileRender()
{
	for (int i = 0; i < _tileNumY; i++)
	{
		for (int j = 0; j < _tileNumX; j++)
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

void mapEditor::objectTileRender()
{
	for (int i = 0; i < _tileNumY; i++)
	{
		for (int j = 0; j < _tileNumX; j++)
		{
			if (_draw_Area_Rc.left < _vvObj[i][j]->getRc().right - CAMERA->getPosX() && 
				_vvObj[i][j]->getRc().right - CAMERA->getPosX() < _draw_Area_Rc.right &&
				_draw_Area_Rc.top < _vvObj[i][j]->getRc().bottom - CAMERA->getPosY() && 
				_vvObj[i][j]->getRc().bottom - CAMERA->getPosY() < _draw_Area_Rc.bottom)
			{
				if (_vvObj[i][j]->getImg() != nullptr)
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
			if (_sampleTile[i][j]->getImg() == nullptr)
			{
				D2DMANAGER->drawRectangle(_sampleTile[i][j]->getRc());
			}
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
		_category[i].img->frameRender(_category[i].rc.left, _category[i].rc.top, _category[i].img->GetFrameX(), _category[i].img->GetFrameY());
	}

	for (int i = 0; i < 4; i++)
	{
		//사이즈 버튼 넣음
	}
	D2DMANAGER->drawRectangle(_sizeArrow[0].rc);
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
}

void mapEditor::testText()
{
	WCHAR str[128];
	swprintf_s(str, L"[1] choice IdX : % d  idy : % d", _corsor_Sample_Select.x, _corsor_Sample_Select.y);
	D2DMANAGER->drawText(str, 0, 340, 20, 0x00ffff);
	swprintf_s(str, L"corsor IdX : % d  idy : % d", _corsor_Map_Idx.x, _corsor_Map_Idx.y);
	D2DMANAGER->drawText(str, 0, 380, 20, 0x00ffff);
}
