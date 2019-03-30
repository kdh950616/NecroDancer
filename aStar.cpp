#include "stdafx.h"
#include "aStar.h"


aStar::aStar()
{
}


aStar::~aStar()
{
}

HRESULT aStar::init()
{
	return S_OK;
}

void aStar::release()
{
}

void aStar::update()
{
}

void aStar::render()
{
}

void aStar::pathFinder(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay)
{
	//현재위치가 도착점이라면
	if (currentPos.x == endPos.x && currentPos.y == endPos.y)
	{
 		POINT road = { (*_vTotalList)[currentPos.y][currentPos.x]->getIdx().x,(*_vTotalList)[currentPos.y][currentPos.x]->getIdx().y };

		lMyWay.clear();

		while (1)
		{
			if ((*_vTotalList)[road.y][road.x]->getParent() == nullptr)
			{
				break;
			}
			lMyWay.push_front(road);

			road = { (*_vTotalList)[road.y][road.x]->getParent()->getIdx().x, (*_vTotalList)[road.y][road.x]->getParent()->getIdx().y };
		}

		//재귀 탈출전 기초정보 리셋
		for (int i = 0; i < _vCloseList.size(); i++)
		{
			_vCloseList[i]->setParent(nullptr);
			_vCloseList[i]->setFScore(0);
			_vCloseList[i]->setGScore(0);
			_vCloseList[i]->setHScore(0);
		}

		for (int i = 0; i < _vOpenList.size(); i++)
		{
			_vOpenList[i]->setParent(nullptr);
			_vOpenList[i]->setFScore(0);
			_vOpenList[i]->setGScore(0);
			_vOpenList[i]->setHScore(0);
		}

		//오픈리스트와 클로즈리스트로 리셋
		_vOpenList.clear();
		_vCloseList.clear();

		//재귀 탈출
		return;
	}


	//오픈리스트들의 중심점을 만들좌
	int curIdxX = currentPos.x;
	int curIdxY = currentPos.y;

	//중심은 바로 클로즈리스트에 넣좌
	_vCloseList.push_back((*_vTotalList)[curIdxY][curIdxX]);

	//오픈목록에 내가 있는지 찾는다. 있으면 지워버림 -> 스타트 빼고 이걸 다함
	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (_vOpenList[i]->getIdx().x == curIdxX && _vOpenList[i]->getIdx().y == curIdxY)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			break;
		}
	}

	//이제 나의 주변타일을 검사하면서 오픈리스트에 넣자
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (abs(i) == abs(j)) continue;

			//벡터 크기 넘어가는거 예외처리
			if (curIdxX + i < 0 || curIdxX + i > (_vTotalList[0]).size() - 1
				|| curIdxY + j < 0 || curIdxY + j >(*_vTotalList).size() - 1)	continue;
			
			//일단 타일 만들어놓고
			tile* openTile = (*_vTotalList)[curIdxY + i][curIdxX + j];

			// 갈수 없는곳이면 건너뜀 -> 오브젝트랑 연동해서 해야할듯
			if (!openTile->getIsAvailMove()) continue;

			//클로즈 리스트에 있는것도 건너뜀
			bool isClosed = false;

			for (int k = 0; k < _vCloseList.size(); k++)
			{
				if (openTile == _vCloseList[k])
				{
					isClosed = true;
					break;
				}
			}

			if (isClosed)continue;

			//예외처리에 걸리지 않으면 -> 벽이아니고, 닫힌목록에 있던얘도 아니라면

			//열린목록에 이미 있는놈인지 검사 (최적의 루트를 찾아줘야함)
			bool isOpened = false;

			for (int k = 0; k < _vOpenList.size(); k++)
			{
			//	if (openTile->getTopIdx().x == _vOpenList[k]->getTopIdx().x
			//		&& openTile->getTopIdx().y == _vOpenList[k]->getTopIdx().y)
				if(openTile == _vOpenList[k])
				{
					isOpened = true;
					break;
				}
			}

			// G , H 미리 구해놓자

			//G 연산 -> 현재 내타일과 부모타일의 거리를 비교해서 대각선인지, 수평or수직에 있는건지 판별
			int gScore = getDistance((*_vTotalList)[curIdxY][curIdxX]->getPos().x, (*_vTotalList)[curIdxY][curIdxX]->getPos().y,
				openTile->getPos().x, openTile->getPos().y) == TILESIZE ? 10 : 14;

			//H 연산 -> 현재 내 타일과 도착타일의 인덱스를 비교
			int hScore = (abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().x - openTile->getIdx().x)
				+ abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().y - openTile->getIdx().y)) * 10;


			//열린목록에 지금 새로 등록되는거면 그냥 바로 추가
			if (!isOpened)
			{
				openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

			//	openTile->setGScore(openTile->getParent()->getGScore() + gScore);

				openTile->setGScore(gScore);

				openTile->setHScore(hScore);

				openTile->setFScore(openTile->getGScore() + openTile->getHScore());

				_vOpenList.push_back(openTile);
			}
			//열린목록에 이미 있는놈이면 과거부모와 현재부모의 G값을 비교함 - > 현재부모의 G값이 더저렴하다면 재설정해줌
			if (isOpened)
			{
				if (openTile->getParent()->getGScore()  > (*_vTotalList)[curIdxY][curIdxX]->getGScore())
				{
					//더나은경우가 나왔으니 일단 오픈타일에서 제거하자 (더나은값을넣고 다시 오픈타일에 넣을거임)
					for (int k = 0; k < _vOpenList.size(); k++)
					{
						if (_vOpenList[k] == openTile)
						{
							_vOpenList[k]->setParent(nullptr);
							_vOpenList[k]->setFScore(0);
							_vOpenList[k]->setGScore(0);
							_vOpenList[k]->setHScore(0);
							_vOpenList.erase(_vOpenList.begin() + k);
							break;
						}
					}

					// 부모를 바꿔주고
					openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

					//G 연산
					openTile->setGScore(openTile->getParent()->getGScore() + gScore);

					//H 연산
					openTile->setHScore(hScore);

					//F 연산
					openTile->setFScore(openTile->getGScore() + openTile->getHScore());

					_vOpenList.push_back(openTile);
				}
			}
		}
	}

	//길을 못찾았다면
	if ((_vOpenList.size() == 0 && _vCloseList.size() > 0) || _vCloseList.size() > 100)
	{
		//시작위치뱉고
		lMyWay.clear();
		lMyWay.push_front(PointMake(startPos.x, startPos.y));

		//재귀 탈출전 기초정보 리셋
		for (int i = 0; i < _vCloseList.size(); i++)
		{
			_vCloseList[i]->setParent(nullptr);
			_vCloseList[i]->setFScore(0);
			_vCloseList[i]->setGScore(0);
			_vCloseList[i]->setHScore(0);
		}

		for (int i = 0; i < _vOpenList.size(); i++)
		{
			_vOpenList[i]->setParent(nullptr);
			_vOpenList[i]->setFScore(0);
			_vOpenList[i]->setGScore(0);
			_vOpenList[i]->setHScore(0);
		}

		//오픈리스트와 클로즈리스트로 리셋
		_vOpenList.clear();
		_vCloseList.clear();

		//재귀 탈출
		return;
	}

	// 제일 저렴한 녀석을 찾아서 현재타일로 다시 박아줌
	int tempCost = 1000000000;
	tile* tempTile = NULL;

	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (tempCost > _vOpenList[i]->getFScore())
		{
			tempCost = _vOpenList[i]->getFScore();
			tempTile = _vOpenList[i];
		}
	}
	// 여기서 선택된 제일 저렴한것을 currentTile 로 다시 재귀함

	// 앙재귀띠
	pathFinder(startPos, endPos, PointMake(tempTile->getIdx().x, tempTile->getIdx().y), lMyWay);

	//언제까지? -> 도착점에 도착하거나, 검색할수 있는 길을 다 검색했지만 길을 못찾았을 경우
}

