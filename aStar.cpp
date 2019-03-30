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
	//������ġ�� �������̶��
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

		//��� Ż���� �������� ����
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

		//���¸���Ʈ�� Ŭ�����Ʈ�� ����
		_vOpenList.clear();
		_vCloseList.clear();

		//��� Ż��
		return;
	}


	//���¸���Ʈ���� �߽����� ������
	int curIdxX = currentPos.x;
	int curIdxY = currentPos.y;

	//�߽��� �ٷ� Ŭ�����Ʈ�� ����
	_vCloseList.push_back((*_vTotalList)[curIdxY][curIdxX]);

	//���¸�Ͽ� ���� �ִ��� ã�´�. ������ �������� -> ��ŸƮ ���� �̰� ����
	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (_vOpenList[i]->getIdx().x == curIdxX && _vOpenList[i]->getIdx().y == curIdxY)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			break;
		}
	}

	//���� ���� �ֺ�Ÿ���� �˻��ϸ鼭 ���¸���Ʈ�� ����
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (abs(i) == abs(j)) continue;

			//���� ũ�� �Ѿ�°� ����ó��
			if (curIdxX + i < 0 || curIdxX + i > (_vTotalList[0]).size() - 1
				|| curIdxY + j < 0 || curIdxY + j >(*_vTotalList).size() - 1)	continue;
			
			//�ϴ� Ÿ�� ��������
			tile* openTile = (*_vTotalList)[curIdxY + i][curIdxX + j];

			// ���� ���°��̸� �ǳʶ� -> ������Ʈ�� �����ؼ� �ؾ��ҵ�
			if (!openTile->getIsAvailMove()) continue;

			//Ŭ���� ����Ʈ�� �ִ°͵� �ǳʶ�
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

			//����ó���� �ɸ��� ������ -> ���̾ƴϰ�, ������Ͽ� �ִ��굵 �ƴ϶��

			//������Ͽ� �̹� �ִ³����� �˻� (������ ��Ʈ�� ã�������)
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

			// G , H �̸� ���س���

			//G ���� -> ���� ��Ÿ�ϰ� �θ�Ÿ���� �Ÿ��� ���ؼ� �밢������, ����or������ �ִ°��� �Ǻ�
			int gScore = getDistance((*_vTotalList)[curIdxY][curIdxX]->getPos().x, (*_vTotalList)[curIdxY][curIdxX]->getPos().y,
				openTile->getPos().x, openTile->getPos().y) == TILESIZE ? 10 : 14;

			//H ���� -> ���� �� Ÿ�ϰ� ����Ÿ���� �ε����� ��
			int hScore = (abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().x - openTile->getIdx().x)
				+ abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().y - openTile->getIdx().y)) * 10;


			//������Ͽ� ���� ���� ��ϵǴ°Ÿ� �׳� �ٷ� �߰�
			if (!isOpened)
			{
				openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

			//	openTile->setGScore(openTile->getParent()->getGScore() + gScore);

				openTile->setGScore(gScore);

				openTile->setHScore(hScore);

				openTile->setFScore(openTile->getGScore() + openTile->getHScore());

				_vOpenList.push_back(openTile);
			}
			//������Ͽ� �̹� �ִ³��̸� ���źθ�� ����θ��� G���� ���� - > ����θ��� G���� �������ϴٸ� �缳������
			if (isOpened)
			{
				if (openTile->getParent()->getGScore()  > (*_vTotalList)[curIdxY][curIdxX]->getGScore())
				{
					//��������찡 �������� �ϴ� ����Ÿ�Ͽ��� �������� (�����������ְ� �ٽ� ����Ÿ�Ͽ� ��������)
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

					// �θ� �ٲ��ְ�
					openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

					//G ����
					openTile->setGScore(openTile->getParent()->getGScore() + gScore);

					//H ����
					openTile->setHScore(hScore);

					//F ����
					openTile->setFScore(openTile->getGScore() + openTile->getHScore());

					_vOpenList.push_back(openTile);
				}
			}
		}
	}

	//���� ��ã�Ҵٸ�
	if ((_vOpenList.size() == 0 && _vCloseList.size() > 0) || _vCloseList.size() > 100)
	{
		//������ġ���
		lMyWay.clear();
		lMyWay.push_front(PointMake(startPos.x, startPos.y));

		//��� Ż���� �������� ����
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

		//���¸���Ʈ�� Ŭ�����Ʈ�� ����
		_vOpenList.clear();
		_vCloseList.clear();

		//��� Ż��
		return;
	}

	// ���� ������ �༮�� ã�Ƽ� ����Ÿ�Ϸ� �ٽ� �ھ���
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
	// ���⼭ ���õ� ���� �����Ѱ��� currentTile �� �ٽ� �����

	// ����Ͷ�
	pathFinder(startPos, endPos, PointMake(tempTile->getIdx().x, tempTile->getIdx().y), lMyWay);

	//��������? -> �������� �����ϰų�, �˻��Ҽ� �ִ� ���� �� �˻������� ���� ��ã���� ���
}

