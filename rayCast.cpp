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
			//코스트는 0으로 시작해서 코스트가 낮을수록 (이동을 적게할수록) 투명도가 높아짐 == 밝음
			int cost = 0;
			int maxCost = torchRange * 10;
			POINT cal = { j,i };
			while (1)
			{
				//=====================
				//		탈출
				//=====================
				if (cal.x == 0 && cal.y == 0)
				{
					//도착햇거나
					break;
				}
				if (cost > maxCost)
				{
					//코스트가 오버되면
					break;
				}

				if (abs(cal.x) == abs(cal.y))
				{
					if (cal.x != 0 && cal.y != 0)
					{
						cost += 14;

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

					//부호에따라
					if (cal.x > 0)
					{
						cal.x--;
					}
					else if (cal.x < 0)
					{
						cal.x++;
					}
				}

				if (cal.y != 0)
				{
					cost += 10;

					//부호에따라
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
			// 이제 무한루프문에서 탈출을햇다. 2가지의 결론이 남.
			//	1. 코스트를 다써서 나옴. -> 바로아래의 이프문에 걸려서 시야설정하는부분 건너뜀
			//	2. 횃불로 인해서 정해진 코스트내에 플레이어한테 도달함 -> 시야 설정
			//=========================================================================================================================================================

			//========================================================================
			//						1번일 경우 여기서 걸림
			//========================================================================

			// 자 지금 뭐했냐면 포문 플레이어 기준으로 - torch범위 부터 + torch범위로 해쓴ㄴ데 지금
			//for문 고치는 중이다. 어. 잘 하도록. 이상!
		}
	}

	for (int i = startY ; i <= endY; i++)
	{
		for (int j = startX ; j <= endX ; j++)
		{
			//횟불 기본 2임(아템먹으면 최대 5까지)
			int cost = torchRange * 10;
			POINT index = { j,i };

			//테스트용
			if (i == 9)
			{
				if (j == 11)
				{
					POINT index = { j,i };
				}
			}

			while (1)
			{
				//=====================
				//		탈출
				//=====================
				if (index.x == playerIdx.x && index.y == playerIdx.y)
				{
					//무사히 코스트를 다쓰지않고 도착했거나
					break;
				}
				if (cost < 0)
				{
					//코스트를 다쓰면 탈출
					break;
				}
				
				//====================
				//		대각선
				//====================
				if (abs(index.x) == abs(index.y))							//x와 y의 절대값이 서로 같을시 (대각 or 도착)
				{
					//x와 y가 도착점이 아닐시 -> 대각선만 적용
					if (index.x != playerIdx.x && index.y != playerIdx.y)
					{
						//16점 깍음
						cost -= 16;

						//절대값으로 계산햇으니 부호에 따라 ++ or --
						if (index.x > playerIdx.x)
						{
							index.x--;
						}
						else if (index.x < playerIdx.x)
						{
							index.x++;
						}

						if (index.y > playerIdx.y)
						{
							index.y--;
						}
						else if (index.y < playerIdx.y)
						{
							index.y++;
						}
					}
				}

				//x가 플레이어 좌표가 맞지않으면
				if (index.x != playerIdx.x)
				{
					cost -= 10;

					//부호에따라
					if (index.x > playerIdx.x)
					{
						index.x--;
					}
					else if (index.x < playerIdx.x)
					{
						index.x++;
					}
				}

				//y가 플레이어좌표와 맞지않으면
				if (index.y != playerIdx.y)
				{
					cost -= 10;

					if (index.y > playerIdx.y)
					{
						index.y--;
					}
					else if (index.y < playerIdx.y)
					{
						index.y++;
					}
				}
			}

			//=========================================================================================================================================================
			// 이제 무한루프문에서 탈출을햇다. 2가지의 결론이 남.
			//	1. 횃불로 인해서 정해진 코스트내에 플레이어한테 도달함 -> 시야 설정
			//	2. 코스트를 다써서 나옴. -> 바로아래의 이프문에 걸려서 시야설정하는부분 건너뜀
			//=========================================================================================================================================================

			//========================================================================
			//						2번일 경우 여기서 걸림
			//========================================================================
			if (cost < 0)
			{
				//만약 코스트가 0보다 낮으면? 사거리 밖에있는거니까 걍 건너뜀.

				if ((*_vvLightMap)[i][j]->getIsFind())
				{
					(*_vvLightMap)[i][j]->setOpacity(0.7f);
				}

				continue;
			}

			//========================================================================
			//						1번일 경우 여기까지와서 시야설정해줌.
			//========================================================================

			//코스트가 높을수록(플레이어와 가까울경우) opacity가 1에 가깝게 코스트가 낮을수록(플레이어와 멀경우) opacity 가 0에 가깝게
			(*_vvLightMap)[i][j]->setIsFind(true);

			if (cost % 10 != 0)
			{
				cost += 6;
			}
			float f_cost = 1.0f - ((float)cost / (torchRange * 10));

			//스위치 문에서 더해주는 이유는 수직 맨끝라인이 코스트가 딱 0이라서 조금씩 더해줌.
			switch (torchRange)
			{
				case 2:		//기본 횃불
					f_cost -= 0.3f;
				break;
				case 3:		// +1 횃불
					f_cost -= 0.4f;
				break;
				case 4:		// +2 횃불
					f_cost -= 0.5f;
				break;
				case 5:		// +3 횃불
					f_cost -= 0.6f;
				break;
			}
			
			//설정된 opacity값을 넣어줌
			(*_vvLightMap)[i][j]->setOpacity(f_cost);

		}
	}

	(*_vvLightMap)[playerIdx.y][playerIdx.x]->setOpacity(0);
	if ((*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->getIsFind())
	{
		(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->setOpacity(0.7f);
	}
	if ((*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->getIsFind())
	{
		(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->setOpacity(0.7f);
	}
	if ((*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->getIsFind())
	{
		(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->setOpacity(0.7f);
	}
	if ((*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->getIsFind())
	{
		(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->setOpacity(0.7f);
	}


}
