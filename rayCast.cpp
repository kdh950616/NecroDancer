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
			//터짐방지
			if (playerIdx.y + i < 0 || playerIdx.x + j < 0
				||playerIdx.y + i > _vvLightMap->size() - 1
				||playerIdx.x + j > _vvLightMap[0][0].size() - 1)
			{
				continue;
			}
			
			float cost = 0;					//코스트는 0으로 시작해서 코스트가 낮을수록 (이동을 적게할수록) 투명도가 높아짐 == 밝음

			int maxCost = torchRange * 10;	//코스트 최대한도는 횃불세기의 * 10
			POINT cal = { j,i };			//인덱스 계산용([0][0]이 플레이어임)

			while (1)
			{
				//=====================
				//		탈출
				//=====================
				if (cal.x == 0 && cal.y == 0)
				{
					//도착하면 탈출
					break;
				}
				if (cost > maxCost)
				{
					//코스트가 오버되면 탈출
					break;
				}

				//계산용 인덱스의 절댓값이 같다면 == 대각선이라면
				if (abs(cal.x) == abs(cal.y))
				{
					if (cal.x != 0 && cal.y != 0)
					{
						//대각선은 16을더하고
						cost += 16;

						//0보다 큰지 작은지를 판별해서 ++ or --
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

						//계산용 인덱스를 새로 적용해서 보니까 벽이었다? 그럼 코스트 또증가
						if ((*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
							(*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
						{
							cost += WALL_COST;
						}
					}
				}

				//x가 0이 아니라면  0이될떄까지 ++ or --
				if (cal.x != 0)
				{
					cost += 10;

					//부호에따라
					if (cal.x > 0)
					{
						//까이거나
						cal.x--;
					}
					else if (cal.x < 0)
					{
						//더함
						cal.x++;
					}

					if ((*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
						(*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
					{
						cost += WALL_COST;
					}
				}
				//y가 0이 아니라면 0이될떄까지 ++ or --
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
			// 무한루프문에서 탈출을햇다. 2가지의 결론이 남.

			//	1. 코스트를 다써서 나왔을경우(범위안에 없었던경우)
			if (cost > maxCost)
			{
				//1) 발견해봤던 곳이었을시엔
				if (playerIdx.x + j >= 0 && playerIdx.x + j <= _vvLightMap[0][0].size() - 1 &&
					playerIdx.y + i >= 0 && playerIdx.y + i <= _vvLightMap->size() - 1)
				{
					if ((*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->getIsFind())
					{
						//1-1) 그곳을 다시 발견했을때의 값으로 칠함
						(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setOpacity(OPACITY_FIND);
					}
					//2) 이아래는 다 건너뜀
					continue;
				}
			}
			
			//	2. 최대 코스트 안에 도착했을경우(범위안에 있엇던경우)

			// 1)발견한걸로 설정.
			(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setIsFind(true);
			// 2)코스트를 비율로 다시 계산함
			cost /= maxCost;	//최대 코스트로 나누고
			cost *= 0.4f;		//0.4를 곱하는데 안하면 너무 밝음. 

			// 3)최종 계산이 끝난 코스트를 현재 인덱스에 넣음(계산용 인덱스말고 실제인덱스 i,j)

			(*_vvLightMap)[playerIdx.y + i][playerIdx.x + j]->setOpacity(cost);
			
			//여기에다가 오브젝트[playerIdx.y + i][playerIdx.x + j]의 속성에 횟불도 있다면 rayCast를 재귀로 더돔 +추가로 i와 j가 0,0이 아니고
			if ((*_vvObj)[playerIdx.y + i][playerIdx.x + j]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + i][playerIdx.x + j]->getAttribute() <= ETC_TORCH_BOSS)
			{
				rayCastingTorch({ playerIdx.x + j , playerIdx.y + i }, 4);
			}

			//벽이 아랫기준이라 위로 더 칠해줘야되서 추가한것.
			if (playerIdx.y + i >= 0 && playerIdx.x + i >= 0									// 터짐방지
				&&playerIdx.y <= _vvObj->size() && playerIdx.x <= _vvObj[0][0].size()				// 터짐방지
				&& (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
				(*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[playerIdx.y + cal.y][playerIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)	// 속성이 벽이면
			{
				//터짐방지
				if (playerIdx.y + i - 1 >= 0 && playerIdx.y + i - 1 <= _vvLightMap->size())
				{
					//벽일때 바로 위에다가 계산된 같은 코스트랑 발견했다고 해줌.
					(*_vvLightMap)[playerIdx.y + i - 1][playerIdx.x + j]->setIsFind(true);
					(*_vvLightMap)[playerIdx.y + i - 1][playerIdx.x + j]->setOpacity(cost);
				}
			}
		}
	}

	//여기서부턴 있는 조건문들은 위에서 검사한곳의 1칸더 겉부분을 발견했던거라면 했을때의 값으로 칠해주는 거임

	//터짐방지
	if (playerIdx.y - (torchRange + 1) >= 0 && playerIdx.y - (torchRange + 1) <= _vvLightMap->size() - 1
		&&(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->getIsFind())
	{
		//칠하고
		(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x]->setOpacity(OPACITY_FIND);

		//터짐방지
		if (playerIdx.x - 1 >= 0)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x - 1]->setOpacity(OPACITY_FIND);
		}
		//터짐방지
		if (playerIdx.x + 1 <= _vvLightMap->size() - 1)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y - (torchRange + 1)][playerIdx.x + 1]->setOpacity(OPACITY_FIND);
		}
	}

	//터짐방지
	if (playerIdx.y + (torchRange + 1) >= 0 && playerIdx.y + (torchRange + 1) <= _vvLightMap->size() - 1
		&&(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->getIsFind())
	{
		//칠하고
		(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x]->setOpacity(OPACITY_FIND);
		//터짐방지
		if (playerIdx.x - 1 >= 0)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x - 1]->setOpacity(OPACITY_FIND);
		}
		//터짐방지
		if (playerIdx.x + 1 <= _vvLightMap[0].size() - 1)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y + (torchRange + 1)][playerIdx.x + 1]->setOpacity(OPACITY_FIND);
		}
	}

	//터짐방지
	if (playerIdx.x - (torchRange + 1) >= 0 && playerIdx.x - (torchRange + 1) <= _vvLightMap[0][0].size() - 1
		&&(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->getIsFind())
	{
		//칠하고
		(*_vvLightMap)[playerIdx.y][playerIdx.x - (torchRange + 1)]->setOpacity(OPACITY_FIND);
		//터짐방지
		if (playerIdx.y - 1 >= 0)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y - 1][playerIdx.x - (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
		//터짐방지
		if (playerIdx.y + 1 <= _vvLightMap->size() - 1)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y + 1][playerIdx.x - (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
	}
	//터짐방지
	if (playerIdx.x + (torchRange + 1) >= 0 && playerIdx.x + (torchRange + 1) <= _vvLightMap[0][0].size() - 1
		&&(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->getIsFind())
	{
		//칠하고
		(*_vvLightMap)[playerIdx.y][playerIdx.x + (torchRange + 1)]->setOpacity(OPACITY_FIND);
		//터짐방지
		if (playerIdx.y - 1 >= 0)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y - 1][playerIdx.x + (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
		//터짐방지
		if (playerIdx.y + 1 <= _vvLightMap->size() - 1)
		{
			//칠하고
			(*_vvLightMap)[playerIdx.y + 1][playerIdx.x + (torchRange + 1)]->setOpacity(OPACITY_FIND);
		}
	}



	//망작 
	{
		//for (int i = startY ; i <= endY; i++)
		//{
		//	for (int j = startX ; j <= endX ; j++)
		//	{
		//		//횟불 기본 2임(아템먹으면 최대 5까지)
		//		int cost = torchRange * 10;
		//		POINT index = { j,i };
		//
		//		//테스트용
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
		//			//		탈출
		//			//=====================
		//			if (index.x == playerIdx.x && index.y == playerIdx.y)
		//			{
		//				//무사히 코스트를 다쓰지않고 도착했거나
		//				break;
		//			}
		//			if (cost < 0)
		//			{
		//				//코스트를 다쓰면 탈출
		//				break;
		//			}
		//			
		//			//====================
		//			//		대각선
		//			//====================
		//			if (abs(index.x) == abs(index.y))							//x와 y의 절대값이 서로 같을시 (대각 or 도착)
		//			{
		//				//x와 y가 도착점이 아닐시 -> 대각선만 적용
		//				if (index.x != playerIdx.x && index.y != playerIdx.y)
		//				{
		//					//16점 깍음
		//					cost -= 16;
		//
		//					//절대값으로 계산햇으니 부호에 따라 ++ or --
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
		//			//x가 플레이어 좌표가 맞지않으면
		//			if (index.x != playerIdx.x)
		//			{
		//				cost -= 10;
		//
		//				//부호에따라
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
		//			//y가 플레이어좌표와 맞지않으면
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
		//		// 이제 무한루프문에서 탈출을햇다. 2가지의 결론이 남.
		//		//	1. 횃불로 인해서 정해진 코스트내에 플레이어한테 도달함 -> 시야 설정
		//		//	2. 코스트를 다써서 나옴. -> 바로아래의 이프문에 걸려서 시야설정하는부분 건너뜀
		//		//=========================================================================================================================================================
		//
		//		//========================================================================
		//		//						2번일 경우 여기서 걸림
		//		//========================================================================
		//		if (cost < 0)
		//		{
		//			//만약 코스트가 0보다 낮으면? 사거리 밖에있는거니까 걍 건너뜀.
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
		//		//						1번일 경우 여기까지와서 시야설정해줌.
		//		//========================================================================
		//
		//		//코스트가 높을수록(플레이어와 가까울경우) opacity가 1에 가깝게 코스트가 낮을수록(플레이어와 멀경우) opacity 가 0에 가깝게
		//		(*_vvLightMap)[i][j]->setIsFind(true);
		//
		//		if (cost % 10 != 0)
		//		{
		//			cost += 6;
		//		}
		//		float f_cost = 1.0f - ((float)cost / (torchRange * 10));
		//
		//		//스위치 문에서 더해주는 이유는 수직 맨끝라인이 코스트가 딱 0이라서 조금씩 더해줌.
		//		switch (torchRange)
		//		{
		//			case 2:		//기본 횃불
		//				f_cost -= 0.3f;
		//			break;
		//			case 3:		// +1 횃불
		//				f_cost -= 0.4f;
		//			break;
		//			case 4:		// +2 횃불
		//				f_cost -= 0.5f;
		//			break;
		//			case 5:		// +3 횃불
		//				f_cost -= 0.6f;
		//			break;
		//		}
		//		
		//		//설정된 opacity값을 넣어줌
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
			//터짐방지
			if (torchIdx.y + i < 0 || torchIdx.x + j < 0
				|| torchIdx.y + i > _vvLightMap->size() - 1
				|| torchIdx.x + j > _vvLightMap[0][0].size() - 1)
			{
				continue;
			}
			if (i == 0 && j == 0)continue;

			float cost = 0;					//코스트는 0으로 시작해서 코스트가 낮을수록 (이동을 적게할수록) 투명도가 높아짐 == 밝음

			int maxCost = torchRange * 10;	//코스트 최대한도는 횃불세기의 * 10
			POINT cal = { j,i };			//인덱스 계산용([0][0]이 플레이어임)

			while (1)
			{
				//=====================
				//		탈출
				//=====================
				if (cal.x == 0 && cal.y == 0)
				{
					//도착하면 탈출
					break;
				}
				if (cost > maxCost)
				{
					//코스트가 오버되면 탈출
					break;
				}

				//계산용 인덱스의 절댓값이 같다면 == 대각선이라면
				if (abs(cal.x) == abs(cal.y))
				{
					if (cal.x != 0 && cal.y != 0)
					{
						//대각선은 16을더하고
						cost += 16;

						//0보다 큰지 작은지를 판별해서 ++ or --
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

						//계산용 인덱스를 새로 적용해서 보니까 벽이었다? 그럼 코스트 또증가
						if ((*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
							(*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
						{
							cost += WALL_COST;
						}
					}
				}

				//x가 0이 아니라면  0이될떄까지 ++ or --
				if (cal.x != 0)
				{
					cost += 10;

					//부호에따라
					if (cal.x > 0)
					{
						//까이거나
						cal.x--;
					}
					else if (cal.x < 0)
					{
						//더함
						cal.x++;
					}

					if ((*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
						(*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)
					{
						cost += WALL_COST;
					}
				}
				//y가 0이 아니라면 0이될떄까지 ++ or --
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
			// 무한루프문에서 탈출을햇다. 2가지의 결론이 남.

			if (torchIdx.x + j == 15 && torchIdx.y + i == 14)
			{
				int a = 0;
			}


			//	1. 코스트를 다써서 나왔을경우(범위안에 없었던경우)
			if (cost > maxCost)
			{
				//1) 발견해봤던 곳이었을시엔
				if (torchIdx.x + j >= 0 && torchIdx.x + j <= _vvLightMap[0][0].size() - 1 &&
					torchIdx.y + i >= 0 && torchIdx.y + i <= _vvLightMap->size() - 1)
				{
					if ((*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->getIsFind() > cost)
					{
						//1-1) 그곳을 다시 발견했을때의 값으로 칠함
						(*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->setOpacity(OPACITY_FIND);
					}
					//2) 이아래는 다 건너뜀
					continue;
				}
			}

			//	2. 최대 코스트 안에 도착했을경우(범위안에 있엇던경우)

			// 1)발견한걸로 설정.
			(*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->setIsFind(true);
			// 2)코스트를 비율로 다시 계산함
			cost /= maxCost;	//최대 코스트로 나누고
			cost *= 0.4f;		//0.4를 곱하는데 안하면 너무 밝음. 

			// 3)최종 계산이 끝난 코스트를 현재 인덱스에 넣음(계산용 인덱스말고 실제인덱스 i,j)
			if ((*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->getOpacity() > cost)
			{
				float tmp = (*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->getOpacity();
				float tmp2 = cost;
				(*_vvLightMap)[torchIdx.y + i][torchIdx.x + j]->setOpacity(cost);
			}

			//벽이 아랫기준이라 위로 더 칠해줘야되서 추가한것.
			if (torchIdx.y + i >= 0 && torchIdx.x + i >= 0									// 터짐방지
				&& torchIdx.y <= _vvObj->size() && torchIdx.x <= _vvObj[0][0].size()				// 터짐방지
				&& (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= OBJ_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= OBJ_DOOR_SIDE ||
				(*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() >= ETC_TORCH_WALL1 && (*_vvObj)[torchIdx.y + cal.y][torchIdx.x + cal.x]->getAttribute() <= ETC_TORCH_BOSS)	// 속성이 벽이면
			{
				//터짐방지
				if (torchIdx.y + i - 1 >= 0 && torchIdx.y + i - 1 <= _vvLightMap->size())
				{
					//벽일때 바로 위에다가 계산된 같은 코스트랑 발견했다고 해줌.
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