void aStar::pathFinder2(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay)
{
	//������ġ�� �������̶��
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

		//��� Ż���� �������� ����
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

		//���¸���Ʈ�� Ŭ�����Ʈ�� ����
		_vOpenList.clear();
		_vCloseList.clear();

		//��� Ż��
		return;
	}


	//���¸���Ʈ���� �߽����� ������
	int curIdxX = currentPos.x;
	int curIdxY = currentPos.y;

	//�߽��� �ٷ� Ŭ�����Ʈ�� ����
	_vCloseList.push_back((*_vTotalList)[curIdxY][curIdxX]);

	//���¸�Ͽ� ���� �ִ��� ã�´�. ������ �������� -> ��ŸƮ ���� �̰� ����
	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (_vOpenList[i]->getIdx().x == curIdxX && _vOpenList[i]->getIdx().y == curIdxY)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			break;
		}
	}

	//���� ���� �ֺ�Ÿ���� �˻��ϸ鼭 ���¸���Ʈ�� ����
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (abs(i) == abs(j)) continue;

			//���� ũ�� �Ѿ�°� ����ó��
			if (curIdxX + i < 0 || curIdxX + i >(_vTotalList[0]).size() - 1
				|| curIdxY + j < 0 || curIdxY + j >(*_vTotalList).size() - 1)	continue;

			//�ϴ� Ÿ�� ��������
			tile* openTile = (*_vTotalList)[curIdxY + i][curIdxX + j];

			// ���� ���°��̸� �ǳʶ� -> ������Ʈ�� �����ؼ� �ؾ��ҵ�
			if (!openTile->getIsAvailMove())continue;
			if (openTile->getAttribute() == OBJ_WALL_BOSS || openTile->getAttribute() == OBJ_WALL_END) continue;
				
			//Ŭ���� ����Ʈ�� �ִ°͵� �ǳʶ�
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

			//����ó���� �ɸ��� ������ -> ���̾ƴϰ�, ������Ͽ� �ִ��굵 �ƴ϶��

			//������Ͽ� �̹� �ִ³����� �˻� (������ ��Ʈ�� ã�������)
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

			// G , H �̸� ���س���

			//G ���� -> ���� ��Ÿ�ϰ� �θ�Ÿ���� �Ÿ��� ���ؼ� �밢������, ����or������ �ִ°��� �Ǻ�
			int gScore = getDistance((*_vTotalList)[curIdxY][curIdxX]->getPos().x, (*_vTotalList)[curIdxY][curIdxX]->getPos().y,
				openTile->getPos().x, openTile->getPos().y) == TILESIZE ? 10 : 14;

			//H ���� -> ���� �� Ÿ�ϰ� ����Ÿ���� �ε����� ��
			int hScore = (abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().x - openTile->getIdx().x)
				+ abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().y - openTile->getIdx().y)) * 10;


			//������Ͽ� ���� ���� ��ϵǴ°Ÿ� �׳� �ٷ� �߰�
			if (!isOpened)
			{
				openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

				//	openTile->setGScore(openTile->getParent()->getGScore() + gScore);

				openTile->setGScore(gScore);

				openTile->setHScore(hScore);

				openTile->setFScore(openTile->getGScore() + openTile->getHScore());

				_vOpenList.push_back(openTile);
			}
			//������Ͽ� �̹� �ִ³��̸� ���źθ�� ����θ��� G���� ���� - > ����θ��� G���� �������ϴٸ� �缳������
			if (isOpened)
			{
				if (openTile->getParent()->getGScore() > (*_vTotalList)[curIdxY][curIdxX]->getGScore())
				{
					//��������찡 �������� �ϴ� ����Ÿ�Ͽ��� �������� (�����������ְ� �ٽ� ����Ÿ�Ͽ� ��������)
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

					// �θ� �ٲ��ְ�
					openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

					//G ����
					openTile->setGScore(openTile->getParent()->getGScore() + gScore);

					//H ����
					openTile->setHScore(hScore);

					//F ����
					openTile->setFScore(openTile->getGScore() + openTile->getHScore());

					_vOpenList.push_back(openTile);
				}
			}
		}
	}

	//���� ��ã�Ҵٸ�
	if ((_vOpenList.size() == 0 && _vCloseList.size() > 0) || _vCloseList.size() > 100)
	{
		//������ġ���
		lMyWay.clear();
		lMyWay.push_front(PointMake(startPos.x, startPos.y));

		//��� Ż���� �������� ����
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

		//���¸���Ʈ�� Ŭ�����Ʈ�� ����
		_vOpenList.clear();
		_vCloseList.clear();

		//��� Ż��
		return;
	}

	// ���� ������ �༮�� ã�Ƽ� ����Ÿ�Ϸ� �ٽ� �ھ���
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
	// ���⼭ ���õ� ���� �����Ѱ��� currentTile �� �ٽ� �����

	// ����Ͷ�
	pathFinder2(startPos, endPos, PointMake(tempTile->getIdx().x, tempTile->getIdx().y), lMyWay);

	//��������? -> �������� �����ϰų�, �˻��Ҽ� �ִ� ���� �� �˻������� ���� ��ã���� ���
}

