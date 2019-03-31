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
		
		//���� ü�������� ���Ҵٸ��޽� -> ���װ� ȥ�ڳ����� Ʈ�簡��
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

			//������ ���������� �������ٸ� �Ȱ�
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
			//ŷ �� ����ġ�°� ¥���־���. destIdx�� ��÷�� x == 1 , y == 6���� �����ϰ� ���� isSpecialAct�� �޽��ϋ�(����ĥ��)
			//�ĸ����� destIdx�� 1,6 �̾��ٸ� 8,13���� 8��13�̾����� 1,6���� ����ġ�Բ� ���Բ� ����.

			//�׸��� ���̶� ĳ�����ϴ°� �躸�� �� �����ؼ� ĳ���� �����
			// ���� �������� isSpecialAct �� Ʈ��Ǹ鼭 aStar�� �÷��̾� �������.
			// ��������߿� ������ �꽬ó�� �ڷιи��� -> ��, ��, �꽬 �ڵ� ����.
			// ŷ ������ �������� y�������� �����ϰ�. �ʵ� ������ �Ʒ� �������� + �Ʒ��� �Ӽ� ��ɷ�,
			// ���ʹ̻����� 0�Ǹ� ���� ���� ��.

			// ���� ü�� ǥ��... ������ ��� UI ǥ��.... �ȵǸ� �����ϰ� �н�.

			//���� �Կ��ϱ��� �غ� -> ������ ������� �İ����� ����.
			// �׽�Ʈ������ ���� ����. �������� 1, 2 �� �ÿ� �������������� �ÿ� + ����. 
			
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
