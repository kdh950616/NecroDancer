#include "stdafx.h"
#include "mapLoader.h"


mapLoader::mapLoader()
{
}


mapLoader::~mapLoader()
{
}
HRESULT mapLoader::beatInit(string stageKeyName, const char * beatFileName, float volume)
{
	SOUNDMANAGER->loadBeat(beatFileName, stageKeyName);
	SOUNDMANAGER->playZone(stageKeyName, volume);

	return S_OK;
}
HRESULT mapLoader::beatInit(string stageKeyName, const char * beatFileName, float volume, vector<tagBeat>* vBeat)
{
	SOUNDMANAGER->loadBeat(beatFileName, stageKeyName);
	SOUNDMANAGER->playZone(stageKeyName, volume);
	*vBeat = SOUNDMANAGER->getVBeat();

	return S_OK;
}



//타일맵 주소, 오브젝트 주소, x사이즈 주소, y사이즈 주소, 불러올 맵번호 0 ~ 9
void mapLoader::mapLoad(vvMap * vvMapAdress, vvObj * vvObjAdress, int * tileSizeX, int * tileSizeY, bool* isTileChanged, int mapNum)
{
	vvMapAdress->clear();
	vvObjAdress->clear();

	HANDLE file;
	DWORD load;

	char tmp[10] = {};
	char fileName[20] = "map";

	//로드할 파일이름
	itoa(mapNum, tmp, 10);
	strcat_s(fileName, sizeof(fileName), tmp);
	strcat_s(fileName, sizeof(fileName), ".map");

	file = CreateFile(fileName, GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int sizeX;
	int sizeY;

	ReadFile(file, &sizeX, sizeof(load), &load, NULL);
	ReadFile(file, &sizeY, sizeof(load), &load, NULL);

	*tileSizeX = sizeX;
	*tileSizeY = sizeY;

	for (int i = 0; i < *tileSizeY; i++)
	{
		vLine mapLine;
		vLine objLine;
		mapLine.clear();
		objLine.clear();
		for (int j = 0; j < *tileSizeX; j++)
		{
			tile* mapTile = new tile;
			setNewMapTile(mapTile, j, i, *isTileChanged);
			mapLine.push_back(mapTile);

			tile* objTile = new tile;
			setNewObjTile(objTile, j, i);
			objLine.push_back(objTile);
		}
		(*vvMapAdress).push_back(mapLine);

		(*vvObjAdress).push_back(objLine);
	}

	for (int i = 0; i < *tileSizeY; ++i)
	{
		for (int j = 0; j < *tileSizeX; ++j)
		{
			POINT idx[2];
			POINT pos[2];
			D2D_RECT_F rc[2];
			int frameX[2];
			int frameY[2];
			int attribute[2];
			bool isAvailMove[2];
			int imgNum[2];

			ReadFile(file, &idx[0], sizeof(POINT), &load, NULL);
			ReadFile(file, &pos[0], sizeof(POINT), &load, NULL);
			ReadFile(file, &rc[0], sizeof(D2D_RECT_F), &load, NULL);
			ReadFile(file, &frameX[0], sizeof(int), &load, NULL);
			ReadFile(file, &frameY[0], sizeof(int), &load, NULL);
			ReadFile(file, &attribute[0], sizeof(int), &load, NULL);
			ReadFile(file, &isAvailMove[0], sizeof(bool), &load, NULL);
			ReadFile(file, &imgNum[0], sizeof(int), &load, NULL);

			(*vvMapAdress)[i][j]->setIdx(idx[0]);
			(*vvMapAdress)[i][j]->setPos(pos[0]);
			(*vvMapAdress)[i][j]->setRc(rc[0]);
			(*vvMapAdress)[i][j]->setFrameX(frameX[0]);
			(*vvMapAdress)[i][j]->setFrameY(frameY[0]);
			(*vvMapAdress)[i][j]->setAttribute(attribute[0]);
			(*vvMapAdress)[i][j]->setIsAvailMove(isAvailMove[0]);
			(*vvMapAdress)[i][j]->setImgNum(imgNum[0]);

			switch ((*vvMapAdress)[i][j]->getImgNum())
			{
				case IMG_NONE:
					(*vvMapAdress)[i][j]->setImg(nullptr);
				break;
				case IMG_TILE:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("tile"));
				break;
				case IMG_WALL:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("wall"));
				break;
				case IMG_ITEM:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("item"));
				break;
				case IMG_ENEMY1:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
				break;
				case IMG_ENEMY2:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy2"));
				break;
				case IMG_ENEMY3:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy3"));
				break;
				case IMG_ENEMY4:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
				break;
				case IMG_ETC1:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("etc1"));
				break;
				case IMG_ETC2:
					(*vvMapAdress)[i][j]->setImg(IMAGEMANAGER->findImage("etc2"));
				break;
			}

			ReadFile(file, &idx[1], sizeof(POINT), &load, NULL);
			ReadFile(file, &pos[1], sizeof(POINT), &load, NULL);
			ReadFile(file, &rc[1], sizeof(D2D_RECT_F), &load, NULL);
			ReadFile(file, &frameX[1], sizeof(int), &load, NULL);
			ReadFile(file, &frameY[1], sizeof(int), &load, NULL);
			ReadFile(file, &attribute[1], sizeof(int), &load, NULL);
			ReadFile(file, &isAvailMove[1], sizeof(bool), &load, NULL);
			ReadFile(file, &imgNum[1], sizeof(int), &load, NULL);

			(*vvObjAdress)[i][j]->setIdx(idx[1]);
			(*vvObjAdress)[i][j]->setPos(pos[1]);
			(*vvObjAdress)[i][j]->setRc(rc[1]);
			(*vvObjAdress)[i][j]->setFrameX(frameX[1]);
			(*vvObjAdress)[i][j]->setFrameY(frameY[1]);
			(*vvObjAdress)[i][j]->setAttribute(attribute[1]);
			(*vvObjAdress)[i][j]->setIsAvailMove(isAvailMove[1]);
			(*vvObjAdress)[i][j]->setImgNum(imgNum[1]);

			switch ((*vvObjAdress)[i][j]->getImgNum())
			{
				case IMG_NONE:
					(*vvObjAdress)[i][j]->setImg(nullptr);
				break;
				case IMG_TILE:
					(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("tile"));
				break;
				case IMG_WALL:
					(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("wall"));
				break;
				case IMG_ITEM:
					(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("item"));
				break;
				case IMG_ENEMY1:
					switch ((*vvObjAdress)[i][j]->getAttribute())
					{
						case ENEMY1_1_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
						break;
						case ENEMY1_1_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
						break;
						case ENEMY1_2_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
						break;
						case ENEMY1_2_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
						break;
						case ENEMY1_2_3:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
						break;
						case ENEMY1_3_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
						break;
						case ENEMY1_3_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy1"));
						break;
					}
				break;
				case IMG_ENEMY2:
					switch ((*vvObjAdress)[i][j]->getAttribute())
					{
						case ENEMY2_1_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy2"));
						break;
						case ENEMY2_1_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy2"));
						break;
						case ENEMY2_2_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy2"));
						break;
						case ENEMY2_2_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy2"));
						break;
					}
				break;
				case IMG_ENEMY3:	
					switch ((*vvObjAdress)[i][j]->getAttribute())
					{
						case ENEMY3_1_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy3"));
						break;
						case ENEMY3_1_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy3"));
						break;
						case ENEMY3_2_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy3"));
						break;
						case ENEMY3_2_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy3"));
						break;
					}
				break;
				case IMG_ENEMY4:
					switch ((*vvObjAdress)[i][j]->getAttribute())
					{
						case ENEMY4_1_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
						break;
						case ENEMY4_1_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
						break;
						case ENEMY4_1_3:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
						break;
						case ENEMY4_2_1:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
						break;
						case ENEMY4_2_2:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
						break;
						case ENEMY4_2_3:
							(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("enemy4"));
						break;
					}
				break;
				case IMG_ETC1:
					(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("etc1"));
				break;
				case IMG_ETC2:
					if ((*vvObjAdress)[i][j]->getAttribute() == ETC_SHOPKEEPER)
					{
						(*vvObjAdress)[i][j]->setImg(IMAGEMANAGER->findImage("shopkeeper"));
					}
					//else if ((*vvObjAdress)[i][j]->getAttribute() == ETC_PLAYER)
					//{
					//}
					break;
			}
		}
	}
	CloseHandle(file);
}