void aStar::pathFinderShopkeeper(POINT startPos, POINT endPos, POINT currentPos, list<POINT>& lMyWay)
{
	//������ġ�� �������̶��
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

		//��� Ż���� �������� ����
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

		//���¸���Ʈ�� Ŭ�����Ʈ�� ����
		_vOpenList.clear();
		_vCloseList.clear();

		//��� Ż��
		return;
	}


	//���¸���Ʈ���� �߽����� ������
	int curIdxX = currentPos.x;
	int curIdxY = currentPos.y;

	//�߽��� �ٷ� Ŭ�����Ʈ�� ����
	_vCloseList.push_back((*_vTotalList)[curIdxY][curIdxX]);

	//���¸�Ͽ� ���� �ִ��� ã�´�. ������ �������� -> ��ŸƮ ���� �̰� ����
	for (int i = 0; i < _vOpenList.size(); i++)
	{
		if (_vOpenList[i]->getIdx().x == curIdxX && _vOpenList[i]->getIdx().y == curIdxY)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			break;
		}
	}

	//���� ���� �ֺ�Ÿ���� �˻��ϸ鼭 ���¸���Ʈ�� ����
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (abs(i) == abs(j)) continue;

			//���� ũ�� �Ѿ�°� ����ó��
			if (curIdxX + i < 0 || curIdxX + i >(_vTotalList[0]).size() - 1
				|| curIdxY + j < 0 || curIdxY + j >(*_vTotalList).size() - 1)	continue;

			//�ϴ� Ÿ�� ��������
			tile* openTile = (*_vTotalList)[curIdxY + i][curIdxX + j];

			// ���� ���°��̸� �ǳʶ� -> ������Ʈ�� �����ؼ� �ؾ��ҵ�
			if ((openTile->getAttribute() >= OBJ_WALL1 && openTile->getAttribute() >= OBJ_WALL_END) ||
				(openTile->getAttribute() >= ETC_TORCH_WALL1 && openTile->getAttribute() >= ETC_TORCH_BOSS)) continue;

			//Ŭ���� ����Ʈ�� �ִ°͵� �ǳʶ�
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

			//����ó���� �ɸ��� ������ -> ���̾ƴϰ�, ������Ͽ� �ִ��굵 �ƴ϶��

			//������Ͽ� �̹� �ִ³����� �˻� (������ ��Ʈ�� ã�������)
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

			// G , H �̸� ���س���

			//G ���� -> ���� ��Ÿ�ϰ� �θ�Ÿ���� �Ÿ��� ���ؼ� �밢������, ����or������ �ִ°��� �Ǻ�
			int gScore = getDistance((*_vTotalList)[curIdxY][curIdxX]->getPos().x, (*_vTotalList)[curIdxY][curIdxX]->getPos().y,
				openTile->getPos().x, openTile->getPos().y) == TILESIZE ? 10 : 14;

			//H ���� -> ���� �� Ÿ�ϰ� ����Ÿ���� �ε����� ��
			int hScore = (abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().x - openTile->getIdx().x)
				+ abs((*_vTotalList)[endPos.y][endPos.x]->getIdx().y - openTile->getIdx().y)) * 10;


			//������Ͽ� ���� ���� ��ϵǴ°Ÿ� �׳� �ٷ� �߰�
			if (!isOpened)
			{
				openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

				//	openTile->setGScore(openTile->getParent()->getGScore() + gScore);

				openTile->setGScore(gScore);

				openTile->setHScore(hScore);

				openTile->setFScore(openTile->getGScore() + openTile->getHScore());

				_vOpenList.push_back(openTile);
			}
			//������Ͽ� �̹� �ִ³��̸� ���źθ�� ����θ��� G���� ���� - > ����θ��� G���� �������ϴٸ� �缳������
			if (isOpened)
			{
				if (openTile->getParent()->getGScore() > (*_vTotalList)[curIdxY][curIdxX]->getGScore())
				{
					//��������찡 �������� �ϴ� ����Ÿ�Ͽ��� �������� (�����������ְ� �ٽ� ����Ÿ�Ͽ� ��������)
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

					// �θ� �ٲ��ְ�
					openTile->setParent((*_vTotalList)[curIdxY][curIdxX]);

					//G ����
					openTile->setGScore(openTile->getParent()->getGScore() + gScore);

					//H ����
					openTile->setHScore(hScore);

					//F ����
					openTile->setFScore(openTile->getGScore() + openTile->getHScore());

					_vOpenList.push_back(openTile);
				}
			}
		}
	}

	//���� ��ã�Ҵٸ�
	if ((_vOpenList.size() == 0 && _vCloseList.size() > 0) || _vCloseList.size() > 100)
	{
		//������ġ���
		lMyWay.clear();
		lMyWay.push_front(PointMake(startPos.x, startPos.y));

		//��� Ż���� �������� ����
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

		//���¸���Ʈ�� Ŭ�����Ʈ�� ����
		_vOpenList.clear();
		_vCloseList.clear();

		//��� Ż��
		return;
	}

	// ���� ������ �༮�� ã�Ƽ� ����Ÿ�Ϸ� �ٽ� �ھ���
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
	// ���⼭ ���õ� ���� �����Ѱ��� currentTile �� �ٽ� �����

	// ����Ͷ�
	pathFinderShopkeeper(startPos, endPos, PointMake(tempTile->getIdx().x, tempTile->getIdx().y), lMyWay);

	//��������? -> �������� �����ϰų�, �˻��Ҽ� �ִ� ���� �� �˻������� ���� ��ã���� ���
}

