#include "stdafx.h"
#include "rayCast.h"


rayCast::rayCast()
{
}


rayCast::~rayCast()
{
}

void rayCast::rayCasting(POINT playerIdx, int torchRange)
{
	for (int i = -torchRange; i <= +torchRange; i++)
	{
		for (int j = -torchRange; j <= +torchRange; j++)
		{
			//��������
			if (playerIdx.y + i < 0 || playerIdx.x + j < 0
				||playerIdx.y + i > _vvLightMap->size() - 1
				||playerIdx.x + j > _vvLightMap[0][0].size() - 1)
			{
				continue;
			}
			
			float cost = 0;					//�ڽ�Ʈ�� 0���� �����ؼ� �ڽ�Ʈ�� �������� (�̵��� �����Ҽ���) ������ ������ == ����

			int maxCost = torchRange * 10;	//�ڽ�Ʈ �ִ��ѵ��� ȶ�Ҽ����� * 10
			POINT cal = { j,i };			//�ε��� ����([0][0]�� �÷��̾���)

			while (1)
			{
				//=====================
				//		Ż��
				//=====================
				if (cal.x == 0 && cal.y == 0)
				{
					//�����ϸ� Ż��
					break;
				}
				if (cost > maxCost)
				{
					//�ڽ�Ʈ�� �����Ǹ� Ż��
					break;
				}

				//���� �ε����� ������ ���ٸ� == �밢���̶��
				if (abs(cal.x) == abs(cal.y))
				{
					if (cal.x != 0 && cal.y != 0)
					{
						//�밢���� 16�����ϰ�
						cost += 16;

						//0���� ū�� �������� �Ǻ��ؼ� ++ or --
						if (cal.x > 0)
						{
							cal.x--;
						}
						else if (cal.x < 0)
						{
							cal.x++;
						}

						if (cal.y > 0)
						{
							cal.y--;
						}
						else if (cal.y < 0)
						{
							cal.y++;
						}

						//���� �ε����� ���� �����ؼ� ���ϱ� ���̾���? �׷� �ڽ�Ʈ ������
						if ((*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
							(*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
						{
							cost += WALL_COST;
						}
					}
				}

				//x�� 0�� �ƴ϶��  0�̵ɋ����� ++ or --
				if (cal.x != 0)
				{
					cost += 10;

					//��ȣ������
					if (cal.x > 0)
					{
						//���̰ų�
						cal.x--;
					}
					else if (cal.x < 0)
					{
						//����
						cal.x++;
					}

					if ((*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
						(*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
					{
						cost += WALL_COST;
					}
				}
				//y�� 0�� �ƴ϶�� 0�̵ɋ����� ++ or --
				if (cal.y != 0)
				{
					cost += 10;

					if (cal.y > 0)
					{
						cal.y--;
					}
					else if (cal.y < 0)
					{
						cal.y++;
					}
					if (playerIdx.x + j >= 0 && playerIdx.x + j <= _vvLightMap[0][0].size() - 1 &&
						playerIdx.y + i >= 0 && playerIdx.y + i <= _vvLightMap->size() - 1)
					{

						if ((*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
							(*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
						{
							cost += WALL_COST;
						}
					}
				}
			}
			// ���ѷ��������� Ż�����޴�. 2������ ����� ��.

			//	1. �ڽ�Ʈ�� �ٽἭ ���������(�����ȿ� ���������)
			if (cost > maxCost)
			{
				//1) �߰��غô� ���̾����ÿ�
				if (playerIdx.x + j >= 0 && playerIdx.x + j <= _vvLightMap[0][0].size() - 1 &&
					playerIdx.y + i >= 0 && playerIdx.y + i <= _vvLightMap->size() - 1)
				{
					if ((*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->getIsFind())
					{
						//1-1) �װ��� �ٽ� �߰��������� ������ ĥ��
						(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setOpacity(OPACITY_FIND);
					}
					//2) �̾Ʒ��� �� �ǳʶ�
					continue;
				}
			}
			
			//	2. �ִ� �ڽ�Ʈ �ȿ� �����������(�����ȿ� �־������)

			// 1)�߰��Ѱɷ� ����.
			(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setIsFind(true);
			// 2)�ڽ�Ʈ�� ������ �ٽ� �����
			cost /= maxCost;	//�ִ� �ڽ�Ʈ�� ������
			cost *= 0.4f;		//0.4�� ���ϴµ� ���ϸ� �ʹ� ����. 

			// 3)���� ����� ���� �ڽ�Ʈ�� ���� �ε����� ����(���� �ε������� �����ε��� i,j)

			(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setOpacity(cost);
			
			//���⿡�ٰ� ������Ʈ[playerIdx.y + i][playerIdx.x + j]�� �Ӽ��� Ƚ�ҵ� �ִٸ� rayCast�� ��ͷ� ���� +�߰��� i�� j�� 0,0�� �ƴϰ�
			if ((*_vvObj)[playerIdx.y + i][playerIdx.x + j]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + i][playerIdx.x + j]->getAttribute() <= ETC_TORCH_BOSS)
			{
				rayCastingTorch({ playerIdx.x + j , playerIdx.y + i }, 4);
			}

			//���� �Ʒ������̶� ���� �� ĥ����ߵǼ� �߰��Ѱ�.
			if (playerIdx.y + i >= 0 && playerIdx.x + i >= 0									// ��������
				&&playerIdx.y <= _vvObj->size() && playerIdx.x <= _vvObj[0][0].size()				// ��������
				&& (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
				(*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)	// �Ӽ��� ���̸�
			{
				//��������
				if (playerIdx.y + i - 1 >= 0 && playerIdx.y + i - 1 <= _vvLightMap->size())
				{
					//���϶� �ٷ� �����ٰ� ���� ���� �ڽ�Ʈ�� �߰��ߴٰ� ����.
					(*_vvLightMap)[playerIdx.y + i - 1][playerIdx.x + j]->setIsFind(true);
					(*_vvLightMap)[playerIdx.y + i - 1][playerIdx.x + j]->setOpacity(cost);
				}
			}
		}
	}

	//���⼭���� �ִ� ���ǹ����� ������ �˻��Ѱ��� 1ĭ�� �Ѻκ��� �߰��ߴ��Ŷ�� �������� ������ ĥ���ִ� ����

	//��������
	if (playerIdx.y - (torchRange + 1) >= 0 && playerIdx.y - (torchRange + 1) <= _vvLightMap->size() - 1
		&&(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->getIsFind())
	{
		//ĥ�ϰ�
		(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->setOpacity(OPACITY_FIND);

		//��������
		if (playerIdx.x - 1 >= 0)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x - 1]->setOpacity(OPACITY_FIND);
		}
		//��������
		if (playerIdx.x + 1 <= _vvLightMap->size() - 1)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x + 1]->setOpacity(OPACITY_FIND);
		}
	}

	//��������
	if (playerIdx.y + (torchRange + 1) >= 0 && playerIdx.y + (torchRange + 1) <= _vvLightMap->size() - 1
		&&(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->getIsFind())
	{
		//ĥ�ϰ�
		(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->setOpacity(OPACITY_FIND);
		//��������
		if (playerIdx.x - 1 >= 0)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x - 1]->setOpacity(OPACITY_FIND);
		}
		//��������
		if (playerIdx.x + 1 <= _vvLightMap[0].size() - 1)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x + 1]->setOpacity(OPACITY_FIND);
		}
	}

	//��������
	if (playerIdx.x - (torchRange + 1) >= 0 && playerIdx.x - (torchRange + 1) <= _vvLightMap[0][0].size() - 1
		&&(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->getIsFind())
	{
		//ĥ�ϰ�
		(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->setOpacity(OPACITY_FIND);
		//��������
		if (playerIdx.y - 1 >= 0)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y - 1][playerIdx.x - (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
		//��������
		if (playerIdx.y + 1 <= _vvLightMap->size() - 1)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y + 1][playerIdx.x - (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
	}
	//��������
	if (playerIdx.x + (torchRange + 1) >= 0 && playerIdx.x + (torchRange + 1) <= _vvLightMap[0][0].size() - 1
		&&(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->getIsFind())
	{
		//ĥ�ϰ�
		(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->setOpacity(OPACITY_FIND);
		//��������
		if (playerIdx.y - 1 >= 0)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y - 1][playerIdx.x + (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
		//��������
		if (playerIdx.y + 1 <= _vvLightMap->size() - 1)
		{
			//ĥ�ϰ�
			(*_vvLightMap)[playerIdx.y + 1][playerIdx.x + (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
	}



	//���� 
	{
		//for (int i = startY ; i <= endY; i++)
		//{
		//	for (int j = startX ; j <= endX ; j++)
		//	{
		//		//Ƚ�� �⺻ 2��(���۸����� �ִ� 5����)
		//		int cost = torchRange * 10;
		//		POINT index = { j,i };
		//
		//		//�׽�Ʈ��
		//		if (i == 9)
		//		{
		//			if (j == 11)
		//			{
		//				POINT index = { j,i };
		//			}
		//		}
		//
		//		while (1)
		//		{
		//			//=====================
		//			//		Ż��
		//			//=====================
		//			if (index.x == playerIdx.x && index.y == playerIdx.y)
		//			{
		//				//������ �ڽ�Ʈ�� �پ����ʰ� �����߰ų�
		//				break;
		//			}
		//			if (cost < 0)
		//			{
		//				//�ڽ�Ʈ�� �پ��� Ż��
		//				break;
		//			}
		//			
		//			//====================
		//			//		�밢��
		//			//====================
		//			if (abs(index.x) == abs(index.y))							//x�� y�� ���밪�� ���� ������ (�밢 or ����)
		//			{
		//				//x�� y�� �������� �ƴҽ� -> �밢���� ����
		//				if (index.x != playerIdx.x && index.y != playerIdx.y)
		//				{
		//					//16�� ����
		//					cost -= 16;
		//
		//					//���밪���� ��������� ��ȣ�� ���� ++ or --
		//					if (index.x > playerIdx.x)
		//					{
		//						index.x--;
		//					}
		//					else if (index.x < playerIdx.x)
		//					{
		//						index.x++;
		//					}
		//
		//					if (index.y > playerIdx.y)
		//					{
		//						index.y--;
		//					}
		//					else if (index.y < playerIdx.y)
		//					{
		//						index.y++;
		//					}
		//				}
		//			}
		//
		//			//x�� �÷��̾� ��ǥ�� ����������
		//			if (index.x != playerIdx.x)
		//			{
		//				cost -= 10;
		//
		//				//��ȣ������
		//				if (index.x > playerIdx.x)
		//				{
		//					index.x--;
		//				}
		//				else if (index.x < playerIdx.x)
		//				{
		//					index.x++;
		//				}
		//			}
		//
		//			//y�� �÷��̾���ǥ�� ����������
		//			if (index.y != playerIdx.y)
		//			{
		//				cost -= 10;
		//
		//				if (index.y > playerIdx.y)
		//				{
		//					index.y--;
		//				}
		//				else if (index.y < playerIdx.y)
		//				{
		//					index.y++;
		//				}
		//			}
		//		}
		//
		//		//=========================================================================================================================================================
		//		// ���� ���ѷ��������� Ż�����޴�. 2������ ����� ��.
		//		//	1. ȶ�ҷ� ���ؼ� ������ �ڽ�Ʈ���� �÷��̾����� ������ -> �þ� ����
		//		//	2. �ڽ�Ʈ�� �ٽἭ ����. -> �ٷξƷ��� �������� �ɷ��� �þ߼����ϴºκ� �ǳʶ�
		//		//=========================================================================================================================================================
		//
		//		//========================================================================
		//		//						2���� ��� ���⼭ �ɸ�
		//		//========================================================================
		//		if (cost < 0)
		//		{
		//			//���� �ڽ�Ʈ�� 0���� ������? ��Ÿ� �ۿ��ִ°Ŵϱ� �� �ǳʶ�.
		//
		//			if ((*_vvLightMap)[i][j]->getIsFind())
		//			{
		//				(*_vvLightMap)[i][j]->setOpacity(0.7f);
		//			}
		//
		//			continue;
		//		}
		//
		//		//========================================================================
		//		//						1���� ��� ��������ͼ� �þ߼�������.
		//		//========================================================================
		//
		//		//�ڽ�Ʈ�� ��������(�÷��̾�� �������) opacity�� 1�� ������ �ڽ�Ʈ�� ��������(�÷��̾�� �ְ��) opacity �� 0�� ������
		//		(*_vvLightMap)[i][j]->setIsFind(true);
		//
		//		if (cost % 10 != 0)
		//		{
		//			cost += 6;
		//		}
		//		float f_cost = 1.0f - ((float)cost / (torchRange * 10));
		//
		//		//����ġ ������ �����ִ� ������ ���� �ǳ������� �ڽ�Ʈ�� �� 0�̶� ���ݾ� ������.
		//		switch (torchRange)
		//		{
		//			case 2:		//�⺻ ȶ��
		//				f_cost -= 0.3f;
		//			break;
		//			case 3:		// +1 ȶ��
		//				f_cost -= 0.4f;
		//			break;
		//			case 4:		// +2 ȶ��
		//				f_cost -= 0.5f;
		//			break;
		//			case 5:		// +3 ȶ��
		//				f_cost -= 0.6f;
		//			break;
		//		}
		//		
		//		//������ opacity���� �־���
		//		(*_vvLightMap)[i][j]->setOpacity(f_cost);
		//
		//	}
		//}
		//
		//(*_vvLightMap)[playerIdx.y][playerIdx.x]->setOpacity(0);
		//if ((*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->getIsFind())
		//{
		//	(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->setOpacity(0.7f);
		//}
		//if ((*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->getIsFind())
		//{
		//	(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->setOpacity(0.7f);
		//}
		//if ((*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->getIsFind())
		//{
		//	(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->setOpacity(0.7f);
		//}
		//if ((*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->getIsFind())
		//{
		//	(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->setOpacity(0.7f);
		//}
	}

}

void rayCast::rayCastingTorch(POINT torchIdx, int torchRange)
{
	for (int i = -torchRange; i <= +torchRange; i++)
	{
		for (int j = -torchRange; j <= +torchRange; j++)
		{
			//��������
			if (torchIdx.y + i < 0 || torchIdx.x + j < 0
				|| torchIdx.y + i > _vvLightMap->size() - 1
				|| torchIdx.x + j > _vvLightMap[0][0].size() - 1)
			{
				continue;
			}
			if (i == 0 && j == 0)continue;

			float cost = 0;					//�ڽ�Ʈ�� 0���� �����ؼ� �ڽ�Ʈ�� �������� (�̵��� �����Ҽ���) ������ ������ == ����

			int maxCost = torchRange * 10;	//�ڽ�Ʈ �ִ��ѵ��� ȶ�Ҽ����� * 10
			POINT cal = { j,i };			//�ε��� ����([0][0]�� �÷��̾���)

			while (1)
			{
				//=====================
				//		Ż��
				//=====================
				if (cal.x == 0 && cal.y == 0)
				{
					//�����ϸ� Ż��
					break;
				}
				if (cost > maxCost)
				{
					//�ڽ�Ʈ�� �����Ǹ� Ż��
					break;
				}

				//���� �ε����� ������ ���ٸ� == �밢���̶��
				if (abs(cal.x) == abs(cal.y))
				{
					if (cal.x != 0 && cal.y != 0)
					{
						//�밢���� 16�����ϰ�
						cost += 16;

						//0���� ū�� �������� �Ǻ��ؼ� ++ or --
						if (cal.x > 0)
						{
							cal.x--;
						}
						else if (cal.x < 0)
						{
							cal.x++;
						}

						if (cal.y > 0)
						{
							cal.y--;
						}
						else if (cal.y < 0)
						{
							cal.y++;
						}

						//���� �ε����� ���� �����ؼ� ���ϱ� ���̾���? �׷� �ڽ�Ʈ ������
						if ((*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
							(*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
						{
							cost += WALL_COST;
						}
					}
				}

				//x�� 0�� �ƴ϶��  0�̵ɋ����� ++ or --
				if (cal.x != 0)
				{
					cost += 10;

					//��ȣ������
					if (cal.x > 0)
					{
						//���̰ų�
						cal.x--;
					}
					else if (cal.x < 0)
					{
						//����
						cal.x++;
					}

					if ((*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
						(*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
					{
						cost += WALL_COST;
					}
				}
				//y�� 0�� �ƴ϶�� 0�̵ɋ����� ++ or --
				if (cal.y != 0)
				{
					cost += 10;

					if (cal.y > 0)
					{
						cal.y--;
					}
					else if (cal.y < 0)
					{
						cal.y++;
					}
					if (torchIdx.x + j >= 0 && torchIdx.x + j <= _vvLightMap[0][0].size() - 1 &&
						torchIdx.y + i >= 0 && torchIdx.y + i <= _vvLightMap->size() - 1)
					{

						if ((*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
							(*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
						{
							cost += WALL_COST;
						}
					}
				}
			}
			// ���ѷ��������� Ż�����޴�. 2������ ����� ��.

			if (torchIdx.x + j == 15 && torchIdx.y + i == 14)
			{
				int a = 0;
			}


			//	1. �ڽ�Ʈ�� �ٽἭ ���������(�����ȿ� ���������)
			if (cost > maxCost)
			{
				//1) �߰��غô� ���̾����ÿ�
				if (torchIdx.x + j >= 0 && torchIdx.x + j <= _vvLightMap[0][0].size() - 1 &&
					torchIdx.y + i >= 0 && torchIdx.y + i <= _vvLightMap->size() - 1)
				{
					if ((*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->getIsFind() > cost)
					{
						//1-1) �װ��� �ٽ� �߰��������� ������ ĥ��
						(*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->setOpacity(OPACITY_FIND);
					}
					//2) �̾Ʒ��� �� �ǳʶ�
					continue;
				}
			}

			//	2. �ִ� �ڽ�Ʈ �ȿ� �����������(�����ȿ� �־������)

			// 1)�߰��Ѱɷ� ����.
			(*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->setIsFind(true);
			// 2)�ڽ�Ʈ�� ������ �ٽ� �����
			cost /= maxCost;	//�ִ� �ڽ�Ʈ�� ������
			cost *= 0.4f;		//0.4�� ���ϴµ� ���ϸ� �ʹ� ����. 

			// 3)���� ����� ���� �ڽ�Ʈ�� ���� �ε����� ����(���� �ε������� �����ε��� i,j)
			if ((*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->getOpacity() > cost)
			{
				float tmp = (*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->getOpacity();
				float tmp2 = cost;
				(*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->setOpacity(cost);
			}

			//���� �Ʒ������̶� ���� �� ĥ����ߵǼ� �߰��Ѱ�.
			if (torchIdx.y + i >= 0 && torchIdx.x + i >= 0									// ��������
				&& torchIdx.y <= _vvObj->size() && torchIdx.x <= _vvObj[0][0].size()				// ��������
				&& (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
				(*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)	// �Ӽ��� ���̸�
			{
				//��������
				if (torchIdx.y + i - 1 >= 0 && torchIdx.y + i - 1 <= _vvLightMap->size())
				{
					//���϶� �ٷ� �����ٰ� ���� ���� �ڽ�Ʈ�� �߰��ߴٰ� ����.
					(*_vvLightMap)[torchIdx.y + i - 1][torchIdx.x + j]->setIsFind(true);
					(*_vvLightMap)[torchIdx.y + i - 1][torchIdx.x + j]->setOpacity(cost);
				}
			}
		}
	}


}

void rayCast::checkMap(POINT playerIdx, int torchRange, int tileSizeX, int tileSizeY)
{
	for (int i = 0; i < tileSizeY; i++)
	{
		for (int j = 0; j < tileSizeX; j++)
		{
			if (i >= playerIdx.y - torchRange && playerIdx.y + torchRange <= i &&
				j >= playerIdx.x - torchRange && playerIdx.x + torchRange <= j)
			{
				continue;
			}

			if ((*_vvLightMap)[i][j]->getIsFind())
			{
				(*_vvLightMap)[i][j]->setOpacity(OPACITY_FIND);
			}
			else
			{
				(*_vvLightMap)[i][j]->setOpacity(1.0f);
			}
		}
	}
}
