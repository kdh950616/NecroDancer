#include "stdafx.h"
#include "king.h"


king::king()
{
}


king::~king()
{
}

HRESULT king::init()
{
	imageInit();

	_maxHp = 3;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 4;
	_posZ = 0;
	_enemyType = KING;
	_isNeedAstar = true;
	_savePos = _posLT;
	_gold = 0;
	_isFind = false;
	_destIdx = { 1,6 };

	return S_OK;
}

void king::release()
{
}

void king::update()
{
	if (_isFind && _isBeat)
	{
		_isBeat = false;
		
		//아직 체스말들이 남았다면펄스 -> 다죽고 혼자남으면 트루가됨
		if (_isSpecialAct == false)
		{
			if (_destIdx.x - _idx.x > 0)
			{
				_direction.x = 1;
			}
			else if (_destIdx.x - _idx.x < 0)
			{
				_direction.x = -1;
			}

			if (_destIdx.y - _idx.y > 0)
			{
				_direction.y = 1;
			}
			else if (_destIdx.y - _idx.y < 0)
			{
				_direction.y = -1;
			}

			//위에서 구해진곳이 갈수없다면 안감
			if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
			{

			}
			else if (_direction.x == 1)
			{

			}
			else if (_direction.x == -1)
			{

			}
			else if (_direction.y == 1)
			{

			}
			else if (_direction.y == -1)
			{

			}
			//킹 이 도망치는거 짜고있었음. destIdx를 맨첨에 x == 1 , y == 6으로 설정하고 만약 isSpecialAct가 펄스일떄(도망칠떄)
			//쳐맞으면 destIdx가 1,6 이엇다면 8,13으로 8의13이엇으면 1,6으로 도망치게끔 가게끔 하자.

			//그리고 룩이랑 캐슬링하는거 룩보고 좀 참고해서 캐슬링 만들고
			// 적들 다죽으면 isSpecialAct 가 트루되면서 aStar로 플레이어 따라오게.
			// 따라오는중에 맞으면 밴쉬처럼 뒤로밀리게 -> 룩, 퀸, 밴쉬 코드 참고.
			// 킹 끝나면 스테이지 y값에따라 전투하고. 필드 위에랑 아래 렌더안함 + 아래는 속성 빈걸로,
			// 에너미사이즈 0되면 위에 렌더 함.

			// 몬스터 체력 표기... 오른쪽 상당 UI 표기.... 안되면 포기하고 패스.

			//최종 촬영하기전 준비 -> 집에서 사양좋은 컴가지고 하자.
			// 테스트씬으로 몬스터 설명. 스테이지 1, 2 로 시연 보스스테이지로 시연 + 설명. 
			
		}
	}
}

void king::render()
{
}

void king::imageInit()
{
	IMAGEMANAGER->addFrameImage("king", L"images/monster/boss/king.png", 200, 180, 4, 2);
	_img = IMAGEMANAGER->findImage("king");

	KEYANIMANAGER->addAnimationType("king");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("king", "king_Stand", "king", stand, 4, ANIMATION_FPS, true);
	_ani = KEYANIMANAGER->findAnimation("king", "king_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	_shadowImg = IMAGEMANAGER->findImage("shadow_Standard");

	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);
	_dustImg = IMAGEMANAGER->findImage("dust");

	KEYANIMANAGER->addAnimationType("dust");

	int dust[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("dust", "dust_Ani", "dust", dust, 5, 10, false);

	_dustAni = KEYANIMANAGER->findAnimation("dust", "dust_Ani");
}

void king::moveCal()
{
}
