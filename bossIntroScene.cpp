#include "stdafx.h"
#include "bossIntroScene.h"


bossIntroScene::bossIntroScene()
{
}


bossIntroScene::~bossIntroScene()
{
}

HRESULT bossIntroScene::init()
{


	_topPos = { 0, 40 };
	_midPos = { 0, 120 };
	_bottomPos = { 230, 430 };

	_topPos.x = -270;
	_midPos.x = 425;
	_bottomPos.x = 960;

	_alpha = 0;

	SOUNDMANAGER->play("sound_boss_intro");

	return S_OK;
}

void bossIntroScene::release()
{
}

void bossIntroScene::update()
{
	topMove(30);

	if (midMove(30))
		bottomMove(50);

	showBoss();
}

void bossIntroScene::render()
{
	IMAGEMANAGER->findImage("boss_top")->render(_topPos.x, _topPos.y);
	IMAGEMANAGER->findImage("boss_bottom")->render(_bottomPos.x, _bottomPos.y);
	IMAGEMANAGER->findImage("boss_background")->render(_midPos.x, _midPos.y);
	IMAGEMANAGER->findImage("boss_face")->render(_midPos.x, _midPos.y - 95);
	IMAGEMANAGER->findImage("boss_name")->render(_bottomPos.x + 350, _bottomPos.y + 20);

	IMAGEMANAGER->findImage("black")->render(0, 0, WINSIZEX, WINSIZEY, _alpha);
}

void bossIntroScene::topMove(int num)
{
	if (_topPos.x != 0)
	{
		_topPos.x += num;
		if (_topPos.x > 0)
		{
			_topPos.x = 0;
		}
	}
}

bool bossIntroScene::midMove(int num)
{
	if (_midPos.x != 0)
	{
		_midPos.x -= num;
		if (_midPos.x < 0)
		{
			_midPos.x = 0;

		}
	}
	else
		return true;

	return false;
}

void bossIntroScene::bottomMove(int num)
{
	if (_bottomPos.x != 0)
	{
		_bottomPos.x -= num;
		if (_bottomPos.x < 230)
			_bottomPos.x = 230;
	}
}

void bossIntroScene::showBoss()
{
	if (!SOUNDMANAGER->isPlaySound("sound_boss_intro"))
	{
		if (_alpha >= 0)
		{
			_alpha += 0.1f;
		}
		if (_alpha >= 1.0f)
		{
			_alpha = 1.0f;
			if (_alpha > 0.9f || KEYMANAGER->isOnceKeyDown(VK_RETURN) || KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				SCENEMANAGER->changeScene("bossScene");
			}
		}
	}
}
