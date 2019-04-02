#include "stdafx.h"
#include "king.h"
#include "player.h"


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

	_showPos = { 0,40 };
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

		if (_isSpecialAct == true)
		{
			_savePos = _posLT;

			if (_listPath.size() > 0 && _direction.x == 0 && _direction.y == 0)
			{
				_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
			}
			else
			{	

			}

			if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				attackPlayer(_dmg);

				int rnd = RND->getFromIntTo(1, 8);

				switch (rnd)
				{
				case 1:
					SOUNDMANAGER->playEff("king_Attack1");
					break;
				case 2:
					SOUNDMANAGER->playEff("king_Attack2");
					break;
				case 3:
					SOUNDMANAGER->playEff("king_Attack3");
					break;
				case 4:
					SOUNDMANAGER->playEff("king_Attack4");
					break;
				case 5:
					SOUNDMANAGER->playEff("king_Attack5");
					break;
				case 6:
					SOUNDMANAGER->playEff("king_Attack6");
					break;
				case 7:
					SOUNDMANAGER->playEff("king_Attack7");
					break;
				}
			}
			else
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				switch (_direction.x)
				{
				case -1:
					_vec.x -= _speed;
					break;
				case 0:
					_vec.x = 0;
					break;
				case 1:
					_vec.x += _speed;
					break;
				}

				switch (_direction.y)
				{
				case -1:
					_vec.y -= _speed;
					break;
				case 0:
					_vec.y = 0;
					break;
				case 1:
					_vec.y += _speed;
					break;
				}
			}
		}
		//아직 체스말들이 남았다면펄스 -> 다죽고 혼자남으면 트루가됨
		else if (_isSpecialAct == false)
		{
			if (_destIdx.x - _idx.x > 0)
			{
				_direction.x = 1;
			}
			else if (_destIdx.x - _idx.x < 0)
			{
				_direction.x = -1;
			}
			else
			{
				_direction.x = 0;
			}

			if (_destIdx.y - _idx.y > 0)
			{
				_direction.y = 1;
			}
			else if (_destIdx.y - _idx.y < 0)
			{
				_direction.y = -1;
			}
			else
			{
				_direction.y = 0;
			}

			//위에서 구해진곳이 갈수없다면 안감
			if ((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false || 
				(*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
			{
				_direction = { 0,0 };
			}
			else
			{
				_dustAni->start();
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				_isMove = true;
				_savePos = _posLT;
				switch (_direction.x)
				{
				case -1:
					_vec.x -= _speed;
					break;
				case 0:
					_vec.x = 0;
					break;
				case 1:
					_vec.x += _speed;
					break;
				}

				switch (_direction.y)
				{
				case -1:
					_vec.y -= _speed;
					break;
				case 0:
					_vec.y = 0;
					break;
				case 1:
					_vec.y += _speed;
					break;
				}
			}
			
		//	//킹 이 도망치는거 짜고있었음. destIdx를 맨첨에 x == 1 , y == 6으로 설정하고 만약 isSpecialAct가 펄스일떄(도망칠떄)
		//	//쳐맞으면 destIdx가 1,6 이엇다면 8,13으로 8의13이엇으면 1,6으로 도망치게끔 가게끔 하자.

		//	//그리고 룩이랑 캐슬링하는거 룩보고 좀 참고해서 캐슬링 만들고
		//	// 적들 다죽으면 isSpecialAct 가 트루되면서 aStar로 플레이어 따라오게.
			// 따라오는중에 맞으면 밴쉬처럼 뒤로밀리게 -> 룩, 퀸, 밴쉬 코드 참고.
			// 킹 끝나면 스테이지 y값에따라 전투하고. 필드 위에랑 아래 렌더안함 + 아래는 속성 빈걸로,
			// 에너미사이즈 0되면 위에 렌더 함.

			// 몬스터 체력 표기... 오른쪽 상당 UI 표기.... 안되면 포기하고 패스.

			//최종 촬영하기전 준비 -> 집에서 사양좋은 컴가지고 하자.
			// 테스트씬으로 몬스터 설명. 스테이지 1, 2 로 시연 보스스테이지로 시연 + 설명. 
			
		}
	}
	if (_isMove)
	{
		moveCal();
	}
	if (_isCastling)
	{
		_savePos = _posLT;
		_dustAni->start();
		_isMove = true;
		if (_direction.x == 3)
		{
			_vec.x = _speed;
			_destIdx = { 8,6 };
		}
		else if (_direction.x == -2)
		{
			_vec.x -= _speed;
			_destIdx = { 1,6 };
		}
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
		(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
		_savePos = _posLT;
		_jumpPower = JUMPPOWER_HORIZON;
		_posZ = 0;
		_isCastling = false;
	}

	if (_curHp <= 0)
	{
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	}

	if (_isAttack)
	{
		countAttackEffect();
	}
}

void king::render()
{
	enemy::render();

	if (_isMove)
	{
		_dustImg->aniRender(_savePos.x - CAMERA->getPosX(), _savePos.y - CAMERA->getPosY(), _dustAni);
	}
	_shadowImg->render(_posLT.x - CAMERA->getPosX() - 3, _posLT.y - 15 - CAMERA->getPosY(), 1.0f);

	_img->aniRender(_posLT.x - CAMERA->getPosX() , _posLT.y - TILESIZE - _posZ - CAMERA->getPosY(), _ani);

	if (_isAttack)
	{
		IMAGEMANAGER->findImage("enemy_Attack")->frameRenderAngle((_idx.x + _atkDirection.x) * TILESIZE - CAMERA->getPosX(), (_idx.y + _atkDirection.y) * TILESIZE - CAMERA->getPosY(), _atkEff_Frame, 0, _atkAngle);
	}
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
	if ((_posLT.x - _savePos.x >= TILESIZE || _savePos.x - _posLT.x >= TILESIZE ||
		_posLT.y - _savePos.y >= TILESIZE || _savePos.y - _posLT.y >= TILESIZE) && _direction.x != -2 && _direction.x != 3)
	{
		if (_vec.x > 0)
		{
			_posLT.x = _savePos.x + TILESIZE;
		}
		else if (_vec.x < 0)
		{
			_posLT.x = _savePos.x - TILESIZE;
		}
		if (_vec.y > 0)
		{
			_posLT.y = _savePos.y + TILESIZE;
		}
		else if (_vec.y < 0)
		{
			_posLT.y = _savePos.y - TILESIZE;
		}
		_idx.x = (_posLT.x) / TILESIZE;
		_idx.y = (_posLT.y) / TILESIZE;
		(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_vec.x = 0;
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;
		_direction = { 0,0 };
		_jumpPower = 0;
		_posZ = 0;
	}
	else if (_direction.x == -2)
	{
		if ((_posLT.x - _savePos.x >= TILESIZE * 2 || _savePos.x - _posLT.x >= TILESIZE * 2))
		{
			if (_vec.x > 0)
			{
				_posLT.x = _savePos.x + TILESIZE * 2;
			}
			else if (_vec.x < 0)
			{
				_posLT.x = _savePos.x - TILESIZE * 2;
			}
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
			_vec.x = 0;
			_savePos = _posLT;
			_isMove = false;
			_direction = { 0,0 };
			_jumpPower = 0;
			_posZ = 0;
			_isFind = true;
			_isBeat = false;
		}
	}
	else if (_direction.x == 3)
	{
		if ((_posLT.x - _savePos.x >= TILESIZE * 3 || _savePos.x - _posLT.x >= TILESIZE * 3))
		{
			if (_vec.x > 0)
			{
				_posLT.x = _savePos.x + TILESIZE * 3;
			}
			else if (_vec.x < 0)
			{
				_posLT.x = _savePos.x - TILESIZE * 3;
			}
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
			(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
			_vec.x = 0;
			_savePos = _posLT;
			_isMove = false;
			_direction = { 0,0 };
			_jumpPower = 0;
			_posZ = 0;
			_isFind = true;
			_isBeat = false;
		}
	}

	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
}
