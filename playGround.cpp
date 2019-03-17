#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init(true);

	_mapEditor = new mapEditor;
	_testScene = new testScene;


	SOUNDMANAGER->addSound("mapTool", "sounds/mapTool/boss_9.ogg", true, true);
	SOUNDMANAGER->addSound("mapTool_shopkeeper", "sounds/mapTool/boss_9_vocal.ogg", true, true);
	SOUNDMANAGER->addSound("testScene", "sounds/zone/zone1_1.ogg", true, true);
	SOUNDMANAGER->addSound("testScene_shopkeeper", "sounds/zone/zone1_1_shopkeeper.ogg", true, true);



	SCENEMANAGER->addScene("mapToolScene",_mapEditor);
	SCENEMANAGER->addScene("testScene", _testScene);


	//SCENEMANAGER->changeScene("mapToolScene");
	SCENEMANAGER->changeScene("testScene");

	
	return S_OK;
}

void playGround::release()
{
	gameNode::release();
	
}

void playGround::update()
{
	gameNode::update();

	SCENEMANAGER->update();
	EFFECTMANAGER->update();

}

void playGround::render()
{
	// Draw ���� - �� �ڵ尡 ������ D2D ��� X
	D2DMANAGER->beginDraw();
	//===========================================================================
	//				##		���⿡ �ڵ� �ۼ�(Start)			##



	SCENEMANAGER->render();
	EFFECTMANAGER->render();



	//				##			���⿡ �ڵ� �ۼ�(End)			##
	//===========================================================================
	//				##ī�޶� ���� ���콺 ���� �ð����� ���	##===================
	WCHAR str[128];
	swprintf_s(str, L"cameraX : %d", CAMERA->getPosX());
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 80);
	D2DMANAGER->drawText(str, 0, 80,20,0xff00ff);
	swprintf_s(str, L"cameraY : %d", CAMERA->getPosY());
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 100);
	D2DMANAGER->drawText(str, 0, 100, 20, 0xff00ff);

	swprintf_s(str, L"mouseX : %.2f", _ptMouse.x);
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 140);
	D2DMANAGER->drawText(str, 0, 140, 20, 0xff00ff);
	swprintf_s(str, L"mouseY : %.2f", _ptMouse.y);
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 160);
	D2DMANAGER->drawText(str, 0, 160, 20, 0xff00ff);

	TIMEMANAGER->render();
	// Draw �� - �� �ڵ尡 ������ D2D ��� X
	D2DMANAGER->endDraw();
}