void aStar::pathFinder2(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay)
{
	//현재위치가 도착점이라면
	if (currentPos.x == endPos.x && currentPos.y == endPos.y)
	{
		POINT road = { (*_vTotalList)[currentPos.y][currentPos.x]->getIdx().x,(*_vTotalList)[currentPos.y][currentPos.x]->getIdx().y };

		lMyWay.clear();

		while (1)
		{
			if ((*_vTotalList)[road.y][road.x]->getParent() == nullptr)
			{
				break;
			}
			lMyWay.push_front(road);

			road = { (*_vTotalList)[road.y][road.x]->getParent()->getIdx().x, (*_vTotalList)[road.y][road.x]->getParent()->getIdx().y };
		}

		//재귀 탈출전 기초정보 리셋
		for (int i = 0; i < _vCloseList.size(); i++)
		{
			_vCloseList[i]->setParent(nullptr);
			_vCloseList[i]->setFScore(0);
			_vCloseList[i]->setGScore(0);
			_vCloseList[i]->setHScore(0);
		}

		for (int i = 0; i < _vOpenList.size(); i++)
		{
			_vOpenList[i]->setParent(nullptr);
			_vOpenList[i]->setFScore(0);
			_vOpenList[i]->setGScore(0);
			_vOpenList[i]->setHScore(0);
		}

		//오픈리스트와 클로즈리스트로 리셋
		_vOpenList.clear();
		_vCloseList.clear();

		//재귀 탈출
		return;
	}


	//오픈리스트들의 중심점을 만들좌
	int curIdxX = currentPos.x;
	int curIdxY = currentPos.y;

	//중심은 바로 클로즈리스트에 넣좌
	_vCloseList.push_back((*_vTotalList)[curIdxY][curIdxX]);

	//오픈목록에 내가 있는지 찾는다. 있으면 지워버림 -> 스타트 빼고 이걸 다함
	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (_vOpenList[i]->getIdx().x == curIdxX && _vOpenList[i]->getIdx().y == curIdxY)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			break;
		}
	}

	//이제 나의 주변타일을 검사하면서 오픈리스트에 넣자
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (abs(i) == abs(j)) continue;

			//벡터 크기 넘어가는거 예외처리
			if (curIdxX + i < 0 || curIdxX + i >(_vTotalList[0]).size() - 1
				|| curIdxY + j < 0 || curIdxY + j >(*_vTotalList).size() - 1)	continue;

			//일단 타일 만들어놓고
			tile* openTile = (*_vTotalList)[curIdxY + i][curIdxX + j];

			// 갈수 없는곳이면 건너뜀 -> 오브젝트랑 연동해서 해야할듯
			if (!openTile->getIsAvailMove())continue;
			if (openTile->getAttribute() == OBJ_WALL_BOSS || openTile->getAttribute() == OBJ_WALL_END) continue;
				
			//클로즈 리스트에 있는것도 건너뜀
			bool isClosed = false;

			for (int k = 0; k < _vCloseList.size(); k++)
			{
				if (openTile == _vCloseList[k])
				{
					isClosed = true;
					break;
				}
			}

			if (isClosed)continue;

			//예외처리에 걸리지 않으면 -> 벽이아니고, 닫힌목록에 있던얘도 아니라면

			//열린목록에 이미 있는놈인지 검사 (최적의 루트를 찾아줘야함)
			bool isOpened = false;

			for (int k = 0; k < _vOpenList.size(); k++)
			{
				//	if (openTile->getTopIdx().x == _vOpenList[k]->getTopIdx().x
				//		&& openTile->getTopIdx().y == _vOpenList[k]->getTopIdx().y)
				if (openTile == _vOpenList[k])
				{
					isOpened = true;
					break;
				}
			}

			// G , H 미리 구해놓자

			//G 연산 -> 현재 내타일과 부모타일의 거리를 비교해서 대각선인지, 수평or수직에 있는건지 판별
			int gScore = getDistance((*_vTotalList)[curIdxY][curIdxX]->getPos().x, (*_vTotalList)[curIdxY][curIdxX]->getPos().y,
				openTile->getPos().x, openTile->getPos().y) == TILESIZE ? 10 : 14;

			//H 연산 -> 현재 내 타일과 도착타일의 인덱스를 비교
			int hScore = (abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().x - openTile->getIdx().x)
				+ abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().y - openTile->getIdx().y)) * 10;


			//열린목록에 지금 새로 등록되는거면 그냥 바로 추가
			if (!isOpened)
			{
				openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

				//	openTile->setGScore(openTile->getParent()->getGScore() + gScore);

				openTile->setGScore(gScore);

				openTile->setHScore(hScore);

				openTile->setFScore(openTile->getGScore() + openTile->getHScore());

				_vOpenList.push_back(openTile);
			}
			//열린목록에 이미 있는놈이면 과거부모와 현재부모의 G값을 비교함 - > 현재부모의 G값이 더저렴하다면 재설정해줌
			if (isOpened)
			{
				if (openTile->getParent()->getGScore() > (*_vTotalList)[curIdxY][curIdxX]->getGScore())
				{
					//더나은경우가 나왔으니 일단 오픈타일에서 제거하자 (더나은값을넣고 다시 오픈타일에 넣을거임)
					for (int k = 0; k < _vOpenList.size(); k++)
					{
						if (_vOpenList[k] == openTile)
						{
							_vOpenList[k]->setParent(nullptr);
							_vOpenList[k]->setFScore(0);
							_vOpenList[k]->setGScore(0);
							_vOpenList[k]->setHScore(0);
							_vOpenList.erase(_vOpenList.begin() + k);
							break;
						}
					}

					// 부모를 바꿔주고
					openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

					//G 연산
					openTile->setGScore(openTile->getParent()->getGScore() + gScore);

					//H 연산
					openTile->setHScore(hScore);

					//F 연산
					openTile->setFScore(openTile->getGScore() + openTile->getHScore());

					_vOpenList.push_back(openTile);
				}
			}
		}
	}

	//길을 못찾았다면
	if ((_vOpenList.size() == 0 && _vCloseList.size() > 0) || _vCloseList.size() > 100)
	{
		//시작위치뱉고
		lMyWay.clear();
		lMyWay.push_front(PointMake(startPos.x, startPos.y));

		//재귀 탈출전 기초정보 리셋
		for (int i = 0; i < _vCloseList.size(); i++)
		{
			_vCloseList[i]->setParent(nullptr);
			_vCloseList[i]->setFScore(0);
			_vCloseList[i]->setGScore(0);
			_vCloseList[i]->setHScore(0);
		}

		for (int i = 0; i < _vOpenList.size(); i++)
		{
			_vOpenList[i]->setParent(nullptr);
			_vOpenList[i]->setFScore(0);
			_vOpenList[i]->setGScore(0);
			_vOpenList[i]->setHScore(0);
		}

		//오픈리스트와 클로즈리스트로 리셋
		_vOpenList.clear();
		_vCloseList.clear();

		//재귀 탈출
		return;
	}

	// 제일 저렴한 녀석을 찾아서 현재타일로 다시 박아줌
	int tempCost = 1000000000;
	tile* tempTile = NULL;

	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (tempCost > _vOpenList[i]->getFScore())
		{
			tempCost = _vOpenList[i]->getFScore();
			tempTile = _vOpenList[i];
		}
	}
	// 여기서 선택된 제일 저렴한것을 currentTile 로 다시 재귀함

	// 앙재귀띠
	pathFinder2(startPos, endPos, PointMake(tempTile->getIdx().x, tempTile->getIdx().y), lMyWay);

	//언제까지? -> 도착점에 도착하거나, 검색할수 있는 길을 다 검색했지만 길을 못찾았을 경우
}

