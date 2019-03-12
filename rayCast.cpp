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
	int startX = playerIdx.x - torchRange;
	int startY = playerIdx.y - torchRange;
	int endX = playerIdx.x + torchRange;
	int endY = playerIdx.y + torchRange;

	for (int i = -torchRange; i <= +torchRange; i++)
	{
		for (int j = -torchRange; j <= +torchRange; j++)
		{
			if (playerIdx.y + i < 0 || playerIdx.x + j < 0
				||playerIdx.y + i > _vvLightMap->size() - 1
				||playerIdx.x + j > _vvLightMap->size() - 1)
			{
				continue;
			}
			//�ڽ�Ʈ�� 0���� �����ؼ� �ڽ�Ʈ�� �������� (�̵��� �����Ҽ���) ������ ������ == ����
			int cost = 0;
			int maxCost = torchRange * 10;
			POINT cal = { j,i };


			while (1)
			{
				//=====================
				//		Ż��
				//=====================
				if (cal.x == 0 && cal.y == 0)
				{
					//�����ްų�
					break;
				}
				if (cost > maxCost)
				{
					//�ڽ�Ʈ�� �����Ǹ�
					break;
				}

				if (abs(cal.x) == abs(cal.y))
				{
					if (cal.x != 0 && cal.y != 0)
					{
						cost += 16;

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
					}
				}

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
				}

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
				}
			}

			//=========================================================================================================================================================
			// ���� ���ѷ��������� Ż�����޴�. 2������ ����� ��.
			//	1. �ڽ�Ʈ�� �ٽἭ ����. -> �ٷξƷ��� �������� �ɷ��� �þ߼����ϴºκ� �ǳʶ�
			//	2. ȶ�ҷ� ���ؼ� ������ �ڽ�Ʈ���� �÷��̾����� ������ -> �þ� ����
			//=========================================================================================================================================================

			//========================================================================
			//						1���� ��� ���⼭ �ɸ�
			//========================================================================

			// �� ���� ���߳ĸ� ���� �÷��̾� �������� - torch���� ���� + torch������ �ؾ����� ����
			//for�� ��ġ�� ���̴�. ��. �� �ϵ���. �̻�!

			if (cost > maxCost)
			{
				//���� �ڽ�Ʈ�� 0���� ������? ��Ÿ� �ۿ��ִ°Ŵϱ� �� �ǳʶ�.

				if ((*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->getIsFind())
				{
					(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setOpacity(OPACITY_FIND);
				}

				continue;
			}

			(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setIsFind(true);

			float fCost = (float)cost / maxCost;

			//switch (torchRange)
			//{
			//case 2:		//�⺻ ȶ��
			//	if(fCost == 1) fCost -= 0.4f;
			//	else if (fCost >= 0.8f) fCost -= 0.1f;
			//	break;
			//case 3:		// +1 ȶ��
			//	if (fCost >= 1) fCost -= 0.25f;
			//	break;
			//case 4:		// +2 ȶ��
			//	fCost -= 0.3f;
			//	break;
			//case 5:		// +3 ȶ��
			//	//if (fCost >= 1) fCost -= 0.2f;
			//	fCost -= 0.3f;
			//	break;
			//}

			//������ opacity���� �־���

			fCost *= 0.4f;

			(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setOpacity(fCost);


			//�Ƹ�����
		}
	}
	
	(*_vvLightMap)[playerIdx.y][playerIdx.x]->setIsFind(true);


	//if (playerIdx.y - (torchRange + 1) >= 0 && playerIdx.y - (torchRange + 1) <= _vvLightMap->size() - 1)
	{
		if (playerIdx.y - (torchRange + 1) >= 0 && playerIdx.y - (torchRange + 1) <= _vvLightMap->size() - 1
			&&(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->getIsFind())
		{
			(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->setOpacity(OPACITY_FIND);
		}
	}

	//if (playerIdx.y + (torchRange + 1) >= 0 && playerIdx.y + (torchRange + 1) <= _vvLightMap->size() - 1)
	{
		if (playerIdx.y + (torchRange + 1) >= 0 && playerIdx.y + (torchRange + 1) <= _vvLightMap->size() - 1
			&&(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->getIsFind())
		{
			(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->setOpacity(OPACITY_FIND);
		}
	}

	//if (playerIdx.x - (torchRange + 1) >= 0 && playerIdx.x - (torchRange + 1) <= _vvLightMap[0].size() - 1)
	{
		if (playerIdx.x - (torchRange + 1) >= 0 && playerIdx.x - (torchRange + 1) <= _vvLightMap[0].size() - 1
			&&(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->getIsFind())
		{
			(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
	}
	//if (playerIdx.x + (torchRange + 1) >= 0 && playerIdx.x + (torchRange + 1) <= _vvLightMap[0].size() - 1)
	{
		if (playerIdx.x + (torchRange + 1) >= 0 && playerIdx.x + (torchRange + 1) <= _vvLightMap[0].size() - 1
			&&(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->getIsFind())
		{
			(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->setOpacity(OPACITY_FIND);
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
