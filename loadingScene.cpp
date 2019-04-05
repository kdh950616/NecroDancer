#include "stdafx.h"
#include "loadingScene.h"


loadingScene::loadingScene()
{
}


loadingScene::~loadingScene()
{
}

HRESULT loadingScene::init()
{
	SOUNDMANAGER->play("loadingScene", SOUNDMANAGER->getVolume());

	return S_OK;
}

void loadingScene::release()
{
}

void loadingScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) || KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		SOUNDMANAGER->stop("loadingScene");
		SCENEMANAGER->changeScene("lobbyScene");
	}
}

void loadingScene::render()
{
	IMAGEMANAGER->findImage("title")->render(0,0);
	IMAGEMANAGER->findImage("continue")->render(WINSIZEX / 2 - IMAGEMANAGER->findImage("continue")->GetWidth() / 2, 500);
}