void mapLoader::makeEnemy(vvObj* vvObj, int * tileSizeX, int * tileSizeY)
{
	for (int i = 0; i < *tileSizeY; i++)
	{
		for (int j = 0; j < *tileSizeX; j++)
		{
			switch ((*vvObj)[i][j]->getAttribute())
			{
				case ENEMY1_1_1:				// 검은박쥐
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY1_1_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY1_1_2:				// 빨간박쥐
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY1_1_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY1_2_1:				// 하얀해골
					setNewObjTile((*vvObj)[i][j], j,i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY1_2_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY1_2_2:				// 노란해골
					setNewObjTile((*vvObj)[i][j], j,i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY1_2_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY1_2_3:				// 까만해골해골
					setNewObjTile((*vvObj)[i][j], j,i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY1_2_3, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY1_3_1:				// 초록슬라임
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY1_3_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY1_3_2:				// 파란슬라임
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY1_3_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY2_1_1:				// 클론
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY2_1_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY2_1_2:				// 레이스
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(true);
					_em->createEnemy(ENEMY2_1_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY2_2_1:				// 아르마딜로
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY2_2_1, (*vvObj)[i][j]->getIdx());
					break;
				break;
				case ENEMY2_2_2:				// 좀비
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY2_2_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY3_1_1:				// 미노타우로스
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(true);
					//_em->createEnemy(ENEMY3_1_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY3_1_2:				// 용
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY3_1_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY3_2_1:				// 황금박쥐
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY3_2_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY3_2_2:				// 밴쉬
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY3_2_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY4_1_1:				// 폰
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY4_1_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY4_1_2:				// 비숍
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY4_1_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY4_1_3:				// 룩
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY4_1_3, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY4_2_1:				// 나이트
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY4_2_1, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY4_2_2:				// 퀸
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY4_2_2, (*vvObj)[i][j]->getIdx());
				break;
				case ENEMY4_2_3:				// 킹
					setNewObjTile((*vvObj)[i][j], j, i);
					(*vvObj)[i][j]->setIsAvailMove(false);
					_em->createEnemy(ENEMY4_2_3, (*vvObj)[i][j]->getIdx());

				break;
			}
		}
	}
}

void mapLoader::makeBoss(vvObj * vvObj, int * tileSizeX, int * tileSizeY, int* pawnBeat)
{
	int pawnCount = 0;

	for (int i = 0; i < *tileSizeY; i++)
	{
		for (int j = 0; j < *tileSizeX; j++)
		{
			switch ((*vvObj)[i][j]->getAttribute())
			{
			case ENEMY4_1_1:				// 폰
				setNewObjTile((*vvObj)[i][j], j, i);
				(*vvObj)[i][j]->setIsAvailMove(false);
				_em->createPawn(ENEMY4_1_1, (*vvObj)[i][j]->getIdx(),pawnBeat[pawnCount]);
				pawnCount++;
				break;
			case ENEMY4_1_2:				// 비숍
				setNewObjTile((*vvObj)[i][j], j, i);
				(*vvObj)[i][j]->setIsAvailMove(false);
				_em->createEnemy(ENEMY4_1_2, (*vvObj)[i][j]->getIdx());
				break;
			case ENEMY4_1_3:				// 룩
				setNewObjTile((*vvObj)[i][j], j, i);
				(*vvObj)[i][j]->setIsAvailMove(false);
				_em->createEnemy(ENEMY4_1_3, (*vvObj)[i][j]->getIdx());
				break;
			case ENEMY4_2_1:				// 나이트
				setNewObjTile((*vvObj)[i][j], j, i);
				(*vvObj)[i][j]->setIsAvailMove(false);
				_em->createEnemy(ENEMY4_2_1, (*vvObj)[i][j]->getIdx());
				break;
			case ENEMY4_2_2:				// 퀸
				setNewObjTile((*vvObj)[i][j], j, i);
				(*vvObj)[i][j]->setIsAvailMove(false);
				_em->createEnemy(ENEMY4_2_2, (*vvObj)[i][j]->getIdx());
				break;
			case ENEMY4_2_3:				// 킹
				setNewObjTile((*vvObj)[i][j], j, i);
				(*vvObj)[i][j]->setIsAvailMove(false);
				_em->createEnemy(ENEMY4_2_3, (*vvObj)[i][j]->getIdx());

				break;
			}
		}
	}
}

void mapLoader::setNewMapTile(tile * tile, int idX, int idY, bool isTileChanged)
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
			if (isTileChanged)
			{
				tmpTileSet.frameX = 4;
			}
			else if (!isTileChanged)
			{
				tmpTileSet.frameX = 1;
			}
			tmpTileSet.frameY = RND->getFromIntTo(0, 2);
		}
		else if (idY % 2 == 1)
		{
			if (isTileChanged)
			{
				tmpTileSet.frameX = 1;
			}
			else if (!isTileChanged)
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
			if (isTileChanged)
			{
				tmpTileSet.frameX = 1;
			}
			else if (!isTileChanged)
			{
				tmpTileSet.frameX = 4;
			}
			tmpTileSet.frameY = RND->getFromIntTo(0, 2);
		}
		else if (idY % 2 == 1)
		{
			if (isTileChanged)
			{
				tmpTileSet.frameX = 4;
			}
			else if (!isTileChanged)
			{
				tmpTileSet.frameX = 1;
			}
			tmpTileSet.frameY = RND->getFromIntTo(0, 2);
		}
	}
	tmpTileSet.imgNum = IMG_TILE;
	tmpTileSet.img = IMAGEMANAGER->findImage("tile");
	tmpTileSet.pos.x = 0 + TILESIZE / 2 + TILESIZE * idX;
	tmpTileSet.pos.y = 0 + TILESIZE / 2 + TILESIZE * idY;
	tmpTileSet.rc = { (float)tmpTileSet.pos.x - TILESIZE / 2,
						(float)tmpTileSet.pos.y - TILESIZE / 2,
						(float)tmpTileSet.pos.x + TILESIZE / 2,
						(float)tmpTileSet.pos.y + TILESIZE / 2 };

	tile->setTileSet(tmpTileSet);
}

void mapLoader::setNewObjTile(tile * tile, int idX, int idY)
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
	tmpTileSet.pos.x = 0 + TILESIZE / 2 + TILESIZE * idX;
	tmpTileSet.pos.y = 0 + TILESIZE / 2 + TILESIZE * idY;
	tmpTileSet.rc = { (float)tmpTileSet.pos.x - TILESIZE / 2,
						(float)tmpTileSet.pos.y - TILESIZE / 2,
						(float)tmpTileSet.pos.x + TILESIZE / 2,
						(float)tmpTileSet.pos.y + TILESIZE / 2 };

	tile->setTileSet(tmpTileSet);
}