void aStar::pathFinderShopkeeper(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay)
{
	//현재위치가 도착점이라면
	if (currentPos.x == endPos.x && currentPos.y == endPos.y)
	{
		POINT road = { (*_vTotalList)[currentPos.y][currentPos.x]->getIdx().x,(*_vTotalList)[currentPos.y][currentPos.x]->getIdx().y };

		lMyWay.clear();

		while (1)
		{
			if ((*_vTotalList)[road.y][road.x]->getParent() == nullptr)
			{
				break;
			}
			lMyWay.push_front(road);

			road = { (*_vTotalList)[road.y][road.x]->getParent()->getIdx().x, (*_vTotalList)[road.y][road.x]->getParent()->getIdx().y };
		}

		//재귀 탈출전 기초정보 리셋
		for (int i = 0; i < _vCloseList.size(); i++)
		{
			_vCloseList[i]->setParent(nullptr);
			_vCloseList[i]->setFScore(0);
			_vCloseList[i]->setGScore(0);
			_vCloseList[i]->setHScore(0);
		}

		for (int i = 0; i < _vOpenList.size(); i++)
		{
			_vOpenList[i]->setParent(nullptr);
			_vOpenList[i]->setFScore(0);
			_vOpenList[i]->setGScore(0);
			_vOpenList[i]->setHScore(0);
		}

		//오픈리스트와 클로즈리스트로 리셋
		_vOpenList.clear();
		_vCloseList.clear();

		//재귀 탈출
		return;
	}


	//오픈리스트들의 중심점을 만들좌
	int curIdxX = currentPos.x;
	int curIdxY = currentPos.y;

	//중심은 바로 클로즈리스트에 넣좌
	_vCloseList.push_back((*_vTotalList)[curIdxY][curIdxX]);

	//오픈목록에 내가 있는지 찾는다. 있으면 지워버림 -> 스타트 빼고 이걸 다함
	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (_vOpenList[i]->getIdx().x == curIdxX && _vOpenList[i]->getIdx().y == curIdxY)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			break;
		}
	}

	//이제 나의 주변타일을 검사하면서 오픈리스트에 넣자
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (abs(i) == abs(j)) continue;

			//벡터 크기 넘어가는거 예외처리
			if (curIdxX + i < 0 || curIdxX + i >(_vTotalList[0]).size() - 1
				|| curIdxY + j < 0 || curIdxY + j >(*_vTotalList).size() - 1)	continue;

			//일단 타일 만들어놓고
			tile* openTile = (*_vTotalList)[curIdxY + i][curIdxX + j];

			// 갈수 없는곳이면 건너뜀 -> 오브젝트랑 연동해서 해야할듯
			if ((openTile->getAttribute() >= OBJ_WALL1 && openTile->getAttribute() >= OBJ_WALL_END) ||
				(openTile->getAttribute() >= ETC_TORCH_WALL1 && openTile->getAttribute() >= ETC_TORCH_BOSS)) continue;

			//클로즈 리스트에 있는것도 건너뜀
			bool isClosed = false;

			for (int k = 0; k < _vCloseList.size(); k++)
			{
				if (openTile == _vCloseList[k])
				{
					isClosed = true;
					break;
				}
			}

			if (isClosed)continue;

			//예외처리에 걸리지 않으면 -> 벽이아니고, 닫힌목록에 있던얘도 아니라면

			//열린목록에 이미 있는놈인지 검사 (최적의 루트를 찾아줘야함)
			bool isOpened = false;

			for (int k = 0; k < _vOpenList.size(); k++)
			{
				//	if (openTile->getTopIdx().x == _vOpenList[k]->getTopIdx().x
				//		&& openTile->getTopIdx().y == _vOpenList[k]->getTopIdx().y)
				if (openTile == _vOpenList[k])
				{
					isOpened = true;
					break;
				}
			}

			// G , H 미리 구해놓자

			//G 연산 -> 현재 내타일과 부모타일의 거리를 비교해서 대각선인지, 수평or수직에 있는건지 판별
			int gScore = getDistance((*_vTotalList)[curIdxY][curIdxX]->getPos().x, (*_vTotalList)[curIdxY][curIdxX]->getPos().y,
				openTile->getPos().x, openTile->getPos().y) == TILESIZE ? 10 : 14;

			//H 연산 -> 현재 내 타일과 도착타일의 인덱스를 비교
			int hScore = (abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().x - openTile->getIdx().x)
				+ abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().y - openTile->getIdx().y)) * 10;


			//열린목록에 지금 새로 등록되는거면 그냥 바로 추가
			if (!isOpened)
			{
				openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

				//	openTile->setGScore(openTile->getParent()->getGScore() + gScore);

				openTile->setGScore(gScore);

				openTile->setHScore(hScore);

				openTile->setFScore(openTile->getGScore() + openTile->getHScore());

				_vOpenList.push_back(openTile);
			}
			//열린목록에 이미 있는놈이면 과거부모와 현재부모의 G값을 비교함 - > 현재부모의 G값이 더저렴하다면 재설정해줌
			if (isOpened)
			{
				if (openTile->getParent()->getGScore() > (*_vTotalList)[curIdxY][curIdxX]->getGScore())
				{
					//더나은경우가 나왔으니 일단 오픈타일에서 제거하자 (더나은값을넣고 다시 오픈타일에 넣을거임)
					for (int k = 0; k < _vOpenList.size(); k++)
					{
						if (_vOpenList[k] == openTile)
						{
							_vOpenList[k]->setParent(nullptr);
							_vOpenList[k]->setFScore(0);
							_vOpenList[k]->setGScore(0);
							_vOpenList[k]->setHScore(0);
							_vOpenList.erase(_vOpenList.begin() + k);
							break;
						}
					}

					// 부모를 바꿔주고
					openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

					//G 연산
					openTile->setGScore(openTile->getParent()->getGScore() + gScore);

					//H 연산
					openTile->setHScore(hScore);

					//F 연산
					openTile->setFScore(openTile->getGScore() + openTile->getHScore());

					_vOpenList.push_back(openTile);
				}
			}
		}
	}

	//길을 못찾았다면
	if ((_vOpenList.size() == 0 && _vCloseList.size() > 0) || _vCloseList.size() > 100)
	{
		//시작위치뱉고
		lMyWay.clear();
		lMyWay.push_front(PointMake(startPos.x, startPos.y));

		//재귀 탈출전 기초정보 리셋
		for (int i = 0; i < _vCloseList.size(); i++)
		{
			_vCloseList[i]->setParent(nullptr);
			_vCloseList[i]->setFScore(0);
			_vCloseList[i]->setGScore(0);
			_vCloseList[i]->setHScore(0);
		}

		for (int i = 0; i < _vOpenList.size(); i++)
		{
			_vOpenList[i]->setParent(nullptr);
			_vOpenList[i]->setFScore(0);
			_vOpenList[i]->setGScore(0);
			_vOpenList[i]->setHScore(0);
		}

		//오픈리스트와 클로즈리스트로 리셋
		_vOpenList.clear();
		_vCloseList.clear();

		//재귀 탈출
		return;
	}

	// 제일 저렴한 녀석을 찾아서 현재타일로 다시 박아줌
	int tempCost = 1000000000;
	tile* tempTile = NULL;

	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (tempCost > _vOpenList[i]->getFScore())
		{
			tempCost = _vOpenList[i]->getFScore();
			tempTile = _vOpenList[i];
		}
	}
	// 여기서 선택된 제일 저렴한것을 currentTile 로 다시 재귀함

	// 앙재귀띠
	pathFinderShopkeeper(startPos, endPos, PointMake(tempTile->getIdx().x, tempTile->getIdx().y), lMyWay);

	//언제까지? -> 도착점에 도착하거나, 검색할수 있는 길을 다 검색했지만 길을 못찾았을 경우
}